// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Notify/AnimNotify_Damage.h"
#include "Interface/Damageable.h"

UAnimNotify_Damage::UAnimNotify_Damage()
{
	AdditionalDamage = 1;
	Radius = 100;
	AttackRange = 100;
}

FString UAnimNotify_Damage::GetNotifyName_Implementation() const
{
	return TEXT("Damage");
}

void UAnimNotify_Damage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (IDamageable* Damageable = Cast<IDamageable>(MeshComp->GetOwner()))
	{
		Damageable->SendDamage(AdditionalDamage, Radius, AttackRange);
	}
}
