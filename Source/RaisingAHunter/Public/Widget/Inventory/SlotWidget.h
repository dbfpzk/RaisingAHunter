// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USlotWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot", meta = (BindWidget))
	TObjectPtr<class UImage> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmountText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	TObjectPtr<class UTexture2D> NoneTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip")
	TSubclassOf<class UTooltipWidget> ToolTipClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip")
	TObjectPtr<class UTooltipWidget> ToolTip;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip")
	TObjectPtr<class AUnit> Unit;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot")
	struct FItemData ItemData;
public:
	void SetItemData(const struct FItemData& InItemData);
	void SpawnItem();
	void ItemUse();
private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
