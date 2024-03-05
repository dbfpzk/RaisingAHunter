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
	SightConfig->SightRadius = 2000.f;	//�� �� �ִ� ����
	SightConfig->LoseSightRadius = 2200.f;	//�̹� �� ��ǥ���� �� �� �ִ� ����
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

// * ���� ����
//1. ������ ��������
//2. �ָ� �̻��� �߻�
//3. ���̻��� �߻�
//4. ���׷� ������