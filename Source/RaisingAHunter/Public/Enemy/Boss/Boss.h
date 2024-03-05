// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Enums.h"
#include "Interface/Attackable.h"
#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Boss.generated.h"

UCLASS()
class RAISINGAHUNTER_API ABoss : public AEnemy, public IAttackable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	virtual void Attack() override;
	void SendDamage();
	virtual void Death() override;
	void SpawnMeteo();
	void FinishMeteorSpawn();
	void SpawnMissile();
	void FinishMissileSpawn();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class UAnimMontage> MeteoMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class UNavigationSystemV1> NavSystem;
private:
	UPROPERTY(VisibleAnywhere, Category = "BossState")
	EBossStateType BossState;
	bool isDamage;
private: //메테오
	int MeteorIndex;
	UPROPERTY()
	TArray<FTimerHandle> MeteorTimers;
	UPROPERTY()
	TArray<TObjectPtr<class AMeteor>> Meteors;
private: //베지어미사일
	UPROPERTY()
	TObjectPtr<class ACharacter> Target;
	float Speed = 1.f;
	float DistanceFromStart = 3000.f; //시작지점을 기준으로 얼마나 꺾일지
	float DistanceFromEnd = 1500.f; //도착지점 기준으로 얼마나 꺾일지
	TArray<FTimerHandle> MissileTimers;
	TArray<TObjectPtr<class AMissile>> Missiles;
	int MissileIndex;
public:
	FORCEINLINE void SetBossState(EBossStateType& InBossState)
	{
		BossState = InBossState;
	}
	FORCEINLINE EBossStateType GetBossState()
	{
		return BossState;
	}
};
