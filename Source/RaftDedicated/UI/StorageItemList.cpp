// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageItemList.h"
#include "MainHUD.h"
#include "Inventory.h"
#include "ItemSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "RaftDedicated/Item/Building/Storage.h"
#include "../RaftDedicatedGameInstance.h"

void UStorageItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemSlotMaxCount = 30;
	m_SelectItemSlot = nullptr;
	m_TargetStorage = nullptr;

	m_CraftingItemOffset = 120;
	m_CraftingItemSpacing = 100;

	m_LineToItemSlotMaxCount = 5;

	m_BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Background")));

	for(int32 i = 0; i < m_ItemSlotMaxCount; ++i)
	{
		m_ArrayItemSlotWidget.Add(Cast<UItemSlot>(GetWidgetFromName(*FString::Printf(TEXT("UI_ItemSlot_%d"), i))));
		m_ArrayItemSlotWidget[i]->SetClickedDelegate<UStorageItemList>(this, &UStorageItemList::ItemSlotClickedCallBack);
		m_ArrayItemSlotWidget[i]->SetPressedDelegate<UStorageItemList>(this, &UStorageItemList::ItemSlotPressedCallBack);
		m_ArrayItemSlotWidget[i]->SetReleasedDelegate<UStorageItemList>(this, &UStorageItemList::ItemSlotReleasedCallBack);
		m_ArrayItemSlotWidget[i]->SetHoveredDelegate<UStorageItemList>(this, &UStorageItemList::ItemSlotHoveredCallBack);
		m_ArrayItemSlotWidget[i]->SetLeavedDelegate<UStorageItemList>(this, &UStorageItemList::ItemSlotLeavedCallBack);
	}
	m_StorageIconImage = Cast<UImage>(GetWidgetFromName(TEXT("StorageIconImage")));
	m_ItemSlotSelectImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemSlot_Select")));
}

void UStorageItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStorageItemList::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(ESlateVisibility::Collapsed == InVisibility)
	{
		DisableStorageInfo();
	}
	else
	{
		EnableStorageInfo();
	}
}

void UStorageItemList::ItemSlotClickedCallBack(UItemSlot* ItemSlotClicked, const FPointerEvent& MouseEvent)
{
}

void UStorageItemList::ItemSlotPressedCallBack(UItemSlot* ItemSlotPressed, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		m_MainHUDWidget->SetPressedItemSlot(ItemSlotPressed);
	}
	else if(MouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		m_MainHUDWidget->SetPressedItemSlot(ItemSlotPressed, false);
	}
}

void UStorageItemList::ItemSlotReleasedCallBack(UItemSlot* ItemSlotReleased, const FPointerEvent& MouseEvent)
{
	m_MainHUDWidget->SetReleasedItemSlot(ItemSlotReleased);
	SetSelectItemSlot(nullptr);
	//m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(nullptr, false);
}

void UStorageItemList::ItemSlotHoveredCallBack(UItemSlot* ItemSlotHovered, const FPointerEvent& MouseEvent)
{
	//m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(ItemSlotHovered, false);
	SetSelectItemSlot(ItemSlotHovered);
	//SetItemSlotSelectImagePosition();
}

void UStorageItemList::ItemSlotLeavedCallBack(UItemSlot* ItemSlotLeaved, const FPointerEvent& MouseEvent)
{
	SetSelectItemSlot(nullptr);
	//m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(nullptr, false);
}

void UStorageItemList::SetStorageItemMaxCount(int32 ItemMaxCount)
{
}

void UStorageItemList::SetSelectItemSlot(UItemSlot* SelectItemSlot)
{
	if(SelectItemSlot == m_SelectItemSlot) { return; }
	m_SelectItemSlot = SelectItemSlot;

	m_MainHUDWidget->GetInventoryWidget()->SetSelectItemSlot(m_SelectItemSlot, false);

	SetItemSlotSelectImagePosition();
}

