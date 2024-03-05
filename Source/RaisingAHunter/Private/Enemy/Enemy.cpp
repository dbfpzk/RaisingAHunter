// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/TextRenderComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Enemy/EnemyHealthWidget.h"
#include "Engine/Font.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utile/Keys.h"
#include "Item/Item.h"
#include "Sound/SoundWave.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InfoPosComp = CreateDefaultSubobject<USceneComponent>(TEXT("InfoPosComp"));
	InfoPosComp->SetupAttachment(GetCapsuleComponent());
	InfoPosComp->SetRelativeLocation(FVector(0, 0, 100));

	// * 타겟팅 이펙트
	TargetEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TargetEffectComp"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TargetAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonFey/FX/Particles/Fey/Abilities/Ultimate/FX/P_Ultimate_Targeting.P_Ultimate_Targeting'"));
	if (TargetAsset.Succeeded())
	{
		TargetEffectComp->SetTemplate(TargetAsset.Object);
		TargetEffectComp->SetupAttachment(InfoPosComp);
		TargetEffectComp->SetRelativeLocation(FVector::Zero());
		TargetEffectComp->SetVisibility(false);
	}
	// * 이름 텍스트
	NameTextRenderComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameTextRenderComp"));
	NameTextRenderComp->SetupAttachment(InfoPosComp);
	NameTextRenderComp->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	NameTextRenderComp->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

	// * 폰트
	static ConstructorHelpers::FObjectFinder<UFont> FontAsset(TEXT("/Script/Engine.Font'/Game/Font/Warhaven_Regular_Font.Warhaven_Regular_Font'"));
	if (FontAsset.Succeeded())
	{
		NameTextRenderComp->SetFont(FontAsset.Object);
	}

	// * 텍스트 머티리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMatAsset(TEXT("/Script/Engine.Material'/Game/Materials/MAT_TextBilboard.MAT_TextBilboard'"));
	if (TextMatAsset.Succeeded())
	{
		NameTextRenderComp->SetMaterial(0, TextMatAsset.Object);
		NameTextRenderComp->SetRelativeLocation(FVector(0, 0, 20));
		NameTextRenderComp->SetVisibility(false);
	}

	// * 체력바
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Widget/Enemy/WB_EnemyHealth.WB_EnemyHealth_C'"));
	if (HealthWidgetAsset.Succeeded())
	{
		HealthWidgetComp->SetWidgetClass(HealthWidgetAsset.Class);
		HealthWidgetComp->SetupAttachment(InfoPosComp);
		HealthWidgetComp->SetDrawSize(FVector2D(100, 15));
		HealthWidgetComp->SetVisibility(false);
	}

	// * 공격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AttackFXAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
	if (AttackFXAsset.Succeeded())
	{
		AttackFX = AttackFXAsset.Object;
	}

	// * 죽음 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathFXAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/CirclingWolves/FX/PS_CirclingWolfExplosion.PS_CirclingWolfExplosion'"));
	if (DeathFXAsset.Succeeded())
	{
		DeathFX = DeathFXAsset.Object;
	}
	// * 사운드
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/SoundFX/Hit/BluntImpact6-Free-1.BluntImpact6-Free-1'"));
	if (SoundAsset.Succeeded())
	{
		HitSound = SoundAsset.Object;
	}
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName(TEXT("Enemy")));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (UEnemyHealthWidget* HealthWidget = Cast<UEnemyHealthWidget>(HealthWidgetComp->GetUserWidgetObject()))
	{
		OnHealthEvent.BindUObject(HealthWidget, &UEnemyHealthWidget::SetHealth);
	}
	ShowHealthBar(false);
	StartLocation = GetActorLocation();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEnemy::Destroyed()
{
	if (OnHealthEvent.IsBound())
	{
		OnHealthEvent.Unbind();
	}
	if (HealthBarTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthBarTimer);
	}
	if (DeathTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
	}
	if (DamageTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
	}
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (isDamage) return Damage;
	CurrentHp -= Damage;
	isDamage = true;
	UGameplayStatics::SpawnSound2D(GetWorld(), HitSound);
	/*FString Message = FString::Printf(TEXT("Damage : %f / CurrnetHp : %f"), Damage, CurrentHp);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);*/
	if (IsValid(DamageCauser))
	{
		FRotator CauserToRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation());
		SetActorRotation(FRotator(0, CauserToRotation.Yaw, 0));
	}
	ShowHealthBar(true);

	GetWorld()->GetTimerManager().SetTimer(HealthBarTimer, FTimerDelegate::CreateLambda([&]() {ShowHealthBar(false); }), 5.f, false);

	if (OnHealthEvent.IsBound())
	{
		OnHealthEvent.Execute(CurrentHp, EnemyData.MaxHp);
	}
	if (CurrentHp <= 0)
	{
		GetCharacterMovement()->StopMovementImmediately(); //이동중지
		isDeath = true;
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			Tags.Remove(FName(TEXT("Enemy")));
			ShowHealthBar(false);
			PlayAnimMontage(DeathMontage);
			SetActorEnableCollision(false);
		}

	}
	GetWorld()->GetTimerManager().SetTimer(DamageTimer, FTimerDelegate::CreateLambda([&]()
		{
			isDamage = false;
		}
	), 0.1f, false);
	return Damage;
}

