// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrbWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UOrbWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(EditAnywhere, Category = "Material", meta = (BindWidget))
	TObjectPtr<class UImage> Orb;
	UPROPERTY(EditAnywhere, Category = "Material")
	TObjectPtr<class UMaterialInstanceConstant> OrbMatInstance;
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> OrbMatDynamic;
	UPROPERTY(EditAnywhere, Category = "State")
	bool isHealth = false;
private:
	FTimerHandle TimerHandle;
public:
	void SetDynamicMatPercentage(float InCurrent, float InMax);
	void Init();
};
