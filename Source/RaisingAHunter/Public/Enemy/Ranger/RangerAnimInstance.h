// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAnimInstance.h"
#include "RangerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API URangerAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class ARanger> Ranger;
protected:
	UFUNCTION()
	void AnimNotify_SendDamage();
	UFUNCTION()
	void AnimNotify_Death();
};
