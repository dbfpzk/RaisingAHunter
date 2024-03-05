// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Components/SphereComponent.h"
#include "Hunter/Unit.h"
#include "Hunter/Controller/UnitController.h"
#include "Utile/Keys.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	// * 데이터 테이블
	static ConstructorHelpers::FObjectFinder<UDataTable> DataAsset(TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_ItemData.DT_ItemData'"));
	if (DataAsset.Succeeded())
	{
		ItemDataRowHandle.DataTable = DataAsset.Object;
		ItemDataRowHandle.RowName = TEXT("Bow");
	}

	// * 메쉬
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("Item"));
	Mesh->SetRelativeScale3D(FVector(1.f));
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;
	Tags.Add(FName("Item"));

	// * 콜리더
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Sphere->InitSphereRadius(100.f);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ItemData = *ItemDataRowHandle.DataTable->FindRow<FItemData>(ItemDataRowHandle.RowName, "Not Fine Row");
	if (&ItemData != nullptr && Mesh != nullptr)
	{
		Mesh->SetSkeletalMesh(ItemData.ItemMesh);
	}
	ItemData.Amount = 1;
	if (ItemData.Name == TEXT("Bow"))
	{
		Mesh->SetRelativeScale3D(FVector(.2f));
	}
}

void AItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (auto Unit = Cast<AUnit>(OtherActor))
	{
		AUnitController* UnitController = Unit->GetController<AUnitController>();
		if (IsValid(UnitController))
		{
			UnitController->AddItem(ItemData);
			Destroy();
		}
	}
}



