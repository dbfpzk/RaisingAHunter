// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace Key
{
	namespace Socket
	{
		const FName RightHand = TEXT("hand_r");
	}
	namespace Blackboard
	{
		const FName Target = TEXT("Target");
		const FName PatrolPos = TEXT("PatrolPos");
		const FName isFind = TEXT("isFind");
		const FName TargetPos = TEXT("TargetPos");
		const FName BossStateType = TEXT("BossStateType");
	}
	namespace EnemyName
	{
		const FName Muru = TEXT("Muru");
		const FName Ranger = TEXT("Ranger");
		const FName Resh = TEXT("Resh");
		const FName Riktor = TEXT("Riktor");
	}
	namespace ItemName
	{
		const TArray<FName> ItemName{ "Sword", "Dagger", "Bow", "HealthPotion", "Bomb", "Shield", "Staff", "Axe"};
	}
}

