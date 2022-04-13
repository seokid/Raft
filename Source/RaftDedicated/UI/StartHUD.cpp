// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUD.h"
#include "StartButtonList.h"
#include "DevelopmentList.h"

void UStartHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartButtonListWidget = Cast<UStartButtonList>(GetWidgetFromName(TEXT("UI_StartButtonList")));
	m_DevelopmentListWidget = Cast<UDevelopmentList>(GetWidgetFromName(TEXT("UI_DevelopmentList")));

	m_StartButtonListWidget->SetStartHUD(this);
}

void UStartHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartHUD::DevelopmentListEnable()
{
	if(ESlateVisibility::Collapsed == m_DevelopmentListWidget->GetVisibility())
	{
		m_DevelopmentListWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
