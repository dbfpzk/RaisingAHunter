// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAIController.h"
#include "RangerAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API ARangerAIController : public AEnemyAIController
{
	GENERATED_BODY()
public:
	ARangerAIController();
protected:
	virtual void OnPossess(class APawn* InPawn) override;
};
