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
	None UMETA(DisplayName = "None"), //없음
	Weapon UMETA(DisplayName = "Weapon"), //무기
	Helmet UMETA(DisplayName = "Helmet"), //헬멧
	Armor UMETA(DisplayName = "Armor"), //갑옷
	Necklace UMETA(DisplayName = "Necklace"), //목걸이
	Ring UMETA(DisplayName = "Ring"), //반지
	Shoes UMETA(DisplayName = "Shoes"), //신발
	Countable UMETA(DisplayName = "Countable"), //소비 아이템 
	Etc UMETA(DisplayName = "Etc") //기타 아이템
};


//UCLASS()
//class UTILES_API AEnums : public AActor
//{
//	GENERATED_BODY()
//public:	
//	AEnums();
//};
