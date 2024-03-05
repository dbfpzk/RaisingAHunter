// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Skill/Blade.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy/Boss/Meteor.h"
#include "Enemy/Boss/Missile.h"

// Sets default values
ABlade::ABlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Particle->SetupAttachment(Root);
	Sphere->SetupAttachment(Particle);
	Sphere->InitSphereRadius(75.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABlade::OnOverlapBegin);

	// * 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/DaggerThrow/FX/P_Kallari_Dagger_Projectile.P_Kallari_Dagger_Projectile'"));
	if (ParticleAsset.Succeeded())
	{
		Particle->SetTemplate(ParticleAsset.Object);
	}

	// * 피격 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/DaggerThrow/FX/P_Kallari_DaggerThrow_HitPlayer.P_Kallari_DaggerThrow_HitPlayer'"));
	if (HitParticleAsset.Succeeded())
	{
		HitParticle = HitParticleAsset.Object;
	}

	ProjectileMovement->InitialSpeed = 1000;
	ProjectileMovement->MaxSpeed = 1000;
	ProjectileMovement->ProjectileGravityScale = 1.f;
}

// Called when the game starts or when spawned
void ABlade::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.f);
}

// Called every frame
void ABlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlade::Init(const FVector& InVelocity)
{
	ProjectileMovement->SetVelocityInLocalSpace(InVelocity);
}

void ABlade::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMeteor::StaticClass())) return;
	if (OtherActor->IsA(AMissile::StaticClass())) return;
	if (OtherActor->IsA(ABlade::StaticClass())) return;
	if (OtherActor->ActorHasTag(FName(TEXT("Player")))) return;
	FTransform Transform;

	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(SweepResult.ImpactNormal.Rotation().Quaternion());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Transform);
	if (OtherActor->ActorHasTag(FName(TEXT("Enemy"))))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 5.f, nullptr, GetOwner(), UDamageType::StaticClass());
	}
	Destroy();
}

