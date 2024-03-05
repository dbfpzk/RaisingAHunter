// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/BossAnimInstance.h"
#include "Enemy/Boss/Boss.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Boss = Cast<ABoss>(Enemy);
}
void UBossAnimInstance::AnimNotify_SendDamage()
{
	Boss->SendDamage();
}
void UBossAnimInstance::AnimNotify_Death()
{
	Boss->Death();
}
