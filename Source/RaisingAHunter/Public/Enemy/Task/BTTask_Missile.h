// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Missile.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UBTTask_Missile : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Missile();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
