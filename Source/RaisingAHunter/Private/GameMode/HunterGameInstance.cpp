// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/HunterGameInstance.h"
#include "Engine/DataTable.h"
#include "Utile/Structures.h"

UHunterGameInstance::UHunterGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyDataAsset(TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_EnemyData.DT_EnemyData'"));
	if (EnemyDataAsset.Succeeded())
	{
		EnemyDataTable = EnemyDataAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> HunterDataAsset(TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_UnitStatus.DT_UnitStatus'"));
	if (HunterDataAsset.Succeeded())
	{
		HunterDataTable = HunterDataAsset.Object;
	}
}

FEnemyData* UHunterGameInstance::GetEnemyDataTable(const FName& RowName)
{
	return EnemyDataTable->FindRow<FEnemyData>(RowName, TEXT("Not Find EnemyDataTable"));
}

FUnitStatus* UHunterGameInstance::GetHunterDataTable(const FName& RowName)
{
	return HunterDataTable->FindRow<FUnitStatus>(RowName, TEXT("Not Find HunterDataTable"));
}
