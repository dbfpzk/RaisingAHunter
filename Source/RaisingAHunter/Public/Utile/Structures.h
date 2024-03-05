// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType, Atomic)
//Atomic : ����ȭ(������ ����Ʈ ������ ��ȯ�Ͽ� ����, ��Ʈ��ũ �ۼ����� ����)
// * ������ ����Ʈ ������ �����Ͱ� �̵��Ѵ�
//BlueprintType : �������Ʈ���� ��� �����ϰ� �������
struct FUnitStatus : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Level; //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Strength; //��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Agility; //��ø
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Intellect; //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Luck; //���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Damage; //���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Defence; //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHp; //�ִ� Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMp; //�ִ� MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CriticalDamage; //ũ��Ƽ�� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CiriticalPercent; //ũ��Ƽ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 NextExp; //�ʿ� ����ġ��
};

USTRUCT(BlueprintType, Atomic)
//Atomic : ����ȭ(������ ����Ʈ ������ ��ȯ�Ͽ� ����, ��Ʈ��ũ �ۼ����� ����)
// * ������ ����Ʈ ������ �����Ͱ� �̵��Ѵ�
//BlueprintType : �������Ʈ���� ��� �����ϰ� �������
struct FReinforceLevel : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 StrengthLevel; //��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 AgilityLevel; //��ø
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 IntellectLevel; //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 LuckLevel; //���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 DamageLevel; //���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 DefenceLevel; //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 MaxHpLevel; //�ִ� Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 MaxMpLevel; //�ִ� MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 CriticalDamageLevel; //ũ��Ƽ�� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 CiriticalPercentLevel; //ũ��Ƽ�� Ȯ��
};

USTRUCT(BlueprintType, Atomic)
//Atomic : ����ȭ(������ ����Ʈ ������ ��ȯ�Ͽ� ����, ��Ʈ��ũ �ۼ����� ����)
// * ������ ����Ʈ ������ �����Ͱ� �̵��Ѵ�
//BlueprintType : �������Ʈ���� ��� �����ϰ� �������
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FItemData() : Name(TEXT("")), Type(EItemType::None), Price(0), Amount(0), Description(TEXT("")), bStack(false), ItemMesh(nullptr), Icon(nullptr) {}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	EItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 Amount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	bool bStack; //���� �� �ִ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<class USkeletalMesh> ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<class UTexture2D> Icon;
public: //������ �����ε�
	bool operator==(const FItemData& InItemData) const
	{
		bool bName = (Name == InItemData.Name);
		bool bType = (Type == InItemData.Type);
		bool bPrice = (Price == InItemData.Price);
		bool bMesh = (ItemMesh == InItemData.ItemMesh);
		bool bIcon = (Icon == InItemData.Icon);
		return (bName && bType && bPrice && bMesh && bIcon);
	}
	bool IsEquipment()
	{
		bool bWeapon = (Type == EItemType::Weapon);
		bool bHelmet = (Type == EItemType::Helmet);
		bool bArmor = (Type == EItemType::Armor);
		bool bNecklace = (Type == EItemType::Necklace);
		bool bRing = (Type == EItemType::Ring);
		bool bShoes = (Type == EItemType::Shoes);
		return (bWeapon || bHelmet || bArmor || bNecklace || bRing || bShoes);
	}

	operator FText()
	{
		FText TypeName;
		switch (Type)
		{
		case EItemType::None:
			TypeName = FText::FromString(TEXT("None"));
			break;
		case EItemType::Weapon:
			TypeName = FText::FromString(TEXT("Weapon"));
			break;
		case EItemType::Helmet:
			TypeName = FText::FromString(TEXT("Helmet"));
			break;
		case EItemType::Armor:
			TypeName = FText::FromString(TEXT("Armor"));
			break;
		case EItemType::Necklace:
			TypeName = FText::FromString(TEXT("Necklace"));
			break;
		case EItemType::Ring:
			TypeName = FText::FromString(TEXT("Ring"));
			break;
		case EItemType::Shoes:
			TypeName = FText::FromString(TEXT("Shoes"));
			break;
		case EItemType::Countable:
			TypeName = FText::FromString(TEXT("Countable"));
			break;
		case EItemType::Etc:
			TypeName = FText::FromString(TEXT("Etc"));
			break;
		default:
			break;
		}
		return TypeName;
	}
};


USTRUCT(BlueprintType, Atomic)
//Atomic : ����ȭ(������ ����Ʈ ������ ��ȯ�Ͽ� ����, ��Ʈ��ũ �ۼ����� ����)
// * ������ ����Ʈ ������ �����Ͱ� �̵��Ѵ�
//BlueprintType : �������Ʈ���� ��� �����ϰ� �������
struct FEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 Level; //��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 Exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	EEnemyType Type;
};

//UCLASS()
//class UTILES_API AStructures : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	AStructures();
//};
