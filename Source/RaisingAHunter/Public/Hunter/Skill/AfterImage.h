// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AfterImage.generated.h"

UCLASS()
class RAISINGAHUNTER_API AAfterImage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAfterImage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void SetPoseableMeshPos(TObjectPtr<class USkeletalMeshComponent> SkeletalMesh);
private:
	UPROPERTY()
	TObjectPtr<class UPoseableMeshComponent> PoseableMesh;
	UPROPERTY()
	TObjectPtr<class UMaterial> GhostMaterial;
	UPROPERTY()
	TObjectPtr<class UMaterial> HiddenMaterial;
	UPROPERTY()
	TArray<TObjectPtr<class UMaterialInstanceDynamic>> GhostMaterials;

	bool isSpawn = false;
	float FadeCountDown;
	float FadeOutTime = 0.5f;
};
