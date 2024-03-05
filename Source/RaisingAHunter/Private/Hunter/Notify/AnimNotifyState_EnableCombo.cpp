// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Notify/AnimNotifyState_EnableCombo.h"
#include "Hunter/Hunter.h"

FString UAnimNotifyState_EnableCombo::GetNotifyName_Implementation() const
{
	return TEXT("EnableCombo");
}

void UAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (!IsValid(MeshComp)) return;
	AUnit* Unit = Cast<AUnit>(MeshComp->GetOwner());
	if (!IsValid(Unit)) return;
	Unit->SetIsEnableCombo(true);
	CurrentPos = Unit->GetActorLocation();
}

void UAnimNotifyState_EnableCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!IsValid(MeshComp)) return;
	AUnit* Unit = Cast<AUnit>(MeshComp->GetOwner());
	if (!IsValid(Unit)) return;
	Unit->SetActorLocation(FVector(CurrentPos.X, CurrentPos.Y, Unit->GetActorLocation().Z));
}

void UAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (!IsValid(MeshComp)) return;
	AUnit* Unit = Cast<AUnit>(MeshComp->GetOwner());
	if (!IsValid(Unit)) return;
	Unit->SetIsEnableCombo(false);
}
