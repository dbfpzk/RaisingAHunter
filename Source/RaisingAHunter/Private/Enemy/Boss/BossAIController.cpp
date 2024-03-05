// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Enemy/Boss/BT_Boss.BT_Boss'"));
	if (BTAsset.Succeeded())
	{
		BehaviorTree = BTAsset.Object;
	}
	SightConfig->SightRadius = 2000.f;	//볼 수 있는 범위
	SightConfig->LoseSightRadius = 2200.f;	//이미 본 목표물을 볼 수 있는 범위
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTree == nullptr) return;
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComp->StartTree(*BehaviorTree);
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}

// * 보스 패턴
//1. 가까우면 근접공격
//2. 멀면 미사일 발사
//3. 곡사미사일 발사
//4. 메테로 떨구기