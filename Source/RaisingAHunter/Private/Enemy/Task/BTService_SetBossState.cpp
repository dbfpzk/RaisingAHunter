// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTService_SetBossState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utile/Keys.h"
#include "AIController.h"
#include "Utile/Enums.h"

UBTService_SetBossState::UBTService_SetBossState()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("SetBossState");
}

void UBTService_SetBossState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)BossState);
}

//void UBTService_SetBossState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
//	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Key::Blackboard::TargetPos);
//	APawn* Boss = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
//	if (FVector::Dist2D(TargetPos, Boss->GetActorLocation()) < 100)
//	{
//		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)EBossStateType::Melee);
//	}
//}

