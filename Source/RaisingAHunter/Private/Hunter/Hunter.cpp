// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Hunter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utile/Keys.h"
#include "MotionWarpingComponent.h"
#include "Hunter/AnimInstance/HunterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Hunter/Controller/HunterController.h"
#include "Hunter/Skill/Blade.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHunter::AHunter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetUnitType(EUnitType::Hunter);
	// * Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Skins/Tier2/ActionMovie/Meshes/TwinBlast_ActionHero.TwinBlast_ActionHero'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	// * Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Huter/Animation/ABP_Hunter.ABP_Hunter_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimAsset.Class);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	// * Weapon
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	if (WeaponAsset.Succeeded())
	{
		Weapon->SetSkeletalMesh(WeaponAsset.Object);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(Key::Socket::RightHand));
		Weapon->SetRelativeLocation(FVector(-7.5f, 5.f, 0));
		Weapon->SetRelativeRotation(FRotator(0, 50.f, 0));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Niagara/FX_FireHitEffect.FX_FireHitEffect'"));
	if (HitEffectAsset.Succeeded())
	{
		HitEffect = HitEffectAsset.Object;
	}


	CharacterMovementComp->MaxFlySpeed = 350.f;
	//날기상태일때 감속하는 속도
	CharacterMovementComp->BrakingDecelerationFlying = 3000.f;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();
	HunterAnim = Cast<UHunterAnimInstance>(GetMesh()->GetAnimInstance());
	UnitStateType = EUnitStateType::Idle;
	HunterController = Cast<AHunterController>(GetController());
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHunter::Dash(FVector2D& Movement)
{
	Super::Dash(Movement);
	if (GetCharacterMovement()->IsFalling()) return;

	const FVector MovementInputVector = GetLastMovementInputVector();
	/*if (MovementInputVector.IsZero())
	{
		MotionWarp->RemoveWarpTarget(TEXT("Dash"));
		return;
	}*/

	const FVector PlayerLoc = GetActorLocation();
	const FVector TargetLoc = (MovementInputVector.GetSafeNormal() * 250.f) + PlayerLoc;
	const FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(MovementInputVector);

	MotionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Dash"), TargetLoc, TargetRotator);

	if (Movement == FVector2D::Zero() || Movement.Y > 0 && FMath::Abs(Movement.X) <= Movement.Y)
	{
		PlayAnimMontage(HunterAnim->GetDashMontage(), 1, FName(TEXT("DashF")));
	}
	if (Movement.Y < 0 && FMath::Abs(Movement.X) <= -Movement.Y)
	{
		PlayAnimMontage(HunterAnim->GetDashMontage(), 1, FName(TEXT("DashB")));
	}
	if (Movement.X > 0 && FMath::Abs(Movement.Y) < Movement.X)
	{
		PlayAnimMontage(HunterAnim->GetDashMontage(), 1, FName(TEXT("DashR")));
	}
	if (Movement.X < 0 && FMath::Abs(Movement.Y) < -Movement.X)
	{
		PlayAnimMontage(HunterAnim->GetDashMontage(), 1, FName(TEXT("DashL")));
	}
	
}

void AHunter::Attack()
{
	Super::Attack();
	if (GetUnitStateType() == EUnitStateType::Jump) return;
	if (GetCurrentHp() <= 0) return;
	SetUnitStateType(EUnitStateType::Attack);
	if (!isAttack)
	{
		if (!CharacterMovementComp->IsFalling())
		{
			PlayAnimMontage(HunterAnim->GetGroundAttackMontage(), 1.f, FName(TEXT("Attack1")));
			SpawnAfterImage();
			OnGroundAttackSignature.Broadcast();
		}
		else if (isAirAttack)
		{
			PlayAnimMontage(HunterAnim->GetAirAttackMontage(), 1.f, FName(TEXT("Attack1"))); 
			SpawnAfterImage();
			LaunchCharacter(FVector(0, 0, 500), true, true);
			for (auto Target : AirTarget)
			{
				Target->LaunchCharacter(FVector(0, 0, 500), true, true);
			}
			OnAirAttackSignature.Broadcast();
		}
		isAttack = true;
		isMove = false;
		ComboCount++;
	}
	else if (isEnableCombo)
	{

		isNextAttack = true;
	}
}

void AHunter::NextAttack()
{
	Super::NextAttack();
	if (!isNextAttack) return;
	switch (ComboCount)
	{
	case 1:
		if (!CharacterMovementComp->IsFalling())
		{
			PlayAnimMontage(HunterAnim->GetGroundAttackMontage(), 1.f, FName(TEXT("Attack2")));
			SpawnAfterImage();
			OnGroundAttackSignature.Broadcast();
		}
		else if (isAirAttack)
		{
			PlayAnimMontage(HunterAnim->GetAirAttackMontage(), 1.f, FName(TEXT("Attack2")));
			SpawnAfterImage();
			LaunchCharacter(FVector(0, 0, 500) + (GetActorForwardVector() * 1100), true, true);
			for (auto Target : AirTarget)
			{
				Target->LaunchCharacter(FVector(0, 0, 500), true, true);
			}
			OnAirAttackSignature.Broadcast();
		}
		isNextAttack = false;
		ComboCount++;
		break;
	case 2:
		if (!CharacterMovementComp->IsFalling())
		{
			PlayAnimMontage(HunterAnim->GetGroundAttackMontage(), 1.f, FName(TEXT("Attack3")));
			SpawnAfterImage();
			OnGroundAttackSignature.Broadcast();
		}
		else if (isAirAttack)
		{
			PlayAnimMontage(HunterAnim->GetAirAttackMontage(), 1.f, FName(TEXT("Attack3")));
			SpawnAfterImage();
			LaunchCharacter(FVector(0, 0, 500), true, true);
			for (auto Target : AirTarget)
			{
				Target->LaunchCharacter(FVector(0, 0, 500), true, true);
			}
			OnAirAttackSignature.Broadcast();
		}
		isNextAttack = false;
		isMove = false;
		ComboCount = 0;
		break;
	}
}

