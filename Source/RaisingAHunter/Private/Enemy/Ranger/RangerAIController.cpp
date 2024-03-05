// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranger/RangerAIController.h"

#include "Utile/Keys.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ARangerAIController::ARangerAIController()
{
	PrimaryActorTick.bCanEverTick = true; //true�ؾ� ���ʹ� ȸ����
	
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Enemy/Minion/Ranger/BT_Ranger.BT_Ranger'"));
	if (BTAsset.Succeeded())
	{
		BehaviorTree = BTAsset.Object;
	}
	SightConfig->SightRadius = 700.f;	//�� �� �ִ� ����
	SightConfig->LoseSightRadius = 750.f;	//�̹� �� ��ǥ���� �� �� �ִ� ����
}

void ARangerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTree == nullptr)return;
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComp->StartTree(*BehaviorTree);
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}
