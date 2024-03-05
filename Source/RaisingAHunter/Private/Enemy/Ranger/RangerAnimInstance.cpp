// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ranger/RangerAnimInstance.h"

#include "Enemy/Ranger/Ranger.h"

void URangerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Ranger = Cast<ARanger>(Enemy);
}

void URangerAnimInstance::AnimNotify_SendDamage()
{
	Ranger->SendDamage();
}

void URangerAnimInstance::AnimNotify_Death()
{
	Ranger->Death();
}
