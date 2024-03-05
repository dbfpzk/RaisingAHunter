// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utile/Structures.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class RAISINGAHUNTER_API AItem : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AItem();
protected:
	//���Ͱ� ������ ��ġ�Ǿ��ų� �����Ǹ� ȣ��Ǵ� �Լ�
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
private:
	UPROPERTY(EditAnywhere, Category = "ItemData")
	struct FDataTableRowHandle ItemDataRowHandle;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	struct FItemData ItemData;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<class USkeletalMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<class USphereComponent> Sphere;
public:
	FORCEINLINE void SetItemRowName(const FName& InItemName)
	{
		ItemDataRowHandle.RowName = InItemName;
	}
	FORCEINLINE void SetItemAmount(const int32& InAmount)
	{
		ItemData.Amount = InAmount;
	}
};
