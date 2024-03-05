// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class USlotWidget>> Slots;
public:
	void AddSlotToItem(struct FItemData& InItemData);
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inventory")
	TObjectPtr<class UButton> TopButton;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inventory")
	TObjectPtr<class UOverlay> InventoryOverlay;
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Inventory")
	//TObjectPtr<class UCanvasPanel> InventoryCavas;
private:
	UFUNCTION()
	void TopButtonOnPressed();
	UFUNCTION()
	void TopButtonOnReleased();
private:
	UFUNCTION()
	void InventoryDragDrop();

	FTimerHandle DragDropHandle;
	FVector2D CanvasPos;
};
