// Fill out your copyright notice in the Description page of Project Settings.


#include "BreathBar.h"

void UBreathBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_BreathGauge = Cast<UImage>(GetWidgetFromName(TEXT("BreathGauge")));
}

void UBreathBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBreathBar::SetBreathGauge(float Percent)
{
	m_BreathGauge->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), Percent);
}