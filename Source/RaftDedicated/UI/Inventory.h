// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */

UCLASS()
class RAFTDEDICATED_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemSlotMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UItemSlot*>			m_ArrayItemSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_SelectItemIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_ItemSlotSelectImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_SelectItemNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_SelectItemDescText;

	class UMainHUD*			m_MainHUDWidget;
	class UItemSlot*		m_SelectItemSlot;

	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	void SetMainHudWidget(class UMainHUD* MainHUD) { m_MainHUDWidget = MainHUD; }
	TArray<class UItemSlot*>& GetArrayItemSlot() { return m_ArrayItemSlotWidget; }
	
public:
	bool AddItem(const FString& ItemName, int32& ItemCount);
	bool RemoveItem(const FString& ItemName, int32& ItemCount);
	void SetSelectItemSlot(class UItemSlot* SelectItemSlot, bool IsItemSlotInInventory = true);
	int32 GetItemCount(const FString& ItemName);
	
public:
	UFUNCTION()
	void ItemSlotClickedCallBack(class UItemSlot* ItemSlotClicked, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void ItemSlotPressedCallBack(class UItemSlot* ItemSlotPressed, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void ItemSlotReleasedCallBack(class UItemSlot* ItemSlotReleased, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void ItemSlotHoveredCallBack(class UItemSlot* ItemSlotHovered, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void ItemSlotLeavedCallBack(class UItemSlot* ItemSlotLeaved, const FPointerEvent& MouseEvent);
	
private:
	void SetItemSlotSelectImagePosition();
	void ShowSelectItemInfo();
	void ClearItemSlotSelectImage();
	void ClearSelectItemSlotInfo();
};

