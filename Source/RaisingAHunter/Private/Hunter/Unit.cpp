// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Unit.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemy/Enemy.h"
#include "Hunter/PlayerState/UnitState.h"
#include "RootMotionModifier.h"
#include "GameFramework/GameModeBase.h"
#include "Hunter/AnimInstance/UnitAnimInstance.h"
#include "Hunter/Skill/AfterImage.h"
#include "Utile/Keys.h"
#include "GameMode/HunterGameMode.h"
#include "Widget/Game/OrbWidget.h"
#include "Widget/Game/GameWidget.h"
#include "Widget/Game/SkillSetWidget.h"

// Sets default values
AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	// * SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	// * Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// * Movement
	bUseControllerRotationYaw = true;
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->bOrientRotationToMovement = false;

	// * Controller
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// * Weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	// * MotionWarping
	MotionWarp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarp"));

	// * AirCombo
	TargetAirPos = CreateDefaultSubobject<USceneComponent>(TEXT("TargetAirPos"));
	TargetAirPos->SetupAttachment(GetCapsuleComponent());
	TargetAirPos->SetRelativeLocation(FVector(130, 0, 0));

	// * Tag
	Tags.Add(FName(TEXT("Player")));

	// * Etc
	isMove = true;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	UnitState = Cast<AUnitState>(GetPlayerState());
	UnitAnim = Cast<UUnitAnimInstance>(GetMesh()->GetAnimInstance());
	InitState();
	GetWorld()->GetTimerManager().SetTimer(DamageTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetCurrentHp(GetCurrentHp() + 0.1f);
			SetCurrentMp(GetCurrentMp() + 0.1f);
		}
	), 1.f, true);
	AHunterGameMode* HunterGameMode = Cast<AHunterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(HunterGameMode))
	{
		HunterGameMode->GetGameWidget()->HealthOrb->Init();
		HunterGameMode->GetGameWidget()->ManaOrb->Init();
		HunterGameMode->GetGameWidget()->SkillSet->Init();
	}
}

void AUnit::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUnit::Destroyed()
{
	Super::Destroyed();
	if (OnChangeHpSignature.IsBound())
	{
		OnChangeHpSignature.Clear();
	}
	if (OnChangeMpSignature.IsBound())
	{
		OnChangeMpSignature.Clear();
	}
	if (DamageTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
	}
	if (OnGroundAttackSignature.IsBound())
	{
		OnGroundAttackSignature.Clear();
	}
	if (OnAirAttackSignature.IsBound())
	{
		OnAirAttackSignature.Clear();
	}
	if (OnProjectileAttackSignature.IsBound())
	{
		OnProjectileAttackSignature.Clear();
	}
}

void AUnit::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	SetUnitStateType(EUnitStateType::Idle);
	isAirAttack = false;
	if (AirTarget.Num() != 0)
	{
		AirTarget.Empty();
	}
}

float AUnit::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (isDamage) return Damage;
	SetCurrentHp(GetCurrentHp() - Damage);
	isDamage = true;
	if (GetCurrentHp() <= 0.0f)
	{
		UnitStateType = EUnitStateType::Die;
		GetCharacterMovement()->StopMovementImmediately(); //이동중지
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); //충돌 중지
		Tags.Remove(FName("Player"));
		DetachFromControllerPendingDestroy(); //컨트롤러와 캐릭터를 분리시킨다.

		GetWorldTimerManager().SetTimer(RespawnHandle, this, &AUnit::Respawn, 3.0f, true);
		SetLifeSpan(10.f);
		PlayAnimMontage(UnitAnim->GetDeathMontage());
	}
	GetWorld()->GetTimerManager().SetTimer(DamageTimer, FTimerDelegate::CreateLambda([&]()
		{
			isDamage = false;
		}
	), 0.1f, false);
	return Damage;
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isAirAttack && SpringArm->TargetArmLength < 800)
	{
		SpringArm->TargetArmLength = SpringArm->TargetArmLength + 30;
	}
	else if(!isAirAttack && SpringArm->TargetArmLength > 400)
	{
		SpringArm->TargetArmLength = SpringArm->TargetArmLength - 30;
	}

}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnit::Zoom(float Axis)
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - (Axis * 10), 200.f, 1200.f);
}