void AEnemy::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	PlayAnimMontage(StandUpMontage);
	if (isDeath)
	{
		Tags.Remove(FName(TEXT("Enemy")));
		ShowHealthBar(false);
		PlayAnimMontage(DeathMontage);
		SetActorEnableCollision(false);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateWidgetToCamera();
	if (isDeath)
	{
		FRotator CurrentRot = GetActorRotation();
		SetActorRotation(FRotator(0, CurrentRot.Yaw, 0));
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetTopInfomationLocation(FVector NewLocation)
{
	NameTextRenderComp->SetRelativeLocation(NewLocation);
	TargetEffectComp->SetRelativeLocation(NewLocation + FVector(0, 0, 30));
}

void AEnemy::RotateWidgetToCamera()
{
	if (!IsValid(CameraManager))
	{
		CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	}
	FVector CameraPos = CameraManager->GetCameraLocation();
	FVector WidgetPos = HealthWidgetComp->GetComponentLocation();
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(WidgetPos, CameraPos);
	HealthWidgetComp->SetWorldRotation(Rotator);
	NameTextRenderComp->SetWorldRotation(FRotator::ZeroRotator);
}

void AEnemy::ShowHealthBar(bool isShow)
{
	NameTextRenderComp->SetVisibility(isShow);
	HealthWidgetComp->SetVisibility(isShow);
}

void AEnemy::Death()
{
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathFX, GetMesh()->GetSocketLocation(FName(TEXT("spine_01"))));
	GetMesh()->SetCastShadow(false);
	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);
	SpawnItem();

	GetWorld()->GetTimerManager().SetTimer(DeathTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetActorRotation(FRotator::ZeroRotator);
			GetMesh()->SetCastShadow(true);
			GetMesh()->SetVisibility(true);
			GetMesh()->SetHiddenInGame(false);
			SetActorEnableCollision(true);
			SetActorLocation(StartLocation);
			GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
			CurrentHp = EnemyData.MaxHp;
			Tags.Add(FName(TEXT("Enemy")));
			isDeath = false;
			AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
			EnemyController->GetBlackboardComponent()->SetValueAsVector(Key::Blackboard::PatrolPos, GetActorLocation());
		}
	), 30.f, false);
}

void AEnemy::SpawnItem()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());
	AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(AItem::StaticClass(), Transform);
	
	int Random = FMath::RandRange(0, Key::ItemName::ItemName.Num() - 1);
	Item->SetItemRowName(Key::ItemName::ItemName[Random]);
	Item->FinishSpawning(Transform);
}
