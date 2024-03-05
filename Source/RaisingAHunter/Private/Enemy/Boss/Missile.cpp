// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/Missile.h"
//#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Boss/Meteor.h"
#include "Hunter/Skill/Blade.h"
#include "Sound/SoundWave.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	RootComponent = Root;

	Particle->SetupAttachment(Root);
	Sphere->SetupAttachment(Particle);
	Sphere->InitSphereRadius(75.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnOverlapBegin);

	// * �̻��� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_thunderBall.P_ky_thunderBall'"));
	if (ParticleAsset.Succeeded())
	{
		Particle->SetTemplate(ParticleAsset.Object);
	}

	// * �ǰ� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_ThunderBallHit.P_ky_ThunderBallHit'"));
	if (HitParticleAsset.Succeeded())
	{
		HitParticle = HitParticleAsset.Object;
	}

	// * ����
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Sound/SoundFX/Boss/cannon_01.cannon_01'"));
	if (SoundAsset.Succeeded())
	{
		DestroySound = SoundAsset.Object;
	}

	Points.Init(FVector::Zero(), 4);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
}

void AMissile::Destroyed()
{
	Super::Destroyed();
	UGameplayStatics::SpawnSound2D(GetWorld(), DestroySound);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimerCurrent > TimerMax)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
		Destroy();
		return;
	}
	TimerCurrent += DeltaTime * Speed;

	FVector BezierCurveLocation = FVector(
		CubicBezierCurve(Points[0].X, Points[1].X, Points[2].X, Points[3].X),
		CubicBezierCurve(Points[0].Y, Points[1].Y, Points[2].Y, Points[3].Y),
		CubicBezierCurve(Points[0].Z, Points[1].Z, Points[2].Z, Points[3].Z)
	);
	/*FString Message = FString::Printf(TEXT("BezierCurveLocation : (%f, %f, %f)"), BezierCurveLocation.X, BezierCurveLocation.Y, BezierCurveLocation.Z);
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, Message);*/
	SetActorLocation(BezierCurveLocation);
}

void AMissile::Init(FTransform InStartTr, FTransform InEndTr, float InSpeed, float InNewPointDistanceFromStartTr, float InNewPointDistanceFromEndTr)
{
	Speed = InSpeed;
	TimerMax = FMath::RandRange(1.5f, 2.f); //���������� ����
	Points[0] = InStartTr.GetLocation(); //��������

	// ���� ������ �������� ���� ����Ʈ ����.
	Points[1] = InStartTr.GetLocation() +
		(InNewPointDistanceFromStartTr * FMath::RandRange(-1.0f, 1.0f) * InStartTr.GetRotation().GetRightVector()) + // X (��, �� ��ü)
		(InNewPointDistanceFromStartTr * FMath::RandRange(-0.15f, 1.0f) * InStartTr.GetRotation().GetUpVector()) + // Y (�Ʒ��� ����, ���� ��ü)
		(InNewPointDistanceFromStartTr * FMath::RandRange(-1.0f, -.8f) * InStartTr.GetRotation().GetForwardVector()); // Z (�� �ʸ�)

	// ���� ������ �������� ���� ����Ʈ ����.
	Points[2] = InEndTr.GetLocation() +
		(InNewPointDistanceFromEndTr * FMath::RandRange(-1.0f, 1.0f) * InEndTr.GetRotation().GetRightVector()) + // X (��, �� ��ü)
		(InNewPointDistanceFromEndTr * FMath::RandRange(-1.0f, 1.0f) * InEndTr.GetRotation().GetUpVector()) + // Y (��, �Ʒ� ��ü)
		(InNewPointDistanceFromEndTr * FMath::RandRange(.8f, 1.0f) * InEndTr.GetRotation().GetForwardVector()); // Z (�� �ʸ�)

	// ���� ����.
	Points[3] = InEndTr.GetLocation();

	SetActorLocation(InStartTr.GetLocation());

}

void AMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMissile::StaticClass())) return;
	if (OtherActor->IsA(AMeteor::StaticClass())) return;
	if (OtherActor->IsA(ABlade::StaticClass())) return;
	if (OtherActor->ActorHasTag(FName(TEXT("Enemy")))) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 1, nullptr, GetOwner(), UDamageType::StaticClass());
	}
	
	Destroy();
}

/// <summary>
/// 3�� ������ �
/// </summary>
/// <param name="a">������ġ</param>
/// <param name="b">������ġ���� �󸶳� ������ ���ϴ� ��ġ</param>
/// <param name="c">������ġ���� �󸶳� ������ ���ϴ� ��ġ</param>
/// <param name="d">������ġ</param>
/// <returns></returns>
float AMissile::CubicBezierCurve(float a, float b, float c, float d)
{
	//(0~1)�� ���� ���� ������ ����� ���ϹǷ� ������ ���� �ð��� ����
	float t = TimerCurrent / TimerMax; //����ð�/�ִ�ð�

	/*
	// * ������
	return Mathf.Pow((1 - t), 3) * a
		+ Mathf.Pow((1 - t), 2) * 3 * t * b
		+ Mathf.Pow(t, 2) * 3 * (1 - t) * c
		+ Mathf.Pow(t, 3) * d;
	*/
	float ab = FMath::Lerp(a, b, t);
	float bc = FMath::Lerp(b, c, t);
	float cd = FMath::Lerp(c, d, t);

	float abbc = FMath::Lerp(ab, bc, t);
	float bccd = FMath::Lerp(bc, cd, t);

	return FMath::Lerp(abbc, bccd, t);
}

