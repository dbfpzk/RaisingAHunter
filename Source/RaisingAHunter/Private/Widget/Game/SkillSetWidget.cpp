// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Game/SkillSetWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Hunter/Unit.h"

USkillSetWidget::USkillSetWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillMatInstance.Init(nullptr, 3);
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> SkillMatAsset1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Skill/MAT_Skill1.MAT_Skill1'"));
	if (SkillMatAsset1.Succeeded())
	{
		SkillMatInstance[0] = SkillMatAsset1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> SkillMatAsset2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Skill/MAT_Skill2.MAT_Skill2'"));
	if (SkillMatAsset2.Succeeded())
	{
		SkillMatInstance[1] = SkillMatAsset2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> SkillMatAsset3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Skill/MAT_Skill3.MAT_Skill3'"));
	if (SkillMatAsset3.Succeeded())
	{
		SkillMatInstance[2] = SkillMatAsset3.Object;
	}

	isSkills.Init(false, 3);
	SkillMatDynamic.Init(nullptr, 3);
	SkillMaxCoolTimes.Add(0.5f);
	SkillMaxCoolTimes.Add(0.5f);
	SkillMaxCoolTimes.Add(1.0f);
}

void USkillSetWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < SkillMatInstance.Num(); i++)
	{
		SkillMatDynamic[i] = UMaterialInstanceDynamic::Create(SkillMatInstance[i], this);
	}

	if (SkillMatDynamic[0] != nullptr)
	{
		SkillImage1->SetBrushFromMaterial(SkillMatDynamic[0]);
	}
	if (SkillMatDynamic[1] != nullptr)
	{
		SkillImage2->SetBrushFromMaterial(SkillMatDynamic[1]);
	}
	if (SkillMatDynamic[2] != nullptr)
	{
		SkillImage3->SetBrushFromMaterial(SkillMatDynamic[2]);
	}

	Init();
}

void USkillSetWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SkillCoolTime(InDeltaTime);
}

void USkillSetWidget::SkillCoolTime(const float& InDeltaTime)
{
	for (int32 i = 0; i < isSkills.Num(); i++)
	{
		if (isSkills[i])
		{
			float Current;
			SkillMatDynamic[i]->GetScalarParameterValue(TEXT("percent"), Current);
			Current += InDeltaTime;
			SkillMatDynamic[i]->SetScalarParameterValue(TEXT("percent"), Current);
			if (Current >= SkillMaxCoolTimes[i])
			{
				isSkills[i] = false;
				SkillMatDynamic[i]->SetScalarParameterValue(TEXT("percent"), 0);
			}
		}
	}
}

void USkillSetWidget::Init()
{
	AUnit* Unit = Cast<AUnit>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Unit))
	{
		Unit->OnGroundAttackSignature.AddLambda([&]()
			{
				isSkills[0] = true;
				SkillMatDynamic[0]->SetScalarParameterValue(TEXT("percent"), 0);
			}
		);
		Unit->OnAirAttackSignature.AddLambda([&]()
			{
				isSkills[1] = true;
				SkillMatDynamic[1]->SetScalarParameterValue(TEXT("percent"), 0);
			}
		);
		Unit->OnProjectileAttackSignature.AddLambda([&]()
			{
				isSkills[2] = true;
				SkillMatDynamic[2]->SetScalarParameterValue(TEXT("percent"), 0);
			}
		);
	}
}
