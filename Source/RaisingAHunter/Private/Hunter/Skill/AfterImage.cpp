// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Skill/AfterImage.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AAfterImage::AAfterImage()
{
	PrimaryActorTick.bCanEverTick = true;
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = PoseableMesh;
	PoseableMesh->SetRelativeLocation(FVector(0, 0, -88));
	PoseableMesh->SetRelativeRotation(FRotator(0, -90, 0));
	static ConstructorHelpers::FObjectFinder<UMaterial> GhostMaterialAsset(TEXT("/Script/Engine.Material'/Game/Materials/MAT_GhostTrail.MAT_GhostTrail'"));
	if (GhostMaterialAsset.Succeeded())
	{
		GhostMaterial = GhostMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> HiddenMaterialAsset(TEXT("/Script/Engine.Material'/Game/Materials/MAT_Hidden.MAT_Hidden'"));
	if (HiddenMaterialAsset.Succeeded())
	{
		HiddenMaterial = HiddenMaterialAsset.Object;
	}
}

// Called when the game starts or when spawned
void AAfterImage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAfterImage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isSpawn) Destroy();
	FadeCountDown -= DeltaTime;
	for (int i = 0; i < GhostMaterials.Num(); i++)
	{
		if (i == 2) continue;
		GhostMaterials[i]->SetScalarParameterValue("Opacity", FadeCountDown / FadeOutTime);
	}
	if (FadeCountDown <= 0)
	{
		Destroy();
	}
}

void AAfterImage::SetPoseableMeshPos(TObjectPtr<class USkeletalMeshComponent> SkeletalMesh)
{
	if (!IsValid(SkeletalMesh)) Destroy();
	isSpawn = true;
	FadeCountDown = FadeOutTime;
	PoseableMesh->SetSkinnedAssetAndUpdate(SkeletalMesh->GetSkeletalMeshAsset());
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh); //포즈복사
	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials(); //메쉬의 머티리얼 가져옴
	for (int i = 0; i < Mats.Num(); i++) //머티리얼 갯수
	{
		if (i == 3)
		{
			GhostMaterials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), HiddenMaterial));
			continue;
		}
		GhostMaterials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
		PoseableMesh->SetMaterial(i, GhostMaterials[i]);
	}
}

