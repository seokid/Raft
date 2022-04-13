// Fill out your copyright notice in the Description page of Project Settings.


#include "DevelopmentList.h"

void UDevelopmentList::NativeConstruct()
{
	Super::NativeConstruct();

	m_ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
	m_ExitButton->OnClicked.AddDynamic(this, &UDevelopmentList::ExitButtonClicked);
}

void UDevelopmentList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDevelopmentList::ExitButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
