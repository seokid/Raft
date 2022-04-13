// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerItemList.h"
#include "HammerItem.h"
#include "Components/CanvasPanelSlot.h"
#include "../RaftDedicatedGameInstance.h"

void UHammerItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_HammerItemMaxCount = 2;
	
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		m_ArrayHammerItemWidget.Add(Cast<UHammerItem>(GetWidgetFromName(*FString::Printf(TEXT("UI_HammerItem_%d"), i))));
		m_ArrayHammerItemWidget[i]->SetHammerItemListWidget(this);
	}

	m_HammerItemSpacing = 80.f;
	m_HammerItemOffset = -40.f;
}

void UHammerItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHammerItemList::SetHammerItem(const FString& ItemName)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }

	const FCraftingItemTableInfo* Info = GameInst->FindCraftingItemInfo(ItemName);
	if(nullptr == Info) { return; }
	
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		if(Info->ArrayItemParts.Num() > i)
		{
			m_ArrayHammerItemWidget[i]->SetItemPart(Info->ArrayItemParts[i].Name, Info->ArrayItemParts[i].Count);
			m_ArrayHammerItemWidget[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			m_ArrayHammerItemWidget[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	SetHammerItemWidgetPosition(Info->ArrayItemParts.Num());
}

bool UHammerItemList::IsBuildPossible()
{
	bool result = true;
	
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		if(ESlateVisibility::Collapsed != m_ArrayHammerItemWidget[i]->GetVisibility())
		{
			result = result && m_ArrayHammerItemWidget[i]->IsPartEnough();
		}
	}

	return result;
}

void UHammerItemList::Building_Build()
{
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		if(ESlateVisibility::Collapsed != m_ArrayHammerItemWidget[i]->GetVisibility())
		{
			m_ArrayHammerItemWidget[i]->Building_Build();
		}
	}
}

void UHammerItemList::ReloadItemCount()
{
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		if(ESlateVisibility::Collapsed != m_ArrayHammerItemWidget[i]->GetVisibility())
		{
			m_ArrayHammerItemWidget[i]->ReloadItemCount();
		}
	}
}

void UHammerItemList::SetHammerItemWidgetPosition(int32 ItemCount)
{
	for(int32 i = 0; i < m_HammerItemMaxCount; ++i)
	{
		if(ESlateVisibility::Collapsed != m_ArrayHammerItemWidget[i]->GetVisibility())
		{
			UCanvasPanelSlot* HammerItemSlot = Cast<UCanvasPanelSlot>(m_ArrayHammerItemWidget[i]->Slot);
			FVector2D SlotPosition = HammerItemSlot->GetPosition();
			SlotPosition.X = (m_HammerItemSpacing * i) + ((ItemCount - 1) * m_HammerItemOffset);
			HammerItemSlot->SetPosition(SlotPosition);

			m_ArrayHammerItemWidget[i]->ReloadItemCount();
		}
	}
}
