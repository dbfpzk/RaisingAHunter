// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UBossAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class ABoss> Boss;
protected:

	UFUNCTION()
	void AnimNotify_SendDamage();
	UFUNCTION()
	void AnimNotify_Death();
};
