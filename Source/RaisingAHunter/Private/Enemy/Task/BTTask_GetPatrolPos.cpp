// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTTask_GetPatrolPos.h"

#include "Utile/Keys.h"
#include "Enemy/EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetPatrolPos::UBTTask_GetPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_GetPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(EnemyController)) return EBTNodeResult::Failed;
	APawn* Enemy = EnemyController->GetPawn();
	if (!IsValid(Enemy)) return EBTNodeResult::Failed;
	FVector OriginPos = Enemy->GetActorLocation();
	FNavLocation NavPos;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys->GetRandomPointInNavigableRadius(OriginPos, SearchRadius, NavPos))
	{
		EnemyController->GetBlackboardComponent()->SetValueAsVector(Key::Blackboard::PatrolPos, NavPos);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
