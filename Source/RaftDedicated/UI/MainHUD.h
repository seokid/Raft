// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/SizeBox.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter*		m_Player;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAim*					m_Aim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBreathBar*			m_BreathBar;
	
	
	// Child Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UItemInteraction*		m_ItemInteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UQuickItemSlotList*		m_QuickItemSlotListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UNotebookCrafting*		m_NotebookCrafting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInventory*		m_InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UDragAndDropShadow* m_DragAndDropShadowWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAddItemList*			m_AddItemListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerStateList*			m_PlayerStateListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStorageItemList*			m_StorageItemListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UHammerItemList*			m_HammerItemListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UIneractionFeedback*		m_InteractionFeedbackWidget;
		
	// Tap Key Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UUserWidget*>	m_ArrayTapKeyWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_EnableWidgetFromTapKey;

	// Item Slot Drag And Drop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UItemSlot*		m_PressedItemSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UItemSlot*		m_ReleasedItemSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_TapActiveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EUISoundType, USoundBase*>		m_MapUISound;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
public:
	void SetPlayer(APlayerCharacter* Player) {m_Player = Player;}
	APlayerCharacter* GetPlayer() {return m_Player;}
	UItemInteraction* GetItemInteractionWidget() {return m_ItemInteractionWidget;}
	UQuickItemSlotList* GetQuickItemSlotListWidget() { return m_QuickItemSlotListWidget; }
	UNotebookCrafting* GetNotebookCraftingWidget() { return m_NotebookCrafting; }
	UInventory* GetInventoryWidget() { return m_InventoryWidget; }
	UHammerItemList* GetHammerItemListWidget() {return m_HammerItemListWidget;}
	UIneractionFeedback* GetIneractionFeedbackWidget() {return m_InteractionFeedbackWidget;}
	
	UItemSlot* GetPressedItemSlot() {return m_PressedItemSlot;}
	
	USoundBase* GetUISound(EUISoundType Type) { return m_MapUISound[Type]; }

public:
	void EnableWidgetFromTapKey();
	void SetItemInteractionUIText(bool SlateVisibilty, const FString& InteractionButtonText = "", const FString& ItemDescText = "");
	void MoveQuickItemSlotSelect(bool Left);
	bool AddItem(const FString& ItemName, int32& ItemCount);
	bool RemoveItem(const FString& ItemName, int32& ItemCount);
	
	int32 GetItemCount(const FString& ItemName);

	void SetPressedItemSlot(class UItemSlot* PressedItemSlot, bool All = true);
	void SetReleasedItemSlot(class UItemSlot* ReleasedItemSlot); 

	void SetStatePercent(float ThirstPercent, float HungerPercent, float HealthPercent);
	void SetChargingPercent(float Percent);
	void SetBreathBarPercent(float Percent);
	void SetBreathBarVisible(ESlateVisibility Type);

	void StorageInteractionEnter(class AStorage* Storage);
	void StorageInteractionExit(class AStorage* Storage);

	void SetHammerItemListWidget(bool Enabled, const FString& ItemName = "");
	
public:
	void BuildCompleted(const FString& ItemName);
	
private:
	void ExcuteItemSlotDragAndDrop();
	void ExcuteItemSlotDragAndDropShadow(bool All = true);

	void SetMouseInputMode(bool Enable);
};



