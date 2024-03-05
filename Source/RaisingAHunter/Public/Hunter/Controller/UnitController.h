// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnitController.generated.h"

/**
 * 
 */
UCLASS()
class RAISINGAHUNTER_API AUnitController : public APlayerController
{
	GENERATED_BODY()
public:
	AUnitController();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
private: //Unit
	UPROPERTY()
	TObjectPtr<class AUnit> Unit;
private: //Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ClimbAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetLeftAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> UpperSlashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InvincibilityAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BladeAction;
private:
	UPROPERTY()
	FVector2D Movement;
private: //Targeting
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	float LockOnDistance = 1000.f;
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TObjectPtr<class AEnemy> LockOnEnemy;
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TArray<TObjectPtr<class AEnemy>> TargetActors;
public:
	FORCEINLINE AEnemy* GetLockOnEnemy()
	{
		return LockOnEnemy;
	}
protected:
	UFUNCTION()
	void Move(const struct FInputActionValue& Value);
	UFUNCTION()
	void EndMove();
	UFUNCTION()
	void Look(const struct FInputActionValue& Value);
	UFUNCTION()
	void Zoom(const struct FInputActionValue& Value);
	UFUNCTION()
	void Dash();
	UFUNCTION()
	void Jump();
	UFUNCTION()
	void Climb();
	UFUNCTION()
	void Attack();
	UFUNCTION()
	void Targeting();
	UFUNCTION()
	void TargetLeft();
	UFUNCTION()
	void TargetRight();
	UFUNCTION()
	void UpperSlash();
	UFUNCTION()
	void Inventory();
	UFUNCTION()
	void Invincibility();
	UFUNCTION()
	void Blade();
private: //Targeting
	void GetTraceTarget();
	void GetTargetEnemy();
	void StartLockOn();
	void EndLockOn();
	void LookAtTarget();
	float GetProjectionLength(TObjectPtr<AActor> Target);
	void GetOtherTarget(bool isRight);
public: // * 인벤토리
	void AddItem(struct FItemData InItemData);
	int FindStack(struct FItemData& InItemData);
private:
	UPROPERTY()
	TObjectPtr<class AUnitState> UnitState;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInventoryWidget> InventoryWidget;
};
