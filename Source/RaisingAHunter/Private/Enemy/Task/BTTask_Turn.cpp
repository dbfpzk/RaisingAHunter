// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTTask_Turn.h"
#include "Utile/Keys.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyAIController.h"
#include "Hunter/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_Turn::UBTTask_Turn()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_Turn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy)) return EBTNodeResult::Failed;

	AUnit* Target = Cast<AUnit>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Key::Blackboard::Target));
	if (!IsValid(Target)) return EBTNodeResult::Failed;

	FVector Direction = Target->GetActorLocation() - Enemy->GetActorLocation();
	Direction.Z = 0.0f;
	FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();
	Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), Rot, GetWorld()->GetDeltaSeconds(), 10.0f));


	return EBTNodeResult::Succeeded;
}
