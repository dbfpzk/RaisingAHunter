// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class RAISINGAHUNTER_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystemComponent> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> Sphere;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY()
	TObjectPtr<class UParticleSystem> HitParticle;
	UPROPERTY()
	TObjectPtr<class UParticleSystem> PortalParticle;
	UPROPERTY()
	TObjectPtr<class UParticleSystem> SpawnParticle;
	UPROPERTY()
	TObjectPtr<class USoundWave> DestroySound;
public:
	void Init();
	FVector ArrivalLoc;
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult);

};
