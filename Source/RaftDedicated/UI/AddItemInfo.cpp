// Fill out your copyright notice in the Description page of Project Settings.


#include "AddItemInfo.h"
#include "AddItemList.h"
#include "../RaftDedicatedGameInstance.h"

void UAddItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	m_AddItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("AddItemIconImage")));
	m_AddItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AddItemNameText")));
	m_AddItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AddItemCountText")));

	m_AccTime = 0.f;
	m_MaxTime = 3.f;

	m_Idx = -1;
	m_AddItemListWidget = nullptr;
}

void UAddItemInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ExcuteEnable(InDeltaTime);
}

void UAddItemInfo::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(ESlateVisibility::Collapsed != InVisibility)
	{
		m_AccTime = 0.f;
		m_ExcuteEnable = true;
	}
	else
	{
		m_AccTime = 0.f;
		m_Idx = -1;
		m_ExcuteEnable = false;
	}
}

bool UAddItemInfo::SetAddItemInfo(const FString& ItemName, int32 ItemCount)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) {return false;}
	const FItemIconTableInfo* ItemIconInfo = GameInst->FindItemIconInfo(ItemName);
	if(nullptr == ItemIconInfo) { return false;}

	m_AddItemIconImage->SetBrushFromTexture(ItemIconInfo->IconTexture2D);
	m_AddItemNameText->SetText(FText::FromString(ItemIconInfo->Name));
	m_AddItemCountText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ItemCount)));

	return true;
}

void UAddItemInfo::ExcuteEnable(float InDeltaTime)
{
	if(false == m_ExcuteEnable && ESlateVisibility::Collapsed != GetVisibility()) { return; }
	m_AccTime += InDeltaTime;

	SetColorAndOpacity(FLinearColor(FVector4(1.f, 1.f, 1.f, FMath::Lerp(1.f, 0.f, m_AccTime / m_MaxTime))));
	
	if(m_AccTime >= m_MaxTime)
	{
		m_AddItemListWidget->DisableAddItemInfo(this);
	}
}
