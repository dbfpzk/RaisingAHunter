// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API USlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DragDrop")
	TObjectPtr<class UUserWidget> WidgetRef;
private:
	struct FItemData ItemData;
public:
	FORCEINLINE FItemData& GetItemData()
	{
		return ItemData;
	}
	FORCEINLINE void SetItemData(FItemData& InItemData)
	{
		ItemData = InItemData;
	}
};
