// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHealthEvent, float, float);

UCLASS()
class RAISINGAHUNTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Landed(const FHitResult& Hit) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	FEnemyData EnemyData;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	FVector TopInfoPos;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	float CurrentHp;
public://Å¸°ÙÆÃ
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class UParticleSystemComponent> TargetEffectComp;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy")
	TObjectPtr<class UTextRenderComponent> NameTextRenderComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class UWidgetComponent> HealthWidgetComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class USceneComponent> InfoPosComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class APlayerCameraManager> CameraManager;
protected: //¸ùÅ¸ÁÖ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class UAnimMontage> StandUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class UAnimMontage> DeathMontage;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class UParticleSystem> AttackFX;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Enemy")
	TObjectPtr<class UParticleSystem> DeathFX;
public://Sound
	UPROPERTY()
	TObjectPtr<class USoundWave> HitSound;
public:
	void SetTopInfomationLocation(FVector NewLocation);
	void RotateWidgetToCamera();
	void ShowHealthBar(bool isShow);
	virtual void Death();
public:
	FOnHealthEvent OnHealthEvent;
	FTimerHandle HealthBarTimer;
	FTimerHandle DeathTimer;
	FTimerHandle DamageTimer;
public:
	FORCEINLINE float GetCurrentHp()
	{
		return CurrentHp;
	}
	FORCEINLINE float GetMaxHp()
	{
		return EnemyData.MaxHp;
	}
private:
	bool isDeath;
	bool isDamage;
	FVector StartLocation;

	void SpawnItem();
};
