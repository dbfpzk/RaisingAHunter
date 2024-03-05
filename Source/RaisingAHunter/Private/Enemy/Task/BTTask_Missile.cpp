// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTTask_Missile.h"
#include "Enemy/Boss/BossAIController.h"
#include "Enemy/Boss/Boss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Missile::UBTTask_Missile()
{
	NodeName = TEXT("Missile");
}

EBTNodeResult::Type UBTTask_Missile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ABossAIController* BossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	ABoss* Boss = Cast<ABoss>(BossController->GetPawn());
	if (!IsValid(Boss)) return EBTNodeResult::Failed;
	if (Boss->GetCharacterMovement()->IsFalling()) return EBTNodeResult::Failed;
	UAnimInstance* Anim = Boss->GetMesh()->GetAnimInstance();
	if (!Anim->Montage_GetIsStopped(Boss->GetCurrentMontage())) return EBTNodeResult::Failed;
	Boss->SpawnMissile();
	return EBTNodeResult::Succeeded;
}
