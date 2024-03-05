// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

public:
	void SetHealth(float CurrnetHp, float MaxHp);
};
