// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTService_ChangeSpeed.h"

#include "Enemy/EnemyAIController.h"
#include "Enemy/Boss/Boss.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utile/Keys.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("ChangeSpeed");
}

void UBTService_ChangeSpeed::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	AEnemy* Enemy = Cast<AEnemy>(SearchData.OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy)) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if(!OwnerComp.GetAIOwner()->GetPawn()->IsA(ABoss::StaticClass())) return;
	UObject* Obj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(Key::Blackboard::Target);
	if (!IsValid(Obj)) return;
	AActor* Target = Cast<AActor>(Obj);
	if (!IsValid(Target)) return;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Key::Blackboard::TargetPos, Target->GetActorLocation());
}
