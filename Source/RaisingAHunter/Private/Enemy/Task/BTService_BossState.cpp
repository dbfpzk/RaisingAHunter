// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTService_BossState.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utile/Keys.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Utile/Enums.h"

UBTService_BossState::UBTService_BossState()
{
	NodeName = TEXT("BossState");
}


void UBTService_BossState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Key::Blackboard::Target));
	if (!IsValid(Target)) return;
	ACharacter* Enemy = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy)) return;
	if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(Key::Blackboard::BossStateType) == (uint8)EBossStateType::Dash) return;
	float Distance = Enemy->GetDistanceTo(Target);
	if (Distance < 400)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)EBossStateType::Melee);
	}
	else if (Distance < 1700)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)EBossStateType::Missile);

	}
	else if (Distance < 2000)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)EBossStateType::Meteo);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(Key::Blackboard::BossStateType, (uint8)EBossStateType::Idle);
	}
}

