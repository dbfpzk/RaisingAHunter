// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HunterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UHunterGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UHunterGameInstance();
private:
	UPROPERTY()
	class UDataTable* EnemyDataTable;
	UPROPERTY()
	class UDataTable* HunterDataTable;
public:
	struct FEnemyData* GetEnemyDataTable(const FName& RowName);
	struct FUnitStatus* GetHunterDataTable(const FName& RowName);
};
