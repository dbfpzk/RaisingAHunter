// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Boss/Boss.h"
#include "Enemy/Boss/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/HunterGameInstance.h"
#include "Components/TextRenderComponent.h"
#include "Utile/Keys.h"
#include "Enemy/Boss/Meteor.h"
#include "NavigationSystem.h"
#include "Enemy/Boss/Missile.h"

#define METEOR_COUNT 20 //메테오 갯수
#define MISSILE_COUNT 12 //베지어미사일 갯수

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;
	// * Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRiktor/Characters/Heroes/Riktor/Meshes/Riktor.Riktor'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	// * 공격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> AttackFXAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
	if (AttackFXAsset.Succeeded())
	{
		AttackFX = AttackFXAsset.Object;
	}
	
	// * 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Enemy/Boss/ABP_Boss.ABP_Boss_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	// * 공격몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Boss/MTG_BossAttack.MTG_BossAttack'"));
	if (AttackMtgAsset.Succeeded())
	{
		AttackMontage = AttackMtgAsset.Object;
	}

	// * 메테오 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeteoMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Boss/MTG_BossMeteo.MTG_BossMeteo'"));
	if (MeteoMtgAsset.Succeeded())
	{
		MeteoMontage = MeteoMtgAsset.Object;
	}

	// * 죽음 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMtgAsset(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Enemy/Boss/MTG_Death.MTG_Death'"));
	if (DeathMtgAsset.Succeeded())
	{
		DeathMontage = DeathMtgAsset.Object;
	}

	InfoPosComp->SetRelativeLocation(FVector(0, 0, 160));
	AIControllerClass = ABossAIController::StaticClass();
	MeteorIndex = 19;
	MissileIndex = 11;

	for (int i = 0; i < METEOR_COUNT; i++)
	{
		FTimerHandle NewTimerHandle;
		MeteorTimers.Emplace(NewTimerHandle);
	}

	for (int i = 0; i < MISSILE_COUNT; i++)
	{
		FTimerHandle NewTimerHandle;
		MissileTimers.Emplace(NewTimerHandle);
	}
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
	UHunterGameInstance* GameInstance = Cast<UHunterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GameInstance))
	{
		EnemyData = *GameInstance->GetEnemyDataTable(Key::EnemyName::Riktor);
	}
	NameTextRenderComp->SetText(FText::FromString(EnemyData.Name));
	CurrentHp = EnemyData.MaxHp;
	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
}

void ABoss::Destroyed()
{
	Super::Destroyed();
	for (int i = 0; i < METEOR_COUNT; i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(MeteorTimers[i]);
	}
	for (int i = 0; i < MISSILE_COUNT; i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(MissileTimers[i]);
	}
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::Attack()
{
	PlayAnimMontage(AttackMontage, EnemyData.AttackSpeed);
}

void ABoss::SendDamage()
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

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 150.f, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackFX, HitResult.Location, HitResult.Normal.Rotation());
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), EnemyData.Damage, GetController(), this, UDamageType::StaticClass());
			}
		}
	}
}

void ABoss::Death()
{
	Super::Death();
}

void ABoss::SpawnMeteo()
{
	if (MeteorIndex != 19) return;
	PlayAnimMontage(MeteoMontage);
	Meteors.Empty();
	MeteorIndex = 0;
	FTransform Transform;
	FNavLocation NavPos;
	float DelayTime = 0.5f;
	for (int i = 0; i < METEOR_COUNT; i++)
	{
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 2000, NavPos))
		{
			Transform.SetLocation(NavPos.Location + FVector(0, 0, 2000));
			AMeteor* Meteor = GetWorld()->SpawnActorDeferred<AMeteor>(AMeteor::StaticClass(), Transform, this);
			Meteors.Add(Meteor);

			Meteor->ArrivalLoc = NavPos.Location;
		}
		DelayTime += 0.1f;
		GetWorld()->GetTimerManager().SetTimer(MeteorTimers[i], this, &ABoss::FinishMeteorSpawn, DelayTime, false);
	}
}

void ABoss::FinishMeteorSpawn()
{
	FTransform Transform;
	Transform.SetLocation(GetActorLocation() + FVector(0, 0, 2000));
	Meteors[MeteorIndex]->FinishSpawning(Transform);
	MeteorIndex++;
	GetWorld()->GetTimerManager().ClearTimer(MeteorTimers[MeteorIndex]);
}

void ABoss::SpawnMissile()
{
	if (MissileIndex != 11) return;
	
		PlayAnimMontage(MeteoMontage);
		MissileIndex = 0;
		Missiles.Empty();
		FTransform Transform;
		Transform.SetLocation(GetActorLocation());
		Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		float DelayTime = 0.1f;
		for (int i = 0; i < MISSILE_COUNT; i++)
		{
			DelayTime += 0.1f;
			AMissile* Missile = GetWorld()->SpawnActorDeferred<AMissile>(AMissile::StaticClass(), Transform, this);
			Missiles.Add(Missile);
			Missile->Init(GetActorTransform(), Target->GetActorTransform(), Speed, DistanceFromStart, DistanceFromEnd);
			GetWorld()->GetTimerManager().SetTimer(MissileTimers[i], this, &ABoss::FinishMissileSpawn, DelayTime, false);
		}
}

void ABoss::FinishMissileSpawn()
{
	/*FString Message = FString::Printf(TEXT("MissileIndex : %d"), MissileIndex);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);*/
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());
	Missiles[MissileIndex]->FinishSpawning(Transform);
	MissileIndex++;
	GetWorld()->GetTimerManager().ClearTimer(MissileTimers[MissileIndex]);
}
