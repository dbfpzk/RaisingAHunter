// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface/Damageable.h"
#include "Utile/Enums.h"
#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Unit.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeHpSignature, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeMpSignature, float, float);
DECLARE_MULTICAST_DELEGATE(FOnGroundAttackSignature);
DECLARE_MULTICAST_DELEGATE(FOnAirAttackSignature);
DECLARE_MULTICAST_DELEGATE(FOnProjectileAttackSignature);



UCLASS()
class RAISINGAHUNTER_API AUnit : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
	virtual void Destroyed() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const&
		DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Weapon;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> MotionWarp;
	UPROPERTY(VisibleAnywhere, Category = "UnitState")
	TObjectPtr<class AUnitState> UnitState;
	UPROPERTY(VisibleAnywhere, Category = "Target")
	TObjectPtr<class USceneComponent> TargetAirPos;
	UPROPERTY(VisibleAnywhere, Category = "Target")
	TArray<TObjectPtr<class ACharacter>> AirTarget;
	UPROPERTY(VisibleAnywhere, Category = "Target")
	TObjectPtr<class UUnitAnimInstance> UnitAnim;
protected:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EUnitStateType UnitStateType;
	UPROPERTY(VisibleAnywhere, Category = "State")
	EUnitType UnitType;
	UPROPERTY(VisibleAnywhere, Category = "State")
	float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "State")
	float MaxHp;
	UPROPERTY(VisibleAnywhere, Category = "State")
	float CurrentMp;
	UPROPERTY(VisibleAnywhere, Category = "State")
	float MaxMp;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<class UParticleSystem> HitEffect;
public:
	UPROPERTY()
	float LeftAndRight;
	UPROPERTY()
	float UpAndDown;
	UPROPERTY()
	bool isClimb;
protected:
	bool isAttack;
	bool isNextAttack;
	bool isEnableCombo;
	int32 ComboCount;
	bool isMove;
	bool isAirAttack; //공중공격 가능 여부
	bool isDamage;
	FTimerHandle DamageTimer;
public:
	void Zoom(float Axis);
	virtual void Dash(FVector2D& Movement);
	virtual void Attack();
	virtual void NextAttack();
	virtual void EndAttack();
	virtual void UpperSlash();
	virtual void UpperAttack();
	virtual void FallingDown();
	void SpawnAfterImage();
	void InitState();
	virtual void Death();
	virtual void Blade();
	virtual void SpawnBlade();
public: //Interface
	virtual void SendDamage(float AdditionalDamage, float Radius, float AttackRange) override;
public: //파쿠르
	FHitResult AttachToSurfaceCalculation();//표면에 부착 계산 함수
	void ClimbMovement(FVector WorldDirection, float ScaleValue);
	virtual void LedgeMantleCalculation();
	void StopClimbing(); //파쿠르 중지
public:
	void Invincibility();
private: //리스폰
	void Respawn();
	FTimerHandle RespawnHandle;
public: //델리게이트
	FOnChangeHpSignature OnChangeHpSignature;
	FOnChangeMpSignature OnChangeMpSignature;
	FOnGroundAttackSignature OnGroundAttackSignature;
	FOnAirAttackSignature OnAirAttackSignature;
	FOnProjectileAttackSignature OnProjectileAttackSignature;
public:
	FORCEINLINE void SetUnitType(EUnitType _UnitType) { this->UnitType = _UnitType; }
	FORCEINLINE EUnitType GetUnitType() { return UnitType; }
	FORCEINLINE void SetUnitStateType(EUnitStateType _UnitStateType) { this->UnitStateType = _UnitStateType; }
	FORCEINLINE EUnitStateType GetUnitStateType() { return UnitStateType; }
	FORCEINLINE bool GetIsAttack() { return isAttack; }
	FORCEINLINE bool GetIsNextAttack() { return isNextAttack; }
	FORCEINLINE bool GetIsEnableCombo() { return isEnableCombo; }
	FORCEINLINE int32 GetComboCount() { return ComboCount; }
	FORCEINLINE bool GetIsMove() { return isMove; }
	FORCEINLINE void SetIsEnableCombo(bool value) { isEnableCombo = value; }
	FORCEINLINE void SetCurrentHp(float value) 
	{ 
		CurrentHp = value;
		if (CurrentHp >= MaxHp)
		{
			CurrentHp = MaxHp;
		}
		OnChangeHpSignature.Broadcast(CurrentHp, MaxHp);
	}
	FORCEINLINE void SetCurrentMp(float value) 
	{ 
		CurrentMp = value;
		if (CurrentMp >= MaxMp)
		{
			CurrentMp = MaxMp;
		}
		OnChangeMpSignature.Broadcast(CurrentMp, MaxMp);
	}
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetCurrentMp() { return CurrentMp; }
	FORCEINLINE TObjectPtr<class AUnitState> GetUnitState() { return UnitState; }
};
