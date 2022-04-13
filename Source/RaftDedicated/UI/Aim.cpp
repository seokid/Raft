// Fill out your copyright notice in the Description page of Project Settings.


#include "Aim.h"


void UAim::NativeConstruct()
{
	Super::NativeConstruct();
	
    m_ChargingGauge = Cast<UImage>(GetWidgetFromName(TEXT("ChargingGauge")));
}

void UAim::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UAim::SetCharigngGauge(float Percent)
{
	m_ChargingGauge->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), Percent);
}