// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSetWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API USkillSetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USkillSetWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
private:
	UPROPERTY(EditAnywhere, Category = "Material", meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage1;
	UPROPERTY(EditAnywhere, Category = "Material", meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage2;
	UPROPERTY(EditAnywhere, Category = "Material", meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage3;
	UPROPERTY(EditAnywhere, Category = "Material")
	TArray<TObjectPtr<class UMaterialInstanceConstant>> SkillMatInstance;
	UPROPERTY()
	TArray<TObjectPtr<class UMaterialInstanceDynamic>> SkillMatDynamic;
	
	UPROPERTY()
	TArray<bool> isSkills;
	UPROPERTY()
	TArray<float> SkillMaxCoolTimes;
public:
	void SkillCoolTime(const float& InDeltaTime);
public:
	UFUNCTION()
	void Init();
};
