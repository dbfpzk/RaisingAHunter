// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/Meteor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy/Boss/Boss.h"
#include "Enemy/Boss/Missile.h"
#include "Hunter/Skill/Blade.h"
#include "Sound/SoundWave.h"

// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Particle->SetupAttachment(Root);
	Sphere->SetupAttachment(Particle);
	Sphere->InitSphereRadius(75.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnOverlapBegin);

	// * 운석 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/ProjectileMeteor/FX/P_Gideon_RMB_Proj.P_Gideon_RMB_Proj'"));
	if (ParticleAsset.Succeeded())
	{
		Particle->SetTemplate(ParticleAsset.Object);
		Particle->SetRelativeRotation(FRotator(-90, 0, 0));
	}

	// * 피격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/ProjectileMeteor/FX/P_Gideon_RMB_HitWorld.P_Gideon_RMB_HitWorld'"));
	if (HitParticleAsset.Succeeded())
	{
		HitParticle = HitParticleAsset.Object;
	}

	// * 포탈 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PortalParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Portal.P_Gideon_Meteor_Portal'"));
	if (PortalParticleAsset.Succeeded())
	{
		PortalParticle = PortalParticleAsset.Object;
	}
	// * 생성 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpawnParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/ProjectileMeteor/FX/P_Gideon_RMB_Portal.P_Gideon_RMB_Portal'"));
	if (PortalParticleAsset.Succeeded())
	{
		SpawnParticle = SpawnParticleAsset.Object;
	}
	// * 사운드
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/SoundFX/Player/Attack/Grenade7Short.Grenade7Short'"));
	if (SoundAsset.Succeeded())
	{
		DestroySound = SoundAsset.Object;
	}
	
	ProjectileMovement->InitialSpeed = 1000;
	ProjectileMovement->MaxSpeed = 1000;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	//ProjectileMovement->SetVelocityInLocalSpace()
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeteor::Init()
{
	FTransform Transform;
	Transform.SetLocation(ArrivalLoc);
	SetActorLocation(ArrivalLoc + FVector(0, 0, 2000));
	Transform.SetScale3D(FVector(0.5f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalParticle, Transform);
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(FRotator(-90, 0, 0).Quaternion());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticle, Transform);
	FVector OutVelocity = FVector::ZeroVector;   // 결과 Velocity

	if (UGameplayStatics::SuggestProjectileVelocity(this, OutVelocity, GetActorLocation(), ArrivalLoc, GetWorld()->GetGravityZ())) // GetWorld()->GetGravityZ()
	{
		//ProjectileMovement->SetVelocityInLocalSpace(OutVelocity);
		ProjectileMovement->Velocity = OutVelocity;
	}
}

void AMeteor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMeteor::StaticClass())) return;
	if (OtherActor->ActorHasTag(FName(TEXT("Enemy")))) return;
	if (OtherActor->IsA(AMissile::StaticClass())) return;
	if (OtherActor->IsA(ABlade::StaticClass())) return;
	FTransform Transform;
	
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(SweepResult.ImpactNormal.Rotation().Quaternion());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Transform);
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 1, nullptr, GetOwner(), UDamageType::StaticClass());
	}
	UGameplayStatics::SpawnSound2D(GetWorld(), DestroySound);
	Destroy();
}


