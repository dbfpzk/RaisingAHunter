// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTTask_Attack.h"
#include "Enemy/Enemy.h"
#include "Interface/Attackable.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utile/Keys.h"
#include "Hunter/Unit.h"
#include "Utile/Enums.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(EnemyController)) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(EnemyController->GetPawn());

	if (!IsValid(Enemy)) return EBTNodeResult::Failed;
	if (Enemy->GetCharacterMovement()->IsFalling()) return EBTNodeResult::Failed;
	UAnimInstance* Anim = Enemy->GetMesh()->GetAnimInstance();
	if (!Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage())) return EBTNodeResult::Failed;
	if (IAttackable* Attackable = Cast<IAttackable>(Enemy))
	{
		Attackable->Attack();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy->GetCharacterMovement()->IsFalling())
	{
		Enemy->StopAnimMontage();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	if (!IsValid(Enemy)) return;
	UAnimInstance* Anim = Enemy->GetMesh()->GetAnimInstance();
	if (Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
