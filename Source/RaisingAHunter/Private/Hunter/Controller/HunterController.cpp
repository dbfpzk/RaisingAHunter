// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Controller/HunterController.h"

AHunterController::AHunterController()
{
}

void AHunterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AHunterController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AHunterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}
