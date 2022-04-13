// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "Aim.h"
#include "BreathBar.h"
#include "ItemInteraction.h"
#include "NotebookCrafting.h"
#include "Inventory.h"
#include "QuickItemSlotList.h"
#include "ItemSlot.h"
#include "DragAndDropShadow.h"
#include "AddItemList.h"
#include "AITypes.h"
#include "HammerItemList.h"
#include "PlayerStateList.h"
#include "StorageItemList.h"
#include "HammerItemList.h"
#include "IneractionFeedback.h"
#include "Components/CanvasPanelSlot.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"


void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_Aim = Cast<UAim>(GetWidgetFromName(TEXT("UI_Aim")));
	m_BreathBar = Cast<UBreathBar>(GetWidgetFromName(TEXT("UI_BreathBar")));
	m_ItemInteractionWidget = Cast<UItemInteraction>(GetWidgetFromName(TEXT("UI_ItemInteraction")));
	m_QuickItemSlotListWidget = Cast<UQuickItemSlotList>(GetWidgetFromName(TEXT("UI_QuickItemSlotList")));
	m_NotebookCrafting = Cast<UNotebookCrafting>(GetWidgetFromName(TEXT("UI_NotebookCrafting")));
	m_InventoryWidget = Cast<UInventory>(GetWidgetFromName(TEXT("UI_Inventory")));
	m_DragAndDropShadowWidget = Cast<UDragAndDropShadow>(GetWidgetFromName(TEXT("UI_DragAndDropShadow")));
	m_AddItemListWidget = Cast<UAddItemList>(GetWidgetFromName(TEXT("UI_AddItemList")));
	m_PlayerStateListWidget = Cast<UPlayerStateList>(GetWidgetFromName(TEXT("UI_PlayerState")));
	m_StorageItemListWidget = Cast<UStorageItemList>(GetWidgetFromName(TEXT("UI_StorageItemList")));
	m_HammerItemListWidget = Cast<UHammerItemList>(GetWidgetFromName(TEXT("UI_HammerItemList")));
	m_InteractionFeedbackWidget = Cast<UIneractionFeedback>(GetWidgetFromName(TEXT("UI_IneractionFeedback")));
	
	m_ArrayTapKeyWidget.Add(m_NotebookCrafting);
	m_ArrayTapKeyWidget.Add(m_InventoryWidget);
	m_EnableWidgetFromTapKey = false;

	m_QuickItemSlotListWidget->SetMainHudWidget(this);
	m_InventoryWidget->SetMainHudWidget(this);
	m_NotebookCrafting->SetMainHudWidget(this);
	m_StorageItemListWidget->SetMainHudWidget(this);
	m_HammerItemListWidget->SetMainHudWidget(this);
	
	m_PressedItemSlot = nullptr;
	m_ReleasedItemSlot = nullptr;

	bIsFocusable = true;
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UMainHUD::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(m_PressedItemSlot)
	{
		if(m_InventoryWidget->IsHovered() || m_QuickItemSlotListWidget->IsHovered() || m_StorageItemListWidget->IsHovered())
		{
			m_DragAndDropShadowWidget->ItemThrowAwayText(false);	
		}
		else
		{
			m_DragAndDropShadowWidget->ItemThrowAwayText(true);
		}
	}
	
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UMainHUD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMainHUD::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(m_PressedItemSlot)
	{
		// 아이템 버리기
		UGameplayStatics::PlaySound2D(GetWorld(), GetUISound(EUISoundType::Drop));
		m_PressedItemSlot->ThrowAwayItem();
		m_PressedItemSlot->ItemSlotPressedEffect(false);
		m_PressedItemSlot = nullptr;
		ExcuteItemSlotDragAndDrop();
	}
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UMainHUD::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == FKey(TEXT("Tab")) && m_StorageItemListWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		EnableWidgetFromTapKey();
	}

	if(InKeyEvent.GetKey() == FKey(TEXT("E")) && false == m_EnableWidgetFromTapKey)
	{
		//StorageInteractionExit();
		SetMouseInputMode(false);
	}
	
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMainHUD::EnableWidgetFromTapKey()
{
	m_EnableWidgetFromTapKey = !m_EnableWidgetFromTapKey;
	
	for(auto widget : m_ArrayTapKeyWidget)
	{
		if(m_EnableWidgetFromTapKey)
		{
			widget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(false == m_EnableWidgetFromTapKey)
	{
		if(ESlateVisibility::Collapsed != m_HammerItemListWidget->GetVisibility())
		{
			m_HammerItemListWidget->ReloadItemCount();
		}

		if(ESlateVisibility::Collapsed != m_DragAndDropShadowWidget->GetVisibility())
		{
			ExcuteItemSlotDragAndDrop();
		}
	}
	
	SetMouseInputMode(m_EnableWidgetFromTapKey);

	UGameplayStatics::PlaySound2D(GetWorld(), GetUISound(EUISoundType::MenuClicked));
}

void UMainHUD::SetItemInteractionUIText(bool SlateVisibilty, const FString& InteractionButtonText,
	const FString& ItemDescText)
{
	if(false == SlateVisibilty || m_EnableWidgetFromTapKey)
	{
		if(m_ItemInteractionWidget->GetVisibility() != ESlateVisibility::Collapsed)
		{
			m_ItemInteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if(SlateVisibilty)
	{
		if(m_ItemInteractionWidget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		{
			m_ItemInteractionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);			
		}
	
		m_ItemInteractionWidget->SetInteractionButtonText(InteractionButtonText);
		m_ItemInteractionWidget->SetIemDescText(ItemDescText);
	}
	
}

void UMainHUD::MoveQuickItemSlotSelect(bool Left)
{
	if(nullptr == m_QuickItemSlotListWidget) { return; }
	m_QuickItemSlotListWidget->MoveQuickItemSlotSelect(Left);
}

bool UMainHUD::AddItem(const FString& ItemName, int32& ItemCount)
{
	int32 AddItemCount = ItemCount;
	if(m_InventoryWidget->AddItem(ItemName, ItemCount) || m_QuickItemSlotListWidget->AddItem(ItemName, ItemCount))
	{
		m_AddItemListWidget->AddItem(ItemName, AddItemCount);
		return true;
	}

	m_AddItemListWidget->AddItem(ItemName, AddItemCount - ItemCount);
	
	return false;	
}

bool UMainHUD::RemoveItem(const FString& ItemName, int32& ItemCount)
{
	if(m_InventoryWidget->RemoveItem(ItemName, ItemCount) || m_QuickItemSlotListWidget->RemoveItem(ItemName, ItemCount))	{ return true; }
	return false;
}

int32 UMainHUD::GetItemCount(const FString& ItemName)
{
	int32 ItemCount = 0;
	ItemCount += m_InventoryWidget->GetItemCount(ItemName);
	ItemCount += m_QuickItemSlotListWidget->GetItemCount(ItemName);
	return ItemCount;
}

void UMainHUD::SetPressedItemSlot(UItemSlot* PressedItemSlot, bool All)
{
	m_PressedItemSlot = PressedItemSlot;
	if(m_PressedItemSlot) {m_PressedItemSlot->ItemSlotPressedEffect(true);}
		
	ExcuteItemSlotDragAndDropShadow(All);
}

void UMainHUD::SetReleasedItemSlot(UItemSlot* ReleasedItemSlot)
{
	m_ReleasedItemSlot = ReleasedItemSlot;
	if(m_ReleasedItemSlot)
	{
		m_PressedItemSlot->ItemSlotPressedEffect(false);
		UGameplayStatics::PlaySound2D(GetWorld(), GetUISound(EUISoundType::Drop));
	}
	ExcuteItemSlotDragAndDrop();
}

void UMainHUD::SetStatePercent(float ThirstPercent, float HungerPercent, float HealthPercent)
{
	m_PlayerStateListWidget->SetStatePercent(ThirstPercent, HungerPercent, HealthPercent);
}

void UMainHUD::SetChargingPercent(float Percent)
{
	m_Aim->SetCharigngGauge(Percent);
}

void UMainHUD::SetBreathBarPercent(float Percent)
{
	m_BreathBar->SetBreathGauge(Percent);
}

void UMainHUD::SetBreathBarVisible(ESlateVisibility Type)
{
	m_BreathBar->SetVisibility(Type);
}

void UMainHUD::StorageInteractionEnter(class AStorage* Storage)
{
	if(m_StorageItemListWidget->GetVisibility() == ESlateVisibility::Visible) { return;	}

	m_StorageItemListWidget->SetTargetStorage(Storage);
	
	m_InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	m_StorageItemListWidget->SetVisibility(ESlateVisibility::Visible);
	SetMouseInputMode(true);
}

void UMainHUD::StorageInteractionExit(class AStorage* Storage)
{
	if(m_StorageItemListWidget->GetVisibility() == ESlateVisibility::Collapsed) { return; }
	m_InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	m_StorageItemListWidget->SetVisibility(ESlateVisibility::Collapsed);
	SetMouseInputMode(false);
}

void UMainHUD::SetHammerItemListWidget(bool Enabled, const FString& ItemName)
{
	if(ESlateVisibility::Collapsed == m_HammerItemListWidget->GetVisibility() && Enabled)
	{
		m_HammerItemListWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else if(ESlateVisibility::Collapsed != m_HammerItemListWidget->GetVisibility() && false == Enabled)
	{
		m_HammerItemListWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(Enabled)
	{
		m_HammerItemListWidget->SetHammerItem(ItemName);
	}
}

void UMainHUD::BuildCompleted(const FString& ItemName)
{
	m_QuickItemSlotListWidget->RemoveItem(ItemName);
}

void UMainHUD::ExcuteItemSlotDragAndDrop()
{
	if(nullptr != m_PressedItemSlot)
	{
		m_PressedItemSlot->TradeSlot(m_ReleasedItemSlot, m_DragAndDropShadowWidget->IsItemAll());
	}

	m_PressedItemSlot = nullptr;
	m_ReleasedItemSlot = nullptr;
	
	m_DragAndDropShadowWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainHUD::ExcuteItemSlotDragAndDropShadow(bool All)
{
	if(false == m_PressedItemSlot->IsItemSlotEmpty())
	{
		m_DragAndDropShadowWidget->SetItem(m_PressedItemSlot, All);
	}
}

void UMainHUD::SetMouseInputMode(bool Enable)
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	
	if(Enable)
	{
		FInputModeUIOnly Mode; // UI 조작 하는 것
		//FInputModeGameAndUI Mode;
		
		Controller->SetInputMode(Mode);
		Controller->SetShowMouseCursor(true);
		FIntPoint ViewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
		Controller->SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);

		SetFocus();
	}
	else
	{
		FInputModeGameOnly Mode; // 디폴트 마우스 조작안됨
		Controller->SetInputMode(Mode);
		Controller->SetShowMouseCursor(false);
	}
}