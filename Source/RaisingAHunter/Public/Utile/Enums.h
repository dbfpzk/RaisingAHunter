// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Hunter UMETA(DisplayName = "Hunter"),
	Huntress UMETA(DisplayName = "Huntress")
};

UENUM(BlueprintType)
enum class EUnitStateType : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Jump UMETA(DisplayName = "Jump"),
	Dash UMETA(DisplayName = "Dash"),
	Attack UMETA(DisplayName = "Attack"),
	Skill UMETA(DisplayName = "Skill"),
	Die UMETA(DisplayName = "Die")
};

UENUM(BlueprintType)
enum class EBossStateType : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Melee UMETA(DisplayName = "Melee"),
	Missile UMETA(DisplayName = "Missile"),
	Meteo UMETA(DisplayName = "Meteo"),
	Dash UMETA(DisplayName = "Dash")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Gun UMETA(DisplayName = "Gun"),
	Projectile UMETA(DisplayName = "Projectile"),
	Boss UMETA(DisplayName = "Boss")
};


UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"), //����
	Weapon UMETA(DisplayName = "Weapon"), //����
	Helmet UMETA(DisplayName = "Helmet"), //���
	Armor UMETA(DisplayName = "Armor"), //����
	Necklace UMETA(DisplayName = "Necklace"), //�����
	Ring UMETA(DisplayName = "Ring"), //����
	Shoes UMETA(DisplayName = "Shoes"), //�Ź�
	Countable UMETA(DisplayName = "Countable"), //�Һ� ������ 
	Etc UMETA(DisplayName = "Etc") //��Ÿ ������
};


//UCLASS()
//class UTILES_API AEnums : public AActor
//{
//	GENERATED_BODY()
//public:	
//	AEnums();
//};
