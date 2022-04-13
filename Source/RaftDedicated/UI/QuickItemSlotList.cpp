// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickItemSlotList.h"
#include "ItemSlot.h"
#include "MainHUD.h"
#include "Inventory.h"
#include "Components/CanvasPanelSlot.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

void UQuickItemSlotList::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemSlotListMaxCount = 10;
	m_ItemSlotSelectIdx = 0;

	for(int32 i = 0; i < m_ItemSlotListMaxCount; ++i)
	{
		m_ArrayItemSlotWidget.Add(Cast<UItemSlot>(GetWidgetFromName(*FString::Printf(TEXT("UI_ItemSlot_%d"), i))));
		m_ArrayItemSlotWidget[i]->SetClickedDelegate<UQuickItemSlotList>(this, &UQuickItemSlotList::ItemSlotClickedCallBack);
		m_ArrayItemSlotWidget[i]->SetPressedDelegate<UQuickItemSlotList>(this, &UQuickItemSlotList::ItemSlotPressedCallBack);
		m_ArrayItemSlotWidget[i]->SetReleasedDelegate<UQuickItemSlotList>(this, &UQuickItemSlotList::ItemSlotReleasedCallBack);
		m_ArrayItemSlotWidget[i]->SetHoveredDelegate<UQuickItemSlotList>(this, &UQuickItemSlotList::ItemSlotHoveredCallBack);
		m_ArrayItemSlotWidget[i]->SetLeavedDelegate<UQuickItemSlotList>(this, &UQuickItemSlotList::ItemSlotLeavedCallBack);
	}

	m_ItemSlotSelectImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemSlot_Select")));
}

void UQuickItemSlotList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuickItemSlotList::MoveQuickItemSlotSelect(bool Left)
{
	if(Left) { --m_ItemSlotSelectIdx; }
	else { ++m_ItemSlotSelectIdx; }
	
	if(m_ItemSlotSelectIdx < 0)
	{
		m_ItemSlotSelectIdx = m_ItemSlotListMaxCount - 1;
	}
	else if(m_ItemSlotSelectIdx >= m_ItemSlotListMaxCount)
	{
		m_ItemSlotSelectIdx = 0;
	}
	
	UCanvasPanelSlot* ItemSlot = Cast<UCanvasPanelSlot>(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->Slot);
	if(nullptr == ItemSlot) { return; }
	
	UCanvasPanelSlot* ItemSlotSelect = Cast<UCanvasPanelSlot>(m_ItemSlotSelectImage->Slot);
	if(nullptr == ItemSlotSelect) { return; }
	
	ItemSlotSelect->SetPosition(ItemSlot->GetPosition());

	SetSelectItemToPlayer();
}

bool UQuickItemSlotList::AddItem(const FString& ItemName, int32& ItemCount)
{
	if(ItemCount <= 0) { return true; }
	
	TArray<UItemSlot*>	ArrayItemSlotSame;
	TArray<UItemSlot*>	ArrayItemSlotEmpty;

	// °°Àº ½½·Ô, ºó ½½·Ô Ã£±â
	for(auto ItemSlot : m_ArrayItemSlotWidget)
	{
		if(ItemSlot->IsAddItemPossible(ItemName))
		{
			ArrayItemSlotSame.Add(ItemSlot);
		}
		else if(ItemSlot->IsItemSlotEmpty())
		{
			ArrayItemSlotEmpty.Add(ItemSlot);
		}
	}

	// °°Àº ½½·Ô¿¡ ¸ÕÀú ³Ö±â
	for(auto ItemSlotSame : ArrayItemSlotSame)
	{
		ItemSlotSame->AddItem(ItemName, ItemCount);
		if(ItemCount <= 0) { return true; }
	}

	// ºó ½½·Ô¿¡ ³Ö±â
	for(auto ItemSlotEmpty : ArrayItemSlotEmpty)
	{
		ItemSlotEmpty->AddItem(ItemName, ItemCount);
		if(ItemCount <= 0) { return true; }
	}
	
	return true;
}

bool UQuickItemSlotList::RemoveItem(const FString& ItemName, int32& ItemCount)
{
	if(ItemCount <= 0) { return true; }
	
	// °°Àº ½½·Ô Ã£±â
	for(auto ItemSlot : m_ArrayItemSlotWidget)
	{
		if(ItemCount <= 0) { return true; }
		ItemSlot->RemoveItem(ItemName, ItemCount);
	}
	
	return false;
}

bool UQuickItemSlotList::RemoveItem(const FString& ItemName)
{
	if(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName() == ItemName)
	{
		int ItemCount = 1;
		m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->RemoveItem(ItemName, ItemCount);
		//m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->ResetItemSlot();
		SetSelectItemToPlayer();
		return true;
	}

	return false;
}


