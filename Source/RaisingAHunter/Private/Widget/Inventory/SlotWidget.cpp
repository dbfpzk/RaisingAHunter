// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/SlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Widget/Inventory/TooltipWidget.h"
#include "Widget/Inventory/SlotDragDropOperation.h"
#include "Item/Item.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter/Unit.h"

USlotWidget::USlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NoneTextureAsset(TEXT("/Script/Engine.Texture2D'/Game/Icons/NoneIcon.NoneIcon'"));
	if (NoneTextureAsset.Succeeded())
	{
		NoneTexture = NoneTextureAsset.Object;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> ToolTipAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Widget/Inventory/WB_Tooltip.WB_Tooltip_C'"));
	if (ToolTipAsset.Succeeded())
	{
		ToolTipClass = ToolTipAsset.Class;
	}
	ItemData.Amount = 0;
}

void USlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ItemData.Amount != 0)
	{
		Icon->SetBrushFromTexture(ItemData.Icon);
		Icon->SetDesiredSizeOverride(FVector2D(70.f, 70.f));
		AmountText->SetText(FText::AsNumber(ItemData.Amount));
	}
	else
	{
		Icon->SetBrushFromTexture(NoneTexture);
		AmountText->SetText(FText::FromString(TEXT("")));
	}
	ToolTip = Cast<UTooltipWidget>(CreateWidget(GetWorld(), ToolTipClass));
	ToolTip->SetItemData(ItemData);
	Unit = Cast<AUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void USlotWidget::SetItemData(const FItemData& InItemData)
{
	ItemData = InItemData;
	if (ItemData.Amount != 0)
	{
		Icon->SetBrushFromTexture(ItemData.Icon);
		Icon->SetDesiredSizeOverride(FVector2D(70.f, 70.f));
		Icon->SetToolTip(ToolTip);
		ToolTip->SetItemData(ItemData);
		if (ItemData.Type == EItemType::Countable || ItemData.Type == EItemType::Etc)
		{
			AmountText->SetText(FText::AsNumber(ItemData.Amount));
		}
		else
		{
			AmountText->SetText(FText::FromString(TEXT("")));
		}
	}
	else
	{
		Icon->SetBrushFromTexture(NoneTexture);
		Icon->SetToolTip(nullptr);
		AmountText->SetText(FText::FromString(TEXT("")));
	}
}

void USlotWidget::SpawnItem()
{
	if (!IsValid(Unit)) return;
	FTransform Transform = Unit->GetActorTransform();
	FVector SpawnLocation = Unit->GetActorLocation() + (Unit->GetActorForwardVector() * 200);
	Transform.SetLocation(SpawnLocation);
	AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(AItem::StaticClass(), Transform);
	Item->SetItemRowName(ItemData.Name);
	Item->FinishSpawning(Transform);
	Item->SetItemAmount(ItemData.Amount);
	SetItemData(FItemData());
}

void USlotWidget::ItemUse()
{
	if (ItemData.Name == FName(TEXT("HealthPotion")))
	{
		if (IsValid(Unit))
		{
			Unit->SetCurrentHp(Unit->GetCurrentHp() + 50.f);
		}
	}
}

FReply USlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		//DetectDragIfPressed : 버튼이 눌린상태로 드래그했는지 판단하는 함수
	}
	if (InMouseEvent.IsMouseButtonDown(FKey("RightMouseButton")))
	{
		if (ItemData.Type == EItemType::Countable)
		{
			ItemUse();
			ItemData.Amount--;
			AmountText->SetText(FText::AsNumber(ItemData.Amount));
			ToolTip->SetItemData(ItemData);
			if (ItemData.Amount <= 0)
			{
				SetItemData(FItemData());
				Icon->SetBrushFromTexture(NoneTexture);
				Icon->SetToolTip(nullptr);
			}
		}
	}
	return Reply.NativeReply;
}

void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (ItemData.Amount == 0)
		return;

	USlotDragDropOperation* Operation = NewObject<USlotDragDropOperation>();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	Operation->SetItemData(ItemData);
	Operation->WidgetRef = this;
	Operation->DefaultDragVisual = this; //드래그 시 표시될 위젯
	Operation->Pivot = EDragPivot::MouseDown; //드래그시 위젯이 표시되는 위치
	OutOperation = Operation; //오퍼레이션 적용
}

void USlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	SetVisibility(ESlateVisibility::Visible);
}

bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	USlotDragDropOperation* Operation = Cast<USlotDragDropOperation>(InOperation);
	if (Operation->WidgetRef != this)
	{
		FItemData Temp = ItemData;
		SetItemData(Operation->GetItemData()); //나는 상대아이템 저장
		USlotWidget* SlotWidget = Cast<USlotWidget>(Operation->WidgetRef);
		SlotWidget->SetItemData(Temp); //상대 아이템 초기화
	}
	Operation->WidgetRef->SetVisibility(ESlateVisibility::Visible);
	SetVisibility(ESlateVisibility::Visible);
	return false;
}
