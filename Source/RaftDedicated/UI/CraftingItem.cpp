// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingItem.h"

#include "CraftingList.h"

void UCraftingItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_CraftingItemIconImage = Cast<UBorder>(GetWidgetFromName(TEXT("CraftingItemIconImage")));
	m_CraftingItemName_BG = Cast<UImage>(GetWidgetFromName(TEXT("CraftingItemName_BG")));
	m_CraftingItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CraftingItemNameText")));
	m_CraftingListWidget = nullptr;
}

void UCraftingItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCraftingItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(nullptr != m_CraftingListWidget)
	{
		m_CraftingListWidget->OnCraftingItemMouseEnter(this);
	}
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCraftingItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if(nullptr != m_CraftingListWidget)
	{
		m_CraftingListWidget->OnCraftingItemMouseLeave(this);
	}
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UCraftingItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		if(nullptr != m_CraftingListWidget)
		{
			m_CraftingListWidget->OnCraftingItemMouseDown(this);
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCraftingItem::SetCraftingItem(const FItemIconTableInfo* Info)
{
	if(nullptr == Info) {return;}
	m_CraftingItemIconImage->SetBrushFromTexture(Info->IconTexture2D);
	m_CraftingItemNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Info->Name)));
	m_CraftingItemName = Info->Name;
}
