// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UBTTask_GetPatrolPos : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_GetPatrolPos();
protected:
	EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 300.0f;
};
