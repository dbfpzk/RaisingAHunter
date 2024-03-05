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
	SightConfig->SightRadius = 500.f;	//볼 수 있는 범위
	SightConfig->LoseSightRadius = 550.f;	//이미 본 목표물을 볼 수 있는 범위
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //시야각도
	SightConfig->SetMaxAge(3.0f); //볼 수 있는 시간
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//감지우선순위를 시야로 설정
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//시야 정보 등록
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	//자극에 감지 되었다면 OnTargetDetected함수 호출
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
