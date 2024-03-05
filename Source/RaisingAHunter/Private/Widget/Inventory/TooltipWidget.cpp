// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/TooltipWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTooltipWidget::SetItemData(FItemData& InItemData)
{
	ItemData = InItemData;
	NameText->SetText(FText::FromName(ItemData.Name));
	PriceText->SetText(FText::AsNumber(ItemData.Price));
	DescriptionText->SetText(FText::FromString(ItemData.Description));
	IconImage->SetBrushFromTexture(ItemData.Icon);
	IconImage->SetDesiredSizeOverride(FVector2D(70, 70));
	ItemTypeText->SetText(StaticCast<FText>(ItemData));
	if (ItemData.Type == EItemType::Countable || ItemData.Type == EItemType::Etc)
	{
		AmountText->SetText(FText::AsNumber(ItemData.Amount));
	}
	else
	{
		AmountText->SetText(FText::FromString(TEXT("")));
	}
}
