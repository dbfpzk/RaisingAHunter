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
// 할일 목록
//1. 공중콤보 구현
//2. 캐릭터 정보창
//3. 강화창
//4. 사운드
//5. 인벤토리
//6. 파쿠르
//7. 조이스틱
//8. 스킬
//9. 패치시스템
//10. 아마존 DynamoDB
//11. 출시