// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class RAISINGAHUNTER_API AMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	/*UPROPERTY(EditAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;*/
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystemComponent> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> Sphere;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY()
	TObjectPtr<class UParticleSystem> HitParticle;
	UPROPERTY()
	TObjectPtr<class USoundWave> DestroySound;
private:
	UPROPERTY()
	TArray<FVector> Points;

	float TimerMax;
	float TimerCurrent;
	float Speed;


public:
	void Init(FTransform InStartTr, FTransform InEndTr, float InSpeed, float InNewPointDistanceFromStartTr, float InNewPointDistanceFromEndTr);
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult);

	float CubicBezierCurve(float a, float b, float c, float d);
};
