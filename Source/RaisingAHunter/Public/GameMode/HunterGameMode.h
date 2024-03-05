// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HunterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API AHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHunterGameMode();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TObjectPtr<class AUnit> Unit;
	UPROPERTY()
	TSubclassOf<class UUserWidget> GameWidgetClass;
	UPROPERTY()
	TObjectPtr<class UGameWidget> GameWidget;

//public:
//	UPROPERTY(EditAnywhere, category = "Widget")
//	TSubclassOf<class UUserWidget> UnitWidgetClass;
public:
	UFUNCTION()
	FORCEINLINE class UGameWidget* GetGameWidget()
	{
		return GameWidget;
	}
};
