// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranger/Ranger.h"
#include "Enemy/Ranger/RangerAIController.h"
#include "GameMode/HunterGameInstance.h"
#include "Utile/Keys.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
ARanger::ARanger()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	}
	// * 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Enemy/Minion/Ranger/ABP_Ranger.ABP_Ranger_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	// * 공격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AttackFXAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
	if (AttackFXAsset.Succeeded())
	{
		AttackFX = AttackFXAsset.Object;
	}
	// * 공격몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Ranger/MTG_RangerAttack.MTG_RangerAttack'"));
	if (AttackMtgAsset.Succeeded())
	{
		AttackMontage = AttackMtgAsset.Object;
	}
	// * 일어나는 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandUpMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Ranger/MTG_StandUp.MTG_StandUp'"));
	if (StandUpMtgAsset.Succeeded())
	{
		StandUpMontage = StandUpMtgAsset.Object;
	}
	// * 죽음 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Ranger/MTG_Death.MTG_Death'"));
	if (DeathMtgAsset.Succeeded())
	{
		DeathMontage = DeathMtgAsset.Object;
	}



	AIControllerClass = ARangerAIController::StaticClass();
}

// Called when the game starts or when spawned
void ARanger::BeginPlay()
{
	Super::BeginPlay();
	UHunterGameInstance* GameInstance = Cast<UHunterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		EnemyData = *GameInstance->GetEnemyDataTable(Key::EnemyName::Ranger);
	}
	NameTextRenderComp->SetText(FText::FromString(EnemyData.Name));
	CurrentHp = EnemyData.MaxHp;

}

// Called every frame
void ARanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARanger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARanger::Attack()
{
	PlayAnimMontage(AttackMontage, EnemyData.AttackSpeed);
}

void ARanger::SendDamage()
{
	ARangerAIController* RangerController = Cast<ARangerAIController>(GetController());
	if (RangerController == nullptr) return;
	AActor* Target = Cast<AActor>(RangerController->GetBlackboardComponent()->GetValueAsObject(Key::Blackboard::Target));
	if (Target == nullptr) return;

	FVector Start, End, Direction;
	Start = GetMesh()->GetSocketLocation(FName(TEXT("Muzzle_01")));
	Direction = (Target->GetActorLocation() - Start).GetSafeNormal();
	End = (Direction * 800) + Start;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult HitResult;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackFX, HitResult.Location, HitResult.Normal.Rotation());
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), EnemyData.Damage, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void ARanger::Death()
{
	Super::Death();
}