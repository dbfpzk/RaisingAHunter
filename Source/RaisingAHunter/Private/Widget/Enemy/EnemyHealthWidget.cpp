// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Enemy/EnemyHealthWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHealthWidget::SetHealth(float CurrnetHp, float MaxHp)
{
	HealthBar->SetPercent(CurrnetHp / MaxHp);
}
