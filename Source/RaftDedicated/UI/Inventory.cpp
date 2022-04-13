// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "ItemSlot.h"
#include "MainHUD.h"
#include "Components/CanvasPanelSlot.h"
#include "../RaftDedicatedGameInstance.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemSlotMaxCount = 15;

	for(int32 i = 0; i < m_ItemSlotMaxCount; ++i)
	{
		m_ArrayItemSlotWidget.Add(Cast<UItemSlot>(GetWidgetFromName(*FString::Printf(TEXT("UI_ItemSlot_%d"), i))));
		m_ArrayItemSlotWidget[i]->SetClickedDelegate<UInventory>(this, &UInventory::ItemSlotClickedCallBack);
		m_ArrayItemSlotWidget[i]->SetPressedDelegate<UInventory>(this, &UInventory::ItemSlotPressedCallBack);
		m_ArrayItemSlotWidget[i]->SetReleasedDelegate<UInventory>(this, &UInventory::ItemSlotReleasedCallBack);
		m_ArrayItemSlotWidget[i]->SetHoveredDelegate<UInventory>(this, &UInventory::ItemSlotHoveredCallBack);
		m_ArrayItemSlotWidget[i]->SetLeavedDelegate<UInventory>(this, &UInventory::ItemSlotLeavedCallBack);
	}
	
	m_SelectItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("SelectItemIconImage")));
	m_ItemSlotSelectImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemSlot_Select")));

	m_SelectItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SelectItemNameText")));
	m_SelectItemDescText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SelectItemDescText")));

	m_SelectItemSlot = nullptr;
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventory::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	SetSelectItemSlot(nullptr);
}

void UInventory::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetSelectItemSlot(nullptr);
}

bool UInventory::AddItem(const FString& ItemName, int32& ItemCount)
{
	if(ItemCount <= 0) { return true; }
	
	TArray<UItemSlot*>	ArrayItemSlotSame;
	TArray<UItemSlot*>	ArrayItemSlotEmpty;

	// ∞∞¿∫ ΩΩ∑‘, ∫Û ΩΩ∑‘ √£±‚
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

	// ∞∞¿∫ ΩΩ∑‘ø° ∏’¿˙ ≥÷±‚
	for(auto ItemSlotSame : ArrayItemSlotSame)
	{
		ItemSlotSame->AddItem(ItemName, ItemCount);
		if(ItemCount <= 0) { return true; }
	}

	// ∫Û ΩΩ∑‘ø° ≥÷±‚
	for(auto ItemSlotEmpty : ArrayItemSlotEmpty)
	{
		ItemSlotEmpty->AddItem(ItemName, ItemCount);
		if(ItemCount <= 0) { return true; }
	}
	
	return false;
}

bool UInventory::RemoveItem(const FString& ItemName, int32& ItemCount)
{
	if(ItemCount <= 0) { return true; }
	
	// ∞∞¿∫ ΩΩ∑‘ √£±‚
	for(auto ItemSlot : m_ArrayItemSlotWidget)
	{
		if(ItemCount <= 0) { return true; }
		ItemSlot->RemoveItem(ItemName, ItemCount);
	}
	
	return false;
}

void UInventory::SetSelectItemSlot(UItemSlot* SelectItemSlot, bool IsItemSlotInInventory)
{
	if(SelectItemSlot == m_SelectItemSlot) { return; }
	m_SelectItemSlot = SelectItemSlot;

	if(nullptr == m_SelectItemSlot)
	{
		ClearItemSlotSelectImage();
		ClearSelectItemSlotInfo();
		return;
	}

	if(nullptr != m_MainHUDWidget->GetPressedItemSlot() || false == m_SelectItemSlot->IsItemSlotEmpty())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), m_MainHUDWidget->GetUISound(EUISoundType::Hovered));
	}
	
	// Show Select Item Info
	ShowSelectItemInfo();
	
	// SelectImage ¿Ãµø
	if(IsItemSlotInInventory)
	{
		SetItemSlotSelectImagePosition();		
	}
	else
	{
		ClearItemSlotSelectImage();
	}
}

