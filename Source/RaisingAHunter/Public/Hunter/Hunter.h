// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "Hunter.generated.h"

UCLASS()
class RAISINGAHUNTER_API AHunter : public AUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY()
	TObjectPtr<class UHunterAnimInstance> HunterAnim;
	UPROPERTY()
	TObjectPtr<class AHunterController> HunterController;
public:
	virtual void Dash(FVector2D& Movement) override;
	virtual void Attack() override;
	virtual void NextAttack() override;
	virtual void EndAttack() override;
	virtual void UpperSlash() override;
	virtual void UpperAttack() override;
	virtual void FallingDown() override;
	virtual void LedgeMantleCalculation() override;
	virtual void Death() override;
	virtual void Blade() override;
	virtual void SpawnBlade() override;
};
// ���� ���
//1. �����޺� ����
//2. ĳ���� ����â
//3. ��ȭâ
//4. ����
//5. �κ��丮
//6. ����
//7. ���̽�ƽ
//8. ��ų
//9. ��ġ�ý���
//10. �Ƹ��� DynamoDB
//11. ���