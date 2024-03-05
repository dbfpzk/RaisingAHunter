// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/HunterGameMode.h"
#include "Hunter/Hunter.h"
#include "Hunter/Controller/HunterController.h"
#include "Hunter/PlayerState/UnitState.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Game/GameWidget.h"


AHunterGameMode::AHunterGameMode()
{
	DefaultPawnClass = AHunter::StaticClass();
	PlayerControllerClass = AHunterController::StaticClass();
	PlayerStateClass = AUnitState::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> GameWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Widget/Game/WB_Game.WB_Game_C'"));
	if (GameWidgetAsset.Succeeded())
	{
		GameWidgetClass = GameWidgetAsset.Class;
	}
}

void AHunterGameMode::BeginPlay()
{
	Super::BeginPlay();
	Unit = Cast<AUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (GameWidgetClass != nullptr)
	{
		GameWidget = CreateWidget<UGameWidget>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport();
	}
}
