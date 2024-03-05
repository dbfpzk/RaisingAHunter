// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/AnimInstance/UnitAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Hunter/Unit.h"

UUnitAnimInstance::UUnitAnimInstance()
{
	
}

void UUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Unit = Cast<AUnit>(TryGetPawnOwner());
}

void UUnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(Unit)) return;
	Speed = Unit->GetVelocity().Size2D();
	FRotator Rot = FRotator(0, Unit->GetControlRotation().Yaw, 0);

	Direction = CalculateDirection(Unit->GetVelocity(), Rot);
	isAir = Unit->GetMovementComponent()->IsFalling();
	LeftAndRight = Unit->LeftAndRight * 100.f;
	UpAndDown = Unit->UpAndDown * 100.f;
	isClimbing = (Unit->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying);
}

void UUnitAnimInstance::AnimNotify_EndDash()
{
	if (Unit->GetUnitStateType() == EUnitStateType::Die) return;
	Unit->SetUnitStateType(EUnitStateType::Idle);
	Unit->EndAttack();
}

void UUnitAnimInstance::AnimNotify_EndLedge()
{
	Unit->SetActorEnableCollision(true);
	Unit->StopClimbing();
	Unit->isClimb = false;
}

void UUnitAnimInstance::AnimNotify_NextAttack()
{
	Unit->NextAttack();
}

void UUnitAnimInstance::AnimNotify_EndAttack()
{
	Unit->EndAttack();
}

void UUnitAnimInstance::AnimNotify_UpperAttack()
{
	Unit->UpperAttack();
}

void UUnitAnimInstance::AnimNotify_FallingDown()
{
	Unit->FallingDown();
}

void UUnitAnimInstance::AnimNotify_Death()
{
	Unit->Death();
}

void UUnitAnimInstance::AnimNotify_Blade()
{
	Unit->SpawnBlade();
}
