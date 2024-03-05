// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyAIController.h"
#include "MuruAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API AMuruAIController : public AEnemyAIController
{
	GENERATED_BODY()
public:
	AMuruAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
