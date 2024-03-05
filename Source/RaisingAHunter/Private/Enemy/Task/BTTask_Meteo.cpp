// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/BTTask_Meteo.h"
#include "Enemy/Boss/BossAIController.h"
#include "Enemy/Boss/Boss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Meteo::UBTTask_Meteo()
{
	NodeName = TEXT("Meteo");
}

EBTNodeResult::Type UBTTask_Meteo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ABossAIController* BossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(BossController)) return EBTNodeResult::Failed;
	ABoss* Boss = Cast<ABoss>(BossController->GetPawn());

	if (!IsValid(Boss)) return EBTNodeResult::Failed;
	if (Boss->GetCharacterMovement()->IsFalling()) return EBTNodeResult::Failed;
	UAnimInstance* Anim = Boss->GetMesh()->GetAnimInstance();
	if (!Anim->Montage_GetIsStopped(Boss->GetCurrentMontage())) return EBTNodeResult::Failed;
	Boss->SpawnMeteo();
	return EBTNodeResult::Succeeded;
}

//void UBTTask_Meteo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
//	if (Enemy->GetCharacterMovement()->IsFalling())
//	{
//		Enemy->StopAnimMontage();
//		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//		return;
//	}
//	if (!IsValid(Enemy)) return;
//	UAnimInstance* Anim = Enemy->GetMesh()->GetAnimInstance();
//	if (!Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage())) return;
//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//}
