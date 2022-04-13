// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerItem.h"
#include "HammerItemList.h"
#include "MainHUD.h"
#include "../RaftDedicatedGameInstance.h"

void UHammerItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	m_ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIconImage")));
	m_ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCountText")));

	m_PartFull = false;
}

void UHammerItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHammerItem::SetItemPart(const FString& ItemName, int32 ItemCount)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }

	const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(ItemName);
	if(nullptr == Info) { return; }

	m_ItemPartName = ItemName;
	m_ItemPartCount = ItemCount;
	
	SetItemIcon(Info->IconTexture2D);

	ReloadItemCount();
}

void UHammerItem::Building_Build()
{
	int32 ItemCount = m_ItemPartCount;
	m_HammerItemListWidget->GetMainHUDWidget()->RemoveItem(m_ItemPartName, ItemCount);
}

void UHammerItem::ReloadItemCount()
{
	int32 PlayerItemCount = m_HammerItemListWidget->GetMainHUDWidget()->GetItemCount(m_ItemPartName);
	SetItemCount(PlayerItemCount, m_ItemPartCount);
	m_PartFull = PlayerItemCount >= m_ItemPartCount ? true : false;
	SetBackground();
	SetPartFull();
}

void UHammerItem::SetBackground()
{
	if(m_PartFull)
	{
		m_BackgroundImage->SetBrushFromTexture(m_Background_Bright);
	}
	else
	{
		m_BackgroundImage->SetBrushFromTexture(m_Background_Red);
	}
}

void UHammerItem::SetItemIcon(UTexture2D* ItemIconTexture)
{
	m_ItemIconImage->SetBrushFromTexture(ItemIconTexture);
}

void UHammerItem::SetItemCount(int32 PlayerItemCount, int32 ItemCount)
{
	m_ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), PlayerItemCount, ItemCount)));
}

void UHammerItem::SetPartFull()
{
	if(m_PartFull)
	{
		m_ItemIconImage->SetOpacity(1.f);
		m_ItemCountText->SetColorAndOpacity(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f)));
	}
	else
	{
		m_ItemIconImage->SetOpacity(0.5f);
		m_ItemCountText->SetColorAndOpacity(FLinearColor(FVector4(0.5f, 0.5f, 0.5f, 1.f)));
	}
}
