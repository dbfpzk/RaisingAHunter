// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType, Atomic)
//Atomic : 직렬화(내용을 바이트 단위로 변환하여 파일, 네트워크 송수신이 가능)
// * 서버는 바이트 단위로 데이터가 이동한다
//BlueprintType : 블루프린트에서 사용 가능하게 만들어줌
struct FUnitStatus : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Level; //레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Strength; //힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Agility; //민첩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Intellect; //지능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 Luck; //행운
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Damage; //공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Defence; //방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHp; //최대 Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMp; //최대 MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CriticalDamage; //크리티컬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CiriticalPercent; //크리티컬 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 NextExp; //필요 경험치량
};

USTRUCT(BlueprintType, Atomic)
//Atomic : 직렬화(내용을 바이트 단위로 변환하여 파일, 네트워크 송수신이 가능)
// * 서버는 바이트 단위로 데이터가 이동한다
//BlueprintType : 블루프린트에서 사용 가능하게 만들어줌
struct FReinforceLevel : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 StrengthLevel; //힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 AgilityLevel; //민첩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 IntellectLevel; //지능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 LuckLevel; //행운
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 DamageLevel; //공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 DefenceLevel; //방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 MaxHpLevel; //최대 Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 MaxMpLevel; //최대 MP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 CriticalDamageLevel; //크리티컬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 CiriticalPercentLevel; //크리티컬 확률
};

USTRUCT(BlueprintType, Atomic)
//Atomic : 직렬화(내용을 바이트 단위로 변환하여 파일, 네트워크 송수신이 가능)
// * 서버는 바이트 단위로 데이터가 이동한다
//BlueprintType : 블루프린트에서 사용 가능하게 만들어줌
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
	bool bStack; //쌓을 수 있는지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<class USkeletalMesh> ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<class UTexture2D> Icon;
public: //연산자 오버로딩
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
//Atomic : 직렬화(내용을 바이트 단위로 변환하여 파일, 네트워크 송수신이 가능)
// * 서버는 바이트 단위로 데이터가 이동한다
//BlueprintType : 블루프린트에서 사용 가능하게 만들어줌
struct FEnemyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 Level; //힘
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
