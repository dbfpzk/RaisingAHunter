// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Muru/MuruAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
AMuruAIController::AMuruAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Enemy/Minion/Muru/BT_Muru.BT_Muru'"));
	if (BTAsset.Succeeded())
	{
		BehaviorTree = BTAsset.Object;
	}
}

void AMuruAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTree == nullptr) return;
	RunBehaviorTree(BehaviorTree);
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}