void AUnit::Dash(FVector2D& Movement)
{
	SpawnAfterImage();
}

void AUnit::Attack()
{
}

void AUnit::NextAttack()
{
}

void AUnit::EndAttack()
{
}

void AUnit::SendDamage(float AdditionalDamage, float Radius, float AttackRange)
{
	FVector Start, End, Direction;
	Start = GetActorLocation();
	Direction = GetActorForwardVector();
	End = Start + (Direction * AttackRange);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	TArray<FHitResult> HitResults;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, Radius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Enemy"))))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, HitResult.Normal.Rotation() + FRotator(0, 90, 0), FVector(2,2,2));

				//TODO : 데미지 전달
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), UnitState->GetDamage() + AdditionalDamage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}

void AUnit::UpperSlash()
{
}

void AUnit::UpperAttack()
{
	SpawnAfterImage();
	FVector Start, End, Direction;
	Start = GetActorLocation();
	Direction = GetActorForwardVector();
	End = Start + (Direction * 100);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	TArray<FHitResult> HitResults;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 150, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AEnemy* Enemy = Cast<AEnemy>(HitResult.GetActor());
			if (IsValid(Enemy))
			{
				//TODO : 데미지 전달
				//UGameplayStatics::ApplyDamage(HitResult.GetActor(), UnitState->GetDamage() + UnitState->GetDamage(), GetController(), this, UDamageType::StaticClass());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint, HitResult.Normal.Rotation());

				//FString Message = FString::Printf(TEXT("Damage : %f"), UnitState->GetDamage());
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Message);
				AirTarget.Add(Enemy);
				Enemy->LaunchCharacter(FVector(0, 0, 1200), true, true);
			}
		}
	}
}

void AUnit::FallingDown()
{
}

void AUnit::SpawnAfterImage()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	AAfterImage* AfterImage = GetWorld()->SpawnActor<AAfterImage>(AAfterImage::StaticClass(), SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParameters);
	AfterImage->SetPoseableMeshPos(GetMesh());
}

void AUnit::InitState()
{
	MaxHp = UnitState->GetMaxHp();
	SetCurrentHp(MaxHp);
	MaxMp = UnitState->GetMaxMp();
	SetCurrentMp(MaxMp);
}

void AUnit::Death()
{
	GetMesh()->SetCastShadow(false);
	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);
	Weapon->SetVisibility(false);
	Weapon->SetHiddenInGame(true);
}

void AUnit::Blade()
{
}

void AUnit::SpawnBlade()
{
}

FHitResult AUnit::AttachToSurfaceCalculation()
{
	float AttachDistance = 100.f;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + (AttachDistance * GetActorForwardVector());
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

	return HitResult;
}

void AUnit::ClimbMovement(FVector WorldDirection, float ScaleValue)
{
	FHitResult HitResult = AttachToSurfaceCalculation();
	if (HitResult.bBlockingHit)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
		AddMovementInput(WorldDirection, ScaleValue);
		LedgeMantleCalculation();
		FRotator Rot = UKismetMathLibrary::MakeRotFromX(HitResult.Normal * -1.f);
		SetActorRotation(Rot);
	}
	else
	{
		StopClimbing();
	}
}

void AUnit::LedgeMantleCalculation()
{
	
}

void AUnit::StopClimbing()
{
	CharacterMovementComp->SetMovementMode(EMovementMode::MOVE_Falling);
	bUseControllerRotationYaw = true;
	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AUnit::Invincibility()
{
	MaxHp = 99999.f;
	SetCurrentHp(99999.f);
	MaxMp = 99999.f;
	SetCurrentMp(99999.f);
}

void AUnit::Respawn()
{
	UnitStateType = EUnitStateType::Die;
	GetWorldTimerManager().ClearTimer(RespawnHandle);
	GetWorld()->GetAuthGameMode()->RestartPlayer(GetWorld()->GetFirstPlayerController());
	this->Destroy();
}