void UStorageItemList::SetItemSlotSelectImagePosition()
{
	if(nullptr == m_SelectItemSlot)
	{
		m_ItemSlotSelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 0)));
		return;
	}
	
	UCanvasPanelSlot* ItemSlot = Cast<UCanvasPanelSlot>(m_SelectItemSlot->Slot);
	if(nullptr == ItemSlot) { return; }
	
	UCanvasPanelSlot* ItemSlotSelect = Cast<UCanvasPanelSlot>(m_ItemSlotSelectImage->Slot);
	if(nullptr == ItemSlotSelect) { return; }

	m_ItemSlotSelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	ItemSlotSelect->SetPosition(ItemSlot->GetPosition());
}

void UStorageItemList::EnableStorageInfo()
{
	if(nullptr == m_TargetStorage) {return;}
	LOG(TEXT("%d"), m_TargetStorage->GetItemSlotMaxCount());

	SetItemSlotWidgetFromStorage();
	SetBackgroundSize();
	SetStorageIconImage();
}

void UStorageItemList::DisableStorageInfo()
{
	if(nullptr == m_TargetStorage) {return;}
	SetItemSlotWidgetToStorage();
	m_TargetStorage = nullptr;
}

void UStorageItemList::SetItemSlotWidgetFromStorage()
{
	int32 ItemSlotMaxCount = m_TargetStorage->GetItemSlotMaxCount();

	TArray<FString>& ArrayItemName = m_TargetStorage->GetArrayItemName();
	TArray<int32>& ArrayItemCount = m_TargetStorage->GetArrayItemCount();
	
	for(int32 i = 0; i < ItemSlotMaxCount; ++i)
	{
		m_ArrayItemSlotWidget[i]->SetVisibility(ESlateVisibility::Visible);
		if(false == ArrayItemName[i].IsEmpty() && ArrayItemCount[i] > 0)
		{
			m_ArrayItemSlotWidget[i]->SetItem(ArrayItemName[i], ArrayItemCount[i]);
		}
	}
	
	for(int32 i = ItemSlotMaxCount; i < m_ItemSlotMaxCount; ++i)
	{
		m_ArrayItemSlotWidget[i]->SetVisibility(ESlateVisibility::Collapsed);
		m_ArrayItemSlotWidget[i]->ResetItemSlot();
	}
}

void UStorageItemList::SetItemSlotWidgetToStorage()
{
	int32 ItemSlotMaxCount = m_TargetStorage->GetItemSlotMaxCount();

	TArray<FString>& ArrayItemName = m_TargetStorage->GetArrayItemName();
	TArray<int32>& ArrayItemCount = m_TargetStorage->GetArrayItemCount();

	for(int32 i = 0; i < ItemSlotMaxCount; ++i)
	{
		ArrayItemName[i] = m_ArrayItemSlotWidget[i]->GetItemName();
		ArrayItemCount[i] = m_ArrayItemSlotWidget[i]->GetItemCount();
		m_ArrayItemSlotWidget[i]->SetVisibility(ESlateVisibility::Collapsed);
		m_ArrayItemSlotWidget[i]->ResetItemSlot();
	}
	
	for(int32 i = ItemSlotMaxCount; i < m_ItemSlotMaxCount; ++i)
	{
		m_ArrayItemSlotWidget[i]->SetVisibility(ESlateVisibility::Collapsed);
		m_ArrayItemSlotWidget[i]->ResetItemSlot();
	}
}

void UStorageItemList::SetBackgroundSize()
{
	int32 ItemSlotMaxCount = m_TargetStorage->GetItemSlotMaxCount();
	
	UCanvasPanelSlot* BackgroundSlot = Cast<UCanvasPanelSlot>(m_BackgroundImage->Slot);
	FVector2D Size = BackgroundSlot->GetSize();

	int32 LineCount = ItemSlotMaxCount / m_LineToItemSlotMaxCount;
	LineCount += (ItemSlotMaxCount % m_LineToItemSlotMaxCount) == 0 ? 0 : 1;
	Size.Y = m_CraftingItemOffset + m_CraftingItemSpacing * LineCount;
	BackgroundSlot->SetSize(Size);
}

void UStorageItemList::SetStorageIconImage()
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }
	const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(m_TargetStorage->GetBuildingInfo().m_Name);
	if(nullptr == Info) { return;}
	
	m_StorageIconImage->SetBrushFromTexture(Info->IconTexture2D);
}
