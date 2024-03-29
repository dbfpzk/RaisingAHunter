// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Orb", meta = (BindWidget))
	TObjectPtr<class UOrbWidget> HealthOrb;
	UPROPERTY(EditAnywhere, Category = "Orb", meta = (BindWidget))
	TObjectPtr<class UOrbWidget> ManaOrb;
	UPROPERTY(EditAnywhere, Category = "Orb", meta = (BindWidget))
	TObjectPtr<class USkillSetWidget> SkillSet;

};
