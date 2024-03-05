// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/DropWidget.h"
#include "Widget/Inventory/SlotWidget.h"
#include "Widget/Inventory/SlotDragDropOperation.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter/PlayerState/UnitState.h"
#include "Blueprint/WidgetBlueprintLibrary.h"



void UDropWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Visible);
}

bool UDropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	USlotDragDropOperation* Operation = Cast<USlotDragDropOperation>(InOperation);
	if (Operation->WidgetRef != this)
	{
		USlotWidget* SlotWidget = Cast<USlotWidget>(Operation->WidgetRef);
		SlotWidget->SpawnItem();
		APlayerState* PlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0);
		AUnitState* UnitState = Cast<AUnitState>(PlayerState);
		if (IsValid(UnitState))
		{
			for (int i = 0; i < UnitState->GetSlotDatas().Num(); i++)
			{
				if (UnitState->GetSlotDatas()[i] == SlotWidget->ItemData)
				{
					if (UnitState->GetSlotDatas()[i].Amount > 1)
					{
						UnitState->GetSlotDatas()[i].Amount--;
					}
					else
					{
						UnitState->GetSlotDatas().RemoveAt(i);
					}
				}
			}
			
		}
	}
	
	Operation->WidgetRef->SetVisibility(ESlateVisibility::Visible);
	return false;
}
