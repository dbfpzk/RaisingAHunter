// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/AnimInstance/HunterAnimInstance.h"

UHunterAnimInstance::UHunterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DashAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_HunterDash.MTG_HunterDash'"));
	if (DashAsset.Succeeded())
	{
		DashMontage = DashAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MantleLedgeAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_MantleLedge.MTG_MantleLedge'"));
	if (MantleLedgeAsset.Succeeded())
	{
		MantleLedgeMontage = MantleLedgeAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttackAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_GroundAttack.MTG_GroundAttack'"));
	if (GroundAttackAsset.Succeeded())
	{
		GroundAttackMontage = GroundAttackAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UpperSlashAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_UpperSlash.MTG_UpperSlash'"));
	if (GroundAttackAsset.Succeeded())
	{
		UpperSlashMontage = UpperSlashAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirAttackAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_AirAttack.MTG_AirAttack'"));
	if (AirAttackAsset.Succeeded())
	{
		AirAttackMontage = AirAttackAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_HunterDeath.MTG_HunterDeath'"));
	if (DeathAsset.Succeeded())
	{
		DeathMontage = DeathAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BladeMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Hunter/Montage/MTG_Blade.MTG_Blade'"));
	if (BladeMontageAsset.Succeeded())
	{
		BladeMontage = BladeMontageAsset.Object;
	}
}

void UHunterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UHunterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
