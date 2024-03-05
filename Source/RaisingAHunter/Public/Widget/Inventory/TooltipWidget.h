// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UTextBlock* PriceText;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UTextBlock* AmountText;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UTextBlock* ItemTypeText;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UTextBlock* DescriptionText;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
	class UImage* IconImage;
private:
	FItemData ItemData;
public:
	void SetItemData(FItemData& InItemData);
};
