// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteraction.h"

void UItemInteraction::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_InteractionButtonText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InteractionButtonText")));
	m_ItemDescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemDescText")));
}

void UItemInteraction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
