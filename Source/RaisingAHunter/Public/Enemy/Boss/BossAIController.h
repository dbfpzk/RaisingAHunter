// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API ABossAIController : public AEnemyAIController
{
	GENERATED_BODY()
public:
	ABossAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
