// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/InventoryWidget.h"
#include "Widget/Inventory/SlotWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Utile/Structures.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < 30; i++)
	{
		FName SlotName = FName(FString::Printf(TEXT("WB_Slot_%d"), i));

		USlotWidget* NewSlot = Cast<USlotWidget>(GetWidgetFromName(SlotName));
		if (IsValid(NewSlot))
		{
			Slots.Add(NewSlot);
		}
	}
	TopButton->OnPressed.AddDynamic(this, &UInventoryWidget::TopButtonOnPressed);
	TopButton->OnReleased.AddDynamic(this, &UInventoryWidget::TopButtonOnReleased);
}

void UInventoryWidget::AddSlotToItem(FItemData& InItemData)
{
	for (auto ItemSlot : Slots)
	{
		if (ItemSlot->ItemData == InItemData && ItemSlot->ItemData.IsEquipment())
		{
			continue;
		}
		else if (ItemSlot->ItemData.Amount == 0)
		{
			ItemSlot->ItemData = InItemData;
			ItemSlot->SetItemData(ItemSlot->ItemData);
			return;
		}
		else if (ItemSlot->ItemData == InItemData)
		{
			ItemSlot->ItemData.Amount += InItemData.Amount;
			ItemSlot->SetItemData(ItemSlot->ItemData);
			return;
		}
	}
}

void UInventoryWidget::TopButtonOnPressed()
{
	FVector2D CursorPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InventoryOverlay);
	if (PanelSlot == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not Slot!"));
		return;
	}
	FVector2D SlotPos = PanelSlot->GetPosition();
	CanvasPos = CursorPos - SlotPos;
	GetWorld()->GetTimerManager().SetTimer(DragDropHandle, this, &UInventoryWidget::InventoryDragDrop, 0.02f, true);
}

void UInventoryWidget::TopButtonOnReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(DragDropHandle);
}

void UInventoryWidget::InventoryDragDrop()
{
	FVector2D CursorPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InventoryOverlay);

	PanelSlot->SetPosition(CursorPos - CanvasPos);
}
