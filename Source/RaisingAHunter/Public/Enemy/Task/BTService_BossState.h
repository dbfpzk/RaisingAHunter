// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BossState.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UBTService_BossState : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_BossState();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
