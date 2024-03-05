// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Muru/MuruAnimInstance.h"
#include "Enemy/Muru/Muru.h"

void UMuruAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Muru = Cast<AMuru>(Enemy);
}
void UMuruAnimInstance::AnimNotify_SendDamage()
{
	Muru->SendDamage();
}
void UMuruAnimInstance::AnimNotify_Death()
{
	Muru->Death();
}
