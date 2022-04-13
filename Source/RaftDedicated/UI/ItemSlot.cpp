// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"
#include "Components/Border.h"
#include "../RaftDedicatedGameInstance.h"

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	m_ItemSlotIconBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ItemSlotIconBorder")));
	m_ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));

	m_ItemSlotIconBorder->OnMouseButtonDownEvent.BindDynamic(this, &UItemSlot::OnItemSlotIconBorderPressed);
	m_ItemSlotIconBorder->OnMouseButtonUpEvent.BindDynamic(this, &UItemSlot::OnItemSlotIconBorderReleased);
	m_ItemSlotIconBorder->OnMouseMoveEvent.BindDynamic(this, &UItemSlot::OnItemSlotIconBorderHovered);
	
	m_ItemName.Empty();
	m_ItemCount = 0;
	m_ItemMaxCount = 0;
	m_IsBoderMousePressed = false;
	m_ItemType = EItemType::None;
}

void UItemSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	//LOG(TEXT("Border NativeOnMouseLeave : %s"), *GetName());
	Super::NativeOnMouseLeave(InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")) || InMouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		m_IsBoderMousePressed = false;
	}

	if(m_LeavedDelegate.IsBound())
	{
    	m_LeavedDelegate.Execute(this, InMouseEvent);
    }
}

bool UItemSlot::AddItem(const FString& ItemName, int32& ItemCount)
{
	if(IsItemSlotEmpty())
	{
		URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
		if(nullptr == GameInst) { return false; }
		const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(ItemName);
		if(nullptr == Info) { return false;}
		
		// Set
		SetItemName(Info->Name);
		SetItemIconTexture(Info->IconTexture2D);
		SetItemCount(ItemCount, Info->MaxCount);
		SetItemType(Info->Type);
	}
	else
	{
		// Add
		AddItemCount(ItemCount);
	}

	return true;
}

bool UItemSlot::RemoveItem(const FString& ItemName,int32& ItemCount)
{
	if(ItemName != m_ItemName || IsItemSlotEmpty()) { return false; }

	if(m_ItemCount <= ItemCount)
	{
		ItemCount -= m_ItemCount;
		ResetItemSlot();
	}
	else
	{
		m_ItemCount -= ItemCount;
		ItemCount = 0;
		SetItemCountText();
	}

	return true;
}

bool UItemSlot::SetItem(const FString& ItemName, int32 ItemCount)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return false; }
	const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(ItemName);
	if(nullptr == Info) { return false;}

	SetItemName(Info->Name);
	SetItemIconTexture(Info->IconTexture2D);
	SetItemCount(ItemCount, Info->MaxCount);
	SetItemType(Info->Type);

	return true;
}

void UItemSlot::SetItemName(const FString& ItemName)
{
	m_ItemName = ItemName;
}

void UItemSlot::SetItemIconTexture(UTexture2D* ItemIconTexture)
{
	m_ItemSlotIconBorder->SetBrushFromTexture(ItemIconTexture);
	if(nullptr == ItemIconTexture)
	{
		m_ItemSlotIconBorder->SetBrushColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 0.f)));
	}
	else
	{
		m_ItemSlotIconBorder->SetBrushColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f)));
	}
}

void UItemSlot::SetItemCount(int32& ItemCount, int32 ItemMaxCount)
{
	m_ItemMaxCount = ItemMaxCount;
	m_ItemCount = ItemCount;
	ItemCount = CalculateItemCount();
	SetItemCountText();
}

void UItemSlot::SetItemType(EItemType Type)
{
	m_ItemType = Type;
}

void UItemSlot::AddItemCount(int32& ItemCount)
{
	m_ItemCount += ItemCount;
	ItemCount = CalculateItemCount();
	SetItemCountText();
}

// bool UItemSlot::IsItemSlotHovered()
// {
// 	FIntPoint MouseAbsolutePosition;
// 	GetWorld()->GetGameViewport()->Viewport->GetMousePos(MouseAbsolutePosition, false);
// 	return GetCachedGeometry().IsUnderLocation(FVector2D(MouseAbsolutePosition));
// }

