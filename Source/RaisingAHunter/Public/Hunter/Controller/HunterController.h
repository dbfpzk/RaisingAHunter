// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hunter/Controller/UnitController.h"
#include "HunterController.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API AHunterController : public AUnitController
{
	GENERATED_BODY()
public:
	AHunterController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
};
