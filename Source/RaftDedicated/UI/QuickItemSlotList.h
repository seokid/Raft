// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ItemSlot.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "QuickItemSlotList.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UQuickItemSlotList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemSlotListMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UItemSlot*>			m_ArrayItemSlotWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_ItemSlotSelectImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemSlotSelectIdx;
	
	class UMainHUD*			m_MainHUDWidget;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetMainHudWidget(class UMainHUD* MainHUD) { m_MainHUDWidget = MainHUD; }
	TArray<class UItemSlot*>& GetArrayItemSlot() { return m_ArrayItemSlotWidget; }

public:
	void MoveQuickItemSlotSelect(bool Left);
	bool AddItem(const FString& ItemName, int32& ItemCount);
	bool RemoveItem(const FString& ItemName, int32& ItemCount);
	bool RemoveItem(const FString& ItemName);
	int32 GetItemCount(const FString& ItemName);
	
	bool RemoveSelectItem(const FString& ItemName, int32& ItemCount);
	const FString& GetSelectItemName();
	int32 GetSelectItemCount();
	bool IsSelectItemEmpty();
	bool ChangeSelectItem(const FString& ItemName, int32& ItemCount, bool ResetSelect = true);
	void ReSelectItem();
	
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
	void SetSelectItemToPlayer();
};