int32 UQuickItemSlotList::GetItemCount(const FString& ItemName)
{
	int ItemCount = 0;
	
	for(int32 i =0; i < m_ItemSlotListMaxCount; ++i)
	{
		if(false == m_ArrayItemSlotWidget[i]->IsItemSlotEmpty())
		{
			if(m_ArrayItemSlotWidget[i]->GetItemName() == ItemName)
			{
				ItemCount += m_ArrayItemSlotWidget[i]->GetItemCount();
			}
		}
	}

	return ItemCount;
}

bool UQuickItemSlotList::RemoveSelectItem(const FString& ItemName, int32& ItemCount)
{
	if(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName() == ItemName)
	{
		m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->RemoveItem(ItemName, ItemCount);
		if(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->IsItemSlotEmpty())
		{
			SetSelectItemToPlayer();
		}
		return true;
	}

	return false;
}

const FString& UQuickItemSlotList::GetSelectItemName()
{
	return m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName();
}

int32 UQuickItemSlotList::GetSelectItemCount()
{
	return m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemCount();
}

bool UQuickItemSlotList::IsSelectItemEmpty()
{
	return m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->IsItemSlotEmpty();
}

bool UQuickItemSlotList::ChangeSelectItem(const FString& ItemName, int32& ItemCount, bool ResetSelect)
{
	bool result = true;
	
	if(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName() == ItemName && m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemCount() == ItemCount)
	{
		return result;
	}
	else
	{
		if(false == m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->IsItemSlotEmpty())
		{
			m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->ResetItemSlot();
		}

		result = m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->SetItem(ItemName, ItemCount); 
	}
	
	if(ResetSelect)
	{
		SetSelectItemToPlayer();
	}
	
	return result;
}

void UQuickItemSlotList::ItemSlotClickedCallBack(UItemSlot* ItemSlotClicked, const FPointerEvent& MouseEvent)
{
	LOG(TEXT("UQuickItemSlotList Item Slot Clicked : %s"), *ItemSlotClicked->GetName());
}

void UQuickItemSlotList::ItemSlotPressedCallBack(UItemSlot* ItemSlotPressed, const FPointerEvent& MouseEvent)
{
	LOG(TEXT("UQuickItemSlotList Item Slot Pressed : %s"), *ItemSlotPressed->GetName());

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		m_MainHUDWidget->SetPressedItemSlot(ItemSlotPressed);
	}
	else if(MouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		m_MainHUDWidget->SetPressedItemSlot(ItemSlotPressed, false);
	}
}

void UQuickItemSlotList::ItemSlotReleasedCallBack(UItemSlot* ItemSlotReleased, const FPointerEvent& MouseEvent)
{
	LOG(TEXT("UQuickItemSlotList Item Slot Released : %s"), *ItemSlotReleased->GetName());

	UItemSlot* PressedItemSlot = m_MainHUDWidget->GetPressedItemSlot();
	m_MainHUDWidget->SetReleasedItemSlot(ItemSlotReleased);
	m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(ItemSlotReleased, false);
	
	if(ItemSlotReleased != PressedItemSlot)
	{
		if(ItemSlotReleased == m_ArrayItemSlotWidget[m_ItemSlotSelectIdx])
		{
			SetSelectItemToPlayer();
		}
	}
	//m_MainHUDWidget->ExcuteItemslotDragAndDropCount();
}

void UQuickItemSlotList::ItemSlotHoveredCallBack(UItemSlot* ItemSlotHovered, const FPointerEvent& MouseEvent)
{
	m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(ItemSlotHovered, false);
}

void UQuickItemSlotList::ItemSlotLeavedCallBack(UItemSlot* ItemSlotLeaved, const FPointerEvent& MouseEvent)
{
	m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(ItemSlotLeaved, false);
}

void UQuickItemSlotList::SetSelectItemToPlayer()
{
	EItemType Type = m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemType();

	switch (Type)
	{
	case EItemType::None:
		{
			m_MainHUDWidget->GetPlayer()->SetBuildEnable(false);
			m_MainHUDWidget->GetPlayer()->UnEquipmentItem();
			LOG(TEXT("None"));
			break;
		}
	case EItemType::Part:
		{
			m_MainHUDWidget->GetPlayer()->SetBuildEnable(false);
			m_MainHUDWidget->GetPlayer()->UnEquipmentItem();
			LOG(TEXT("Part"));
			break;
		}
	case EItemType::Equipment:
		{
			m_MainHUDWidget->GetPlayer()->SetBuildEnable(false);
			m_MainHUDWidget->GetPlayer()->UnEquipmentItem();
			m_MainHUDWidget->GetPlayer()->OnEquipmentItem(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName());
			LOG(TEXT("Equipment"));
			break;
		}
	case EItemType::Build_Install:
		{
			m_MainHUDWidget->GetPlayer()->UnEquipmentItem();
			m_MainHUDWidget->GetPlayer()->SetBuildEnable(true);
			m_MainHUDWidget->GetPlayer()->SetBuildName(m_ArrayItemSlotWidget[m_ItemSlotSelectIdx]->GetItemName());
			LOG(TEXT("Build_Install"));
			break;
		}
	}
}