int32 UInventory::GetItemCount(const FString& ItemName)
{
	int ItemCount = 0;
	
	for(int32 i =0; i < m_ItemSlotMaxCount; ++i)
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

void UInventory::ItemSlotClickedCallBack(UItemSlot* ItemSlotClicked, const FPointerEvent& MouseEvent)
{
	//SetSelectItemSlot(ItemSlotClicked);
	LOG(TEXT("Inventory Item Slot Clicked : %s"), *ItemSlotClicked->GetName());
	
	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		LOG(TEXT("Inventory Item Slot Clicked : LeftMouseButton"));
	}
	else if(MouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		LOG(TEXT("Inventory Item Slot Clicked : RightMouseButton"));
	}
}

void UInventory::ItemSlotPressedCallBack(UItemSlot* ItemSlotPressed, const FPointerEvent& MouseEvent)
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

void UInventory::ItemSlotReleasedCallBack(UItemSlot* ItemSlotReleased, const FPointerEvent& MouseEvent)
{
	LOG(TEXT("Inventory Item Slot Released : %s"), *ItemSlotReleased->GetName());

	//m_MainHUDWidget->ExcuteItemslotDragAndDropCount();
	m_MainHUDWidget->SetReleasedItemSlot(ItemSlotReleased);
	//SetSelectItemSlot(nullptr);
	SetSelectItemSlot(ItemSlotReleased);
}

void UInventory::ItemSlotHoveredCallBack(UItemSlot* ItemSlotHovered, const FPointerEvent& MouseEvent)
{
	SetSelectItemSlot(ItemSlotHovered);

	// if(nullptr != m_MainHUDWidget->GetPressedItemSlot() || false == ItemSlotHovered->IsItemSlotEmpty())
	// {
	// 	UGameplayStatics::PlaySound2D(GetWorld(), m_HoveredSound);
	// }
}

void UInventory::ItemSlotLeavedCallBack(UItemSlot* ItemSlotLeaved, const FPointerEvent& MouseEvent)
{
	SetSelectItemSlot(nullptr);
}

void UInventory::SetItemSlotSelectImagePosition()
{
	UCanvasPanelSlot* ItemSlot = Cast<UCanvasPanelSlot>(m_SelectItemSlot->Slot);
	if(nullptr == ItemSlot) { return; }
	
	UCanvasPanelSlot* ItemSlotSelect = Cast<UCanvasPanelSlot>(m_ItemSlotSelectImage->Slot);
	if(nullptr == ItemSlotSelect) { return; }

	m_ItemSlotSelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	ItemSlotSelect->SetPosition(ItemSlot->GetPosition());
}

void UInventory::ShowSelectItemInfo()
{
	if(m_SelectItemSlot->IsItemSlotEmpty())
	{
		ClearItemSlotSelectImage();
		ClearSelectItemSlotInfo();
		return;
	}
	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	const FItemIconTableInfo* ItemIconInfo = GameInst->FindItemIconInfo(m_SelectItemSlot->GetItemName());
	
	if(nullptr == GameInst || nullptr == ItemIconInfo)
	{
		ClearItemSlotSelectImage();
		ClearSelectItemSlotInfo();
		return;
	}

	m_SelectItemIconImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	m_SelectItemIconImage->SetBrushFromTexture(ItemIconInfo->IconTexture2D);
	m_SelectItemNameText->SetText(FText::FromString(ItemIconInfo->Name));
	m_SelectItemDescText->SetText(FText::FromString(ItemIconInfo->Desc));
}

void UInventory::ClearItemSlotSelectImage()
{
	m_ItemSlotSelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 0)));
}

void UInventory::ClearSelectItemSlotInfo()
{
	m_SelectItemIconImage->SetBrushFromTexture(nullptr);
	m_SelectItemIconImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 0)));
	m_SelectItemNameText->SetText(FText::FromString(TEXT("")));
	m_SelectItemDescText->SetText(FText::FromString(TEXT("")));
}
