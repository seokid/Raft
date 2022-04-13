// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "StorageItemList.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UStorageItemList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_BackgroundImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemSlotMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UItemSlot*>			m_ArrayItemSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_StorageIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_ItemSlotSelectImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_LineToItemSlotMaxCount;

	class UMainHUD*			m_MainHUDWidget;
	class UItemSlot*		m_SelectItemSlot;

	class AStorage*			m_TargetStorage;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	void SetMainHudWidget(class UMainHUD* MainHUD) { m_MainHUDWidget = MainHUD; }
	void SetTargetStorage(class AStorage* Storage) {m_TargetStorage = Storage;}

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

public:
	void SetStorageItemMaxCount(int32 ItemMaxCount);
	
private:
	void SetSelectItemSlot(class UItemSlot* SelectItemSlot);
	void SetItemSlotSelectImagePosition();

	void EnableStorageInfo();
	void DisableStorageInfo();

	void SetItemSlotWidgetFromStorage();
	void SetItemSlotWidgetToStorage();
	
	void SetBackgroundSize();
	void SetStorageIconImage();
};


