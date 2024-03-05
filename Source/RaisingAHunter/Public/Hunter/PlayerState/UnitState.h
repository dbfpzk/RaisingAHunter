// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UnitState.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API AUnitState : public APlayerState
{
	GENERATED_BODY()
public:
	AUnitState();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TObjectPtr<class UDataTable> UnitStatusTable;

	FUnitStatus CurrentStatus; //현재 레벨의 데이터 테이블 정보
	UPROPERTY()
	FUnitStatus ReinforceStatus; //강화한 스텟 정보
	UPROPERTY()
	FReinforceLevel ReinforceLevel; //강화한 스텟 레벨
	UPROPERTY()
	int32 Level; //현재 레벨
	UPROPERTY()
	TObjectPtr<class AUnit> Unit;
	UPROPERTY()
	TObjectPtr<class UHunterGameInstance> Instance;
public:
	UFUNCTION()
	void LevelUp();
private:
	UPROPERTY()
	TArray<FItemData> SlotDatas;
public:
	UFUNCTION()
	FORCEINLINE TArray<struct FItemData> GetSlotDatas()
	{
		return SlotDatas;
	}
public:
	struct FUnitStatus* GetUnitStatus(int32 InLevel);

	FORCEINLINE FUnitStatus GetCurrentStatus()
	{
		return CurrentStatus;
	}
	FORCEINLINE int32 GetMaxHp()
	{
		return CurrentStatus.MaxHp + CurrentStatus.Strength + ReinforceStatus.MaxHp + ReinforceStatus.Strength;
	}
	FORCEINLINE int32 GetMaxMp()
	{
		return CurrentStatus.MaxMp + CurrentStatus.Intellect + ReinforceStatus.MaxMp + ReinforceStatus.Intellect;
	}
	FORCEINLINE float GetDamage()
	{
		return CurrentStatus.Damage + CurrentStatus.Strength + ReinforceStatus.Damage + ReinforceStatus.Strength;
	}
	FORCEINLINE float GetDefence()
	{
		return CurrentStatus.Defence + CurrentStatus.Agility + ReinforceStatus.Defence + ReinforceStatus.Agility;
	}
};