void AHunter::EndAttack()
{
	Super::EndAttack();
	SetUnitStateType(EUnitStateType::Idle);

	isMove = true;
	isAttack = false;
	isNextAttack = false;
	ComboCount = 0;
}

void AHunter::UpperSlash()
{
	if (GetCurrentMp() <= 0) return;
	if (GetUnitStateType() != EUnitStateType::Idle) return;
	if (CharacterMovementComp->IsFalling() == true) return;
	OnAirAttackSignature.Broadcast();
	SetUnitStateType(EUnitStateType::Jump);
	SetCurrentMp(GetCurrentMp() - 1);
	PlayAnimMontage(HunterAnim->GetUpperSlashMontage(), 1.0f);
}

void AHunter::UpperAttack()
{
	Super::UpperAttack();
	if (AirTarget.Num() != 0)
	{
		LaunchCharacter(FVector(0, 0, 1200), true, true);
		isAirAttack = true;
		return;
	}
	LaunchCharacter(FVector(0, 0, 300), true, true);
}

void AHunter::FallingDown()
{
	Super::FallingDown();
	LaunchCharacter(FVector(0, 0, -9000), true, true);
	if (AirTarget.Num() != 0)
	{
		for (auto Target : AirTarget)
		{
			Target->LaunchCharacter(FVector(0, 0, -9000), true, true);
		}
	}
}

void AHunter::LedgeMantleCalculation()
{
	Super::LedgeMantleCalculation();
	FVector Start = GetActorLocation() + FVector(0, 0, 100.f);
	FVector End = Start + (GetActorForwardVector() * 100.f);

	FHitResult HitResult;
	bool isTrace = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	if (isTrace == true) return;

	for (int i = 0; i <= 60; i += 3)
	{
		FVector Start2 = HitResult.TraceStart + FVector(0, 0, 90.f);
		FVector End2 = ((GetActorForwardVector() * i) + Start) - FVector(0, 0, 100.f);
		FHitResult HitResult2;
		GetWorld()->LineTraceSingleByChannel(HitResult2, Start2, End2, ECollisionChannel::ECC_Visibility);

		FVector WarpPos1(0, 0, 0);
		WarpPos1.X = (GetActorForwardVector().X * 48.f) + GetActorLocation().X;
		WarpPos1.Y = (GetActorForwardVector().Y * 48.f) + GetActorLocation().Y;
		WarpPos1.Z = HitResult2.Location.Z;

		FMotionWarpingTarget Target1 = {};
		Target1.Name = FName("LedgeClimbUp");
		Target1.Location = WarpPos1;
		Target1.Rotation = GetActorRotation();

		MotionWarp->AddOrUpdateWarpTarget(Target1);

		FVector WarpPos2(0, 0, 0);
		WarpPos2.X = HitResult2.Location.X;
		WarpPos2.Y = HitResult2.Location.Y;
		WarpPos2.Z = GetActorLocation().Z + 90.f;

		FMotionWarpingTarget Target2 = {};
		Target2.Name = FName("LedgeClimbForward");
		Target2.Location = WarpPos2;
		Target2.Rotation = GetActorRotation();

		MotionWarp->AddOrUpdateWarpTarget(Target2);

		if (IsValid(HunterAnim))
		{
			PlayAnimMontage(HunterAnim->GetMantleLedgeMontage(), 1.5f);
			isClimb = true;
			SetActorEnableCollision(false);
		}
	}
}

void AHunter::Death()
{
	Super::Death();
}

void AHunter::Blade()
{
	Super::Blade();
	if (!IsValid(HunterController)) return;
	PlayAnimMontage(HunterAnim->GetBladeMontage());
	OnProjectileAttackSignature.Broadcast();
	isMove = false;
}

void AHunter::SpawnBlade()
{
	FVector TargetPos;

	if (IsValid(HunterController->GetLockOnEnemy()))
	{
		TargetPos = HunterController->GetLockOnEnemy()->GetActorLocation();
	}
	else
	{
		FRotator Rotator(0, GetControlRotation().Yaw, 0);
		FVector Forward = Rotator.Quaternion().GetForwardVector();
		TargetPos = GetActorLocation() + (Forward * 1000.f);
	}
	FVector OutVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutVelocity, GetActorLocation(), TargetPos, GetWorld()->GetGravityZ(), 0.5f);
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());

	ABlade* Blade = GetWorld()->SpawnActor<ABlade>(ABlade::StaticClass(), Transform);
	if (IsValid(Blade))
	{
		Blade->Init(OutVelocity);
	}
}

