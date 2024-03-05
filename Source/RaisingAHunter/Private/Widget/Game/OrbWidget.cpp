// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Game/OrbWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter/Unit.h"


void UOrbWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OrbMatDynamic = UMaterialInstanceDynamic::Create(OrbMatInstance, this);
	if (OrbMatDynamic != nullptr)
	{
		Orb->SetBrushFromMaterial(OrbMatDynamic);
		OrbMatDynamic->SetScalarParameterValue(TEXT("Percentage"), 1.0f);
	}
	
	Init();
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UOrbWidget::Init, 1.f);
}

void UOrbWidget::SetDynamicMatPercentage(float InCurrent, float InMax)
{
	if (OrbMatDynamic != nullptr)
	{
		OrbMatDynamic->SetScalarParameterValue(TEXT("Percentage"), InCurrent / InMax);
	}
}

void UOrbWidget::Init()
{
	AUnit* Unit = Cast<AUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (isHealth)
	{
		Unit->OnChangeHpSignature.AddUObject(this, &UOrbWidget::SetDynamicMatPercentage);
	}
	else
	{
		Unit->OnChangeMpSignature.AddUObject(this, &UOrbWidget::SetDynamicMatPercentage);
	}
	if (OrbMatDynamic != nullptr)
	{
		OrbMatDynamic->SetScalarParameterValue(TEXT("Percentage"), 1.0f);
	}
}
