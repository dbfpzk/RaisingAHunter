// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Hunter/Unit.h"
#include "Utile/Keys.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));;
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.f;	//�� �� �ִ� ����
	SightConfig->LoseSightRadius = 550.f;	//�̹� �� ��ǥ���� �� �� �ִ� ����
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //�þ߰���
	SightConfig->SetMaxAge(3.0f); //�� �� �ִ� �ð�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//�����켱������ �þ߷� ����
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//�þ� ���� ���
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	//�ڱؿ� ���� �Ǿ��ٸ� OnTargetDetected�Լ� ȣ��
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AEnemyAIController::OnTargetDetected(AActor* InActor, FAIStimulus Stimulus)
{
	if (InActor->ActorHasTag(FName(TEXT("Player"))))
	{
		GetBlackboardComponent()->SetValueAsBool(Key::Blackboard::isFind, Stimulus.WasSuccessfullySensed());
		TSubclassOf<UAISense> AISense;
		TArray<AActor*> Actors;
		GetPerceptionComponent()->GetCurrentlyPerceivedActors(AISense, Actors);
		for (auto Other : Actors)
		{
			if (Target = Cast<AUnit>(Other))
			{
				if (Target->GetUnitStateType() != EUnitStateType::Die)
					GetBlackboardComponent()->SetValueAsObject(Key::Blackboard::Target, Target);
			}
		}
	}
}
