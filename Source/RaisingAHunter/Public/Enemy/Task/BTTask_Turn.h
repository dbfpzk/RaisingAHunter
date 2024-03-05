// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Turn.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UBTTask_Turn : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Turn();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
