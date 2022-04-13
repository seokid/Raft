// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingItemInfo.h"
#include "CraftingItemPart.h"
#include "CraftingList.h"
#include "MainHUD.h"
#include "NotebookCrafting.h"
#include "../RaftDedicatedGameInstance.h"
#include "Components/CanvasPanelSlot.h"

void UCraftingItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	m_CraftingItemPartMaxCount = 6;

	m_CraftingItemPartOffset = 300;
	m_CraftingItemPartSpacing = 80;

	m_CraftingBoderPressed = false;
	m_CraftingEnable = false;
	
	m_BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	m_CraftingItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("CraftingItemIconImage")));
	m_CraftingItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CraftingItemNameText")));
	m_CraftingItemDescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CraftingItemDescText")));
	m_CraftingBorder = Cast<UBorder>(GetWidgetFromName(TEXT("CraftingBorder")));

	m_CraftingBorder->OnMouseButtonDownEvent.BindDynamic(this, &UCraftingItemInfo::OnCraftingBorderPressed);
	m_CraftingBorder->OnMouseButtonUpEvent.BindDynamic(this, &UCraftingItemInfo::OnCraftingBorderReleased);
	m_CraftingBorder->OnMouseMoveEvent.BindDynamic(this, &UCraftingItemInfo::OnCraftingBorderHovered);

	for(int32 i = 0; i < m_CraftingItemPartMaxCount; ++i)
	{
		m_ArrayCraftingItemPartWidget.Add(Cast<UCraftingItemPart>(GetWidgetFromName(*FString::Printf(TEXT("UI_CraftingItemParts_%d"), i))));
	}
}

void UCraftingItemInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCraftingItemInfo::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	m_CraftingBoderPressed = false;
}

FEventReply UCraftingItemInfo::OnCraftingBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		m_CraftingBoderPressed = true;
	}
	
	return result;
}

FEventReply UCraftingItemInfo::OnCraftingBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		if(m_CraftingBoderPressed)
		{
			ExcuteCraftingItem();
			m_CraftingBoderPressed = false;
		}
	}
	
	return result;
}

FEventReply UCraftingItemInfo::OnCraftingBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;
	return result;
}

void UCraftingItemInfo::SetCraftingItemName(const FString& CraftingItemName)
{
	m_CraftingItemName = CraftingItemName;
	SetCraftingItemInfo();
}

void UCraftingItemInfo::SetCraftingItemInfo()
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }

	const FCraftingItemTableInfo* ItemInfo = GameInst->FindCraftingItemInfo(m_CraftingItemName);
	const FItemIconTableInfo* ItemIconInfo = GameInst->FindItemIconInfo(m_CraftingItemName);
	
	if(nullptr == ItemInfo || nullptr == ItemIconInfo)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	SetCraftingItemIconImage(ItemIconInfo->IconTexture2D);
	SetCraftingItemNameText(ItemIconInfo->Name);
	SetCraftingItemDescText(ItemIconInfo->Desc);
	
	SetCraftingItemParts(ItemInfo->ArrayItemParts);
	SetCraftingEnable();
}

void UCraftingItemInfo::SetCraftingItemIconImage(UTexture2D* CraftingItemIconTexture)
{
	m_CraftingItemIconImage->SetBrushFromTexture(CraftingItemIconTexture);
}

void UCraftingItemInfo::SetCraftingItemNameText(const FString& CraftingItemDesc)
{
	m_CraftingItemNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *CraftingItemDesc)));
}

void UCraftingItemInfo::SetCraftingItemDescText(const FString& CraftingItemDesc)
{
	m_CraftingItemDescText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *CraftingItemDesc)));
}

void UCraftingItemInfo::SetCraftingItemParts(const TArray<FCraftingItemParts>& ArrayParts)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }

	m_CraftingEnable = true;

	for(int32 i = 0; i < m_CraftingItemPartMaxCount; ++i)
	{
		if(i < ArrayParts.Num())
		{
			int32 PlayerItemCount = m_CraftingListWidget->GetNotebookCraftingWidget()->GetMainHudWidget()->GetItemCount(ArrayParts[i].Name);
			if(false == m_ArrayCraftingItemPartWidget[i]->SetCraftingItemPart(ArrayParts[i].Name, PlayerItemCount, ArrayParts[i].Count))
			{
				m_CraftingEnable = false;
			}

			m_ArrayCraftingItemPartWidget[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			m_ArrayCraftingItemPartWidget[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	SetBackgroundSize(ArrayParts.Num());
}

void UCraftingItemInfo::SetBackgroundSize(int32 PartCount)
{
	UCanvasPanelSlot* BackgroundSlot = Cast<UCanvasPanelSlot>(m_BackgroundImage->Slot);
	FVector2D Size = BackgroundSlot->GetSize();
	Size.Y = m_CraftingItemPartOffset + m_CraftingItemPartSpacing * PartCount;
	BackgroundSlot->SetSize(Size);
}

void UCraftingItemInfo::SetCraftingEnable()
{
	if(m_CraftingEnable)
	{
		//if(!m_CraftButtonNormalTexture)
		{
			m_CraftingBorder->SetBrushFromTexture(m_CraftButtonNormalTexture);
		}
	}
	else
	{
		//if(!m_CraftButtonDisabledTexture)
		{
			m_CraftingBorder->SetBrushFromTexture(m_CraftButtonDisabledTexture);
		}
	}
}

void UCraftingItemInfo::ExcuteCraftingItem()
{
	if(false == m_CraftingEnable)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), m_CraftingListWidget->GetNotebookCraftingWidget()->GetMainHudWidget()->GetUISound(EUISoundType::ButtonFailClicked));
		return;
	}

	LOG(TEXT("ExcuteCraftingItem"));
	TArray<int32> PartCount;
	TArray<FString> PartName;
	
	for(int32 i = 0; i < m_CraftingItemPartMaxCount; ++i)
	{
		if(ESlateVisibility::Collapsed != m_ArrayCraftingItemPartWidget[i]->GetVisibility())
		{
			if(!m_ArrayCraftingItemPartWidget[i]->IsPartEnough())
			{
				UGameplayStatics::PlaySound2D(GetWorld(), m_CraftingListWidget->GetNotebookCraftingWidget()->GetMainHudWidget()->GetUISound(EUISoundType::ButtonFailClicked));
				return;
			}

			PartName.Add(m_ArrayCraftingItemPartWidget[i]->GetPartName());
			PartCount.Add(m_ArrayCraftingItemPartWidget[i]->GetPartCount());
		}
	}


	LOG(TEXT("Item Part Remove"));
	for(int32 i = 0; i < PartCount.Num(); ++i)
	{
		m_CraftingListWidget->GetNotebookCraftingWidget()->GetMainHudWidget()->RemoveItem(PartName[i], PartCount[i]);
	}
	
	LOG(TEXT("Item Crafting"));
	int ItemCount = 1;
	m_CraftingListWidget->GetNotebookCraftingWidget()->GetMainHudWidget()->AddItem(m_CraftingItemName, ItemCount);
	SetCraftingItemInfo();

	UGameplayStatics::PlaySound2D(GetWorld(), m_CraftingSound);
}