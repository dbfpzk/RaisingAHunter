// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface/Attackable.h"
#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Muru.generated.h"

UCLASS()
class RAISINGAHUNTER_API AMuru : public AEnemy, public IAttackable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMuru();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	virtual void Attack() override;
	void SendDamage();
	virtual void Death() override;
};
