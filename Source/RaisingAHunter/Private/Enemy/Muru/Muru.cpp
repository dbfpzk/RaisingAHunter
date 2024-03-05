// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Muru/Muru.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/HunterGameInstance.h"
#include "Components/TextRenderComponent.h"
#include "Utile/Keys.h"
#include "Enemy/Muru/MuruAIController.h"
// Sets default values
AMuru::AMuru()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// * Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	// * 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Enemy/Minion/Muru/ABP_Muru.ABP_Muru_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	// * 공격몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Muru/MTG_MuruAttack.MTG_MuruAttack'"));
	if (AttackMtgAsset.Succeeded())
	{
		AttackMontage = AttackMtgAsset.Object;
	}

	// * 일어나는 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StandUpMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Muru/MTG_MuruStandUp.MTG_MuruStandUp'"));
	if (StandUpMtgAsset.Succeeded())
	{
		StandUpMontage = StandUpMtgAsset.Object;
	}
	// * 죽음 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Minion/Muru/MTG_MuruDeath.MTG_MuruDeath'"));
	if (DeathMtgAsset.Succeeded())
	{
		DeathMontage = DeathMtgAsset.Object;
	}
	// * 공격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AttackFXAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
	if (AttackFXAsset.Succeeded())
	{
		AttackFX = AttackFXAsset.Object;
	}
	AIControllerClass = AMuruAIController::StaticClass();
}

// Called when the game starts or when spawned
void AMuru::BeginPlay()
{
	Super::BeginPlay();
	UHunterGameInstance* GameInstance = Cast<UHunterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		EnemyData = *GameInstance->GetEnemyDataTable(Key::EnemyName::Muru);
	}
	NameTextRenderComp->SetText(FText::FromString(EnemyData.Name));
	CurrentHp = EnemyData.MaxHp;
}

// Called every frame
void AMuru::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMuru::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMuru::Attack()
{
	PlayAnimMontage(AttackMontage, EnemyData.AttackSpeed);
}

void AMuru::SendDamage()
{
	FVector Start, End, Direction;
	Start = GetActorLocation();
	Direction = GetActorForwardVector();
	End = Start + (Direction * 150.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	TArray<FHitResult> HitResults;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 50.f, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true))
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackFX, HitResult.Location, HitResult.Normal.Rotation());
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), EnemyData.Damage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}

void AMuru::Death()
{
	Super::Death();
}