void UItemSlot::TradeSlot(UItemSlot* OtherSlot, bool All)
{
	if(nullptr == OtherSlot || this == OtherSlot) { return; }

	// Name
	const FString ThisItemName = m_ItemName;
	const FString OtherItemName = OtherSlot->m_ItemName;

	// Count
	int32 ThisItemCount = m_ItemCount;
	int32 OtherItemCount = OtherSlot->m_ItemCount;

	// MaxCount
	int32 ThisItemMaxCount = m_ItemMaxCount;
	int32 OtherItemMaxCount = OtherSlot->m_ItemMaxCount;

	// Texture
	UTexture2D* ThisItemIconTexture = Cast<UTexture2D>(m_ItemSlotIconBorder->Background.GetResourceObject());
	UTexture2D* OtherItemIconTexture = Cast<UTexture2D>(OtherSlot->m_ItemSlotIconBorder->Background.GetResourceObject());

	// Item Type
	EItemType ThisItemType = m_ItemType;
	EItemType OtherItemType = OtherSlot->m_ItemType;
	
	if(All)
	{
        m_ItemName = OtherItemName;
        OtherSlot->m_ItemName = ThisItemName;
        
        m_ItemCount = OtherItemCount;
        OtherSlot->m_ItemCount = ThisItemCount;
        
        m_ItemMaxCount = OtherItemMaxCount;
        OtherSlot->m_ItemMaxCount = ThisItemMaxCount;

		m_ItemType = OtherItemType;
		OtherSlot->m_ItemType = ThisItemType;
		
        SetItemCountText();
        OtherSlot->SetItemCountText();
        
        SetItemIconTexture(OtherItemIconTexture);
        OtherSlot->SetItemIconTexture(ThisItemIconTexture);
	}
	else
	{
		if(ThisItemName == OtherItemName)
		{
			if(OtherItemCount < OtherItemMaxCount)
			{
				m_ItemCount -= 1;
				OtherSlot->m_ItemCount += 1;
				
				SetItemCountText();
				OtherSlot->SetItemCountText();
			}
		}
		else
		{
			if(OtherSlot->IsItemSlotEmpty())
			{
				m_ItemCount -= 1;
				
				OtherSlot->m_ItemName = ThisItemName;
				OtherSlot->m_ItemCount = 1;
				OtherSlot->m_ItemMaxCount = ThisItemMaxCount;
				OtherSlot->m_ItemType = ThisItemType;
				
				SetItemCountText();
				OtherSlot->SetItemCountText();
				
				OtherSlot->SetItemIconTexture(ThisItemIconTexture);
			}
		}

		if(m_ItemCount == 0)
		{
			ResetItemSlot();
		}
	}
}

void UItemSlot::ThrowAwayItem()
{
	m_ItemName.Empty();
	m_ItemCount = 0;
	m_ItemMaxCount = 0;
	m_IsBoderMousePressed = false;
	m_ItemSlotIconBorder->SetBrushFromTexture(nullptr);
	m_ItemSlotIconBorder->SetBrushColor(FLinearColor(FVector4(1.f, 1.f, 1.f, 0.f)));
	m_ItemCountText->SetText(FText::FromString(TEXT("")));
	m_ItemType = EItemType::None;
}

void UItemSlot::ItemSlotPressedEffect(bool Enable)
{
	if(Enable)
	{
		m_BackgroundImage->SetColorAndOpacity(FLinearColor(FVector4(0.5f, 0.5f, 0.5f, 1.f)));
	}
	else
	{
		m_BackgroundImage->SetColorAndOpacity(FLinearColor(FVector4(1.f, 1.f, 1.f, 1.f)));
	}
}

FEventReply UItemSlot::OnItemSlotIconBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;
	//LOG(TEXT("Border Pressed : %s"), *GetName());

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")) || MouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		m_IsBoderMousePressed = true;
	
		if(m_PressedDelegate.IsBound())
		{
			m_PressedDelegate.Execute(this, MouseEvent);
		}
	}
	
	return result;
}

FEventReply UItemSlot::OnItemSlotIconBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;
	//LOG(TEXT("Border Released : %s"), *GetName());

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")) || MouseEvent.GetEffectingButton() == FKey(TEXT("RightMouseButton")))
	{
		if(m_ReleasedDelegate.IsBound())
		{
			m_ReleasedDelegate.Execute(this, MouseEvent);
		}

		if(m_IsBoderMousePressed && m_ClickedDelegate.IsBound())
		{
			m_ClickedDelegate.Execute(this, MouseEvent);
			m_IsBoderMousePressed = false;
		}
	}
	
	
	return result;
}

FEventReply UItemSlot::OnItemSlotIconBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	if(m_HoveredDelegate.IsBound())
	{
		m_HoveredDelegate.Execute(this, MouseEvent);
	}
	
	return result;
}

int32 UItemSlot::CalculateItemCount()
{
	if(m_ItemCount <= m_ItemMaxCount)
	{
		return 0;
	}
	else
	{
		int ItemCount = m_ItemCount - m_ItemMaxCount;
		m_ItemCount = m_ItemMaxCount;
		return ItemCount;
	}
}

void UItemSlot::SetItemCountText()
{
	if(m_ItemCount > 1) { m_ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_ItemCount))); }
	else { m_ItemCountText->SetText(FText::FromString(TEXT(""))); }
}
