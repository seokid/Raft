// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingItemPart.h"
#include "../RaftDedicatedGameInstance.h"

void UCraftingItemPart::NativeConstruct()
{
	Super::NativeConstruct();
	m_CraftingItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("CraftingItemIconImage")));
	m_CraftingItemTextBacbgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("CraftingItemName_BG")));
	m_CraftingItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PartNameText")));
	m_CraftingItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PartCountText")));

	m_PartFull = false;
	m_PartCount = 0;

	m_CraftingDisableOpacity = 0.5f;
	m_CraftingDisableColor = FVector4(1.f, 0.3f, 0.3f, 1.f);
}

void UCraftingItemPart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

bool UCraftingItemPart::SetCraftingItemPart(const FString& Name, int32 PlayerItemCount, int32 ItemPartCount)
{
	if(false == SetCraftingItemNameText(Name)) { return false; }
	if(false == SetCraftingItemCount(PlayerItemCount, ItemPartCount)) { return false; }
	return true;
}

bool UCraftingItemPart::SetCraftingItemNameText(const FString& Name)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return false; }

	const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(Name);
	if(nullptr == Info) { return false; }

	SetCraftingItemIconImage(Info->IconTexture2D);

	m_CraftingItemNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Info->Name)));
	m_PartName = Name;

	return true;
}

void UCraftingItemPart::SetCraftingItemIconImage(UTexture2D* ImageTexture)
{
	m_CraftingItemIconImage->SetBrushFromTexture(ImageTexture);
}

bool UCraftingItemPart::SetCraftingItemCount(int32 PlayerItemCount, int32 ItemPartCount)
{
	m_CraftingItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), PlayerItemCount, ItemPartCount)));
	m_PartCount = ItemPartCount;
	
	if(PlayerItemCount >= ItemPartCount)
	{
		m_PartFull = true;
	}
	else
	{
		m_PartFull = false;
	}

	SetPartEnough();
	
	return m_PartFull;
}

void UCraftingItemPart::SetPartEnough()
{
	if(m_PartFull)
	{
		m_CraftingItemIconImage->SetOpacity(1.f);
		m_CraftingItemTextBacbgroundImage->SetColorAndOpacity(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f)));
	}
	else
	{
		m_CraftingItemIconImage->SetOpacity(m_CraftingDisableOpacity);
		m_CraftingItemTextBacbgroundImage->SetColorAndOpacity(FLinearColor(m_CraftingDisableColor));
	}
}

