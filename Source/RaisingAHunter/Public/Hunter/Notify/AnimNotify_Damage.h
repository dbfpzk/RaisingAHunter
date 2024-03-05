// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Damage.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UAnimNotify_Damage : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Damage();
protected:
	FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) final;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float AdditionalDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float Radius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float AttackRange;
};
