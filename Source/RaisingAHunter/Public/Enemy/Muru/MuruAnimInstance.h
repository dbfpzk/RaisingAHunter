// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAnimInstance.h"
#include "MuruAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UMuruAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
protected:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class AMuru> Muru;
protected:
		UFUNCTION()
	void AnimNotify_Death();
		UFUNCTION()
		void AnimNotify_SendDamage();
};
