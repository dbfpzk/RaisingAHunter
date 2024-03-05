// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UUnitAnimInstance();
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool isAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool isClimbing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float LeftAndRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float UpAndDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class AUnit> Unit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parkour", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> MantleLedgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> GroundAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> UpperSlashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> AirAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> BladeMontage;
protected:
	UFUNCTION()
	void AnimNotify_EndDash();
	UFUNCTION()
	void AnimNotify_EndLedge();
	UFUNCTION()
	void AnimNotify_NextAttack();
	UFUNCTION()
	void AnimNotify_EndAttack();
	UFUNCTION()
	void AnimNotify_UpperAttack();
	UFUNCTION()
	void AnimNotify_FallingDown();
	UFUNCTION()
	void AnimNotify_Death();
	UFUNCTION()
	void AnimNotify_Blade();
public:
	FORCEINLINE class UAnimMontage* GetDashMontage()
	{
		return DashMontage;
	}
	FORCEINLINE class UAnimMontage* GetMantleLedgeMontage()
	{
		return MantleLedgeMontage;
	}
	FORCEINLINE class UAnimMontage* GetGroundAttackMontage()
	{
		return GroundAttackMontage;
	}
	FORCEINLINE class UAnimMontage* GetUpperSlashMontage()
	{
		return UpperSlashMontage;
	}
	FORCEINLINE class UAnimMontage* GetAirAttackMontage()
	{
		return AirAttackMontage;
	}
	FORCEINLINE class UAnimMontage* GetDeathMontage()
	{
		return DeathMontage;
	}
	FORCEINLINE class UAnimMontage* GetBladeMontage()
	{
		return BladeMontage;
	}
};
