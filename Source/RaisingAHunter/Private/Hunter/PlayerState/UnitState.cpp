// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/PlayerState/UnitState.h"
#include "GameMode/HunterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter/Unit.h"


AUnitState::AUnitState()
{
	Level = 1;
}

void AUnitState::BeginPlay()
{
	Super::BeginPlay();
	Instance = Cast<UHunterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentStatus = *GetUnitStatus(Level);
	Unit = Cast<AUnit>(GetPlayerController()->GetPawn());
}

void AUnitState::LevelUp()
{
	CurrentStatus = *GetUnitStatus(++Level);
	Unit->InitState();
}

FUnitStatus* AUnitState::GetUnitStatus(int32 InLevel)
{
	if (IsValid(Instance))
	{
		return Instance->GetHunterDataTable(*FString::FromInt(InLevel));
	}
	return nullptr;
	
}
