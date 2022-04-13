// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

/**
 * 
 */


DECLARE_DELEGATE_TwoParams(FItemSlotIconClickedDelegate, class UItemSlot*, const FPointerEvent&);
DECLARE_DELEGATE_TwoParams(FItemSlotIconPressedDelegate, class UItemSlot*,const FPointerEvent&);
DECLARE_DELEGATE_TwoParams(FItemSlotIconReleasedDelegate, class UItemSlot*,const FPointerEvent&);
DECLARE_DELEGATE_TwoParams(FItemSlotIconHoveredDelegate, class UItemSlot*,const FPointerEvent&);
DECLARE_DELEGATE_TwoParams(FItemSlotIconLeavedDelegate, class UItemSlot*,const FPointerEvent&);
DECLARE_DELEGATE_TwoParams(FItemSlotIconMouseEventDelegate, class UItemSlot*,const FPointerEvent&);


UCLASS()
class RAFTDEDICATED_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_BackgroundImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBorder*				m_ItemSlotIconBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_ItemCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString					m_ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemType				m_ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_IsBoderMousePressed;
	
	FItemSlotIconClickedDelegate	m_ClickedDelegate;
	FItemSlotIconPressedDelegate	m_PressedDelegate;
	FItemSlotIconReleasedDelegate	m_ReleasedDelegate;
	FItemSlotIconHoveredDelegate	m_HoveredDelegate;
	FItemSlotIconLeavedDelegate		m_LeavedDelegate;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	
public:
	const FString& GetItemName() { return m_ItemName; }
	int32 GetItemCount() {return m_ItemCount;}
	int32 GetItemMaxCount() {return m_ItemMaxCount;}
	EItemType GetItemType() {return m_ItemType;}
	UTexture2D* GetItemIconTexture() { if(nullptr == m_ItemSlotIconBorder) { return nullptr; } return Cast<UTexture2D>(m_ItemSlotIconBorder->Background.GetResourceObject()); }
	bool IsAddItemPossible(const FString& ItemName) { return ItemName == m_ItemName && m_ItemCount < m_ItemMaxCount; }
	bool IsItemSlotEmpty() { return 0 == m_ItemCount || m_ItemName.IsEmpty(); }
	void ResetItemSlot()
	{ SetItemIconTexture(nullptr); m_ItemCount = 0; m_ItemMaxCount = 0; m_ItemName.Empty();
		SetItemCountText(); m_IsBoderMousePressed = false; m_ItemType = EItemType::None;}
	
public:
	bool AddItem(const FString& ItemName, int32& ItemCount);
	bool RemoveItem(const FString& ItemName,int32& ItemCount);
	bool SetItem(const FString& ItemName, int32 ItemCount);
	void SetItemName(const FString& ItemName);
	void SetItemIconTexture(UTexture2D* ItemIconTexture);
	void SetItemCount(int32& ItemCount, int32 ItemMaxCount);
	void SetItemType(EItemType Type);
	void AddItemCount(int32& ItemCount);
	void TradeSlot(UItemSlot* OtherSlot, bool All = true);
	void ThrowAwayItem();

	void ItemSlotPressedEffect(bool Enable);
	
public:
	UFUNCTION()
	FEventReply OnItemSlotIconBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnItemSlotIconBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnItemSlotIconBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	
public:
	template <typename T>
	void SetClickedDelegate(T* Obj, void(T::* Func)(class UItemSlot* ItemSlotClicked, const FPointerEvent& MouseEvent))
	{
		m_ClickedDelegate.BindUObject(Obj, Func);
	}

	template <typename T>
	void SetPressedDelegate(T* Obj, void(T::* Func)(class UItemSlot* ItemSlotPressed, const FPointerEvent& MouseEvent))
	{
		m_PressedDelegate.BindUObject(Obj, Func);
	}

	template <typename T>
	void SetReleasedDelegate(T* Obj, void(T::* Func)(class UItemSlot* ItemSlotReleased, const FPointerEvent& MouseEvent))
	{
		m_ReleasedDelegate.BindUObject(Obj, Func);
	}

	template <typename T>
	void SetHoveredDelegate(T* Obj, void(T::* Func)(class UItemSlot* ItemSlotHovered, const FPointerEvent& MouseEvent))
	{
		m_HoveredDelegate.BindUObject(Obj, Func);
	}

	template <typename T>
	void SetLeavedDelegate(T* Obj, void(T::* Func)(class UItemSlot* ItemSlotLeaved, const FPointerEvent& MouseEvent))
	{
		m_LeavedDelegate.BindUObject(Obj, Func);
	}
	
private:
	int32 CalculateItemCount();
	void SetItemCountText();
};





