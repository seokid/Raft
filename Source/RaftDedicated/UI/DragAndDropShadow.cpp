// Fill out your copyright notice in the Description page of Project Settings.


#include "DragAndDropShadow.h"
#include "ItemSlot.h"
#include "Components/CanvasPanelSlot.h"

void UDragAndDropShadow::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIconImage")));

	m_ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCountText")));
	m_ItemTrowAwayText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemThrowAwayText")));

	
}

void UDragAndDropShadow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ExcuteShadow();
}

void UDragAndDropShadow::SetItem(UItemSlot* ItemSlot, bool All)
{
	if(nullptr == ItemSlot)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		ItemThrowAwayText(false);
		return;
	}
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_ItemIconImage->SetBrushFromTexture(ItemSlot->GetItemIconTexture());

	m_ItemAll = All;
	
	if(m_ItemAll)
	{
		m_ItemCount = ItemSlot->GetItemCount();
	}
	else
	{
		m_ItemCount = 1;
	}
	
	if(ItemSlot->GetItemCount() > 0) { m_ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_ItemCount))); }
	else { m_ItemCountText->SetText(FText::FromString(TEXT(""))); }
}

void UDragAndDropShadow::ItemThrowAwayText(bool ThrowAway)
{
	if(ThrowAway && 1.f > m_ItemTrowAwayText->ColorAndOpacity.GetSpecifiedColor().A)
	{
		m_ItemTrowAwayText->SetOpacity(1.f);
	}
	else if(false == ThrowAway && 0.f < m_ItemTrowAwayText->ColorAndOpacity.GetSpecifiedColor().A)
	{
		m_ItemTrowAwayText->SetOpacity(0.f);
	}
}


void UDragAndDropShadow::ExcuteShadow()
{
	if(ESlateVisibility::Collapsed == GetVisibility()) { return; }

	FVector2D ViewportSize = FVector2D(GetWorld()->GetGameViewport()->Viewport->GetSizeXY());

	FIntPoint MousePosition;
	GetWorld()->GetGameViewport()->Viewport->GetMousePos(MousePosition);
	FVector2D MousePositionInViewport = FVector2D(MousePosition);

	float RatioViewport = ViewportSize.X / ViewportSize.Y;
	float RatioHUD = 1920.f / 1080.f;

	FVector2D MousePisitionInHUD;

	if(RatioViewport > RatioHUD) // view port의 x 가 더 길다
	{
		MousePisitionInHUD.X = (MousePositionInViewport.X / ViewportSize.X) * (RatioViewport * 1080.f);
		MousePisitionInHUD.Y = (MousePositionInViewport.Y / ViewportSize.Y) * 1080.f ;
	}
	else
	{
		MousePisitionInHUD.X = (MousePositionInViewport.X / ViewportSize.X) * 1920.f;
		MousePisitionInHUD.Y = (MousePositionInViewport.Y / ViewportSize.Y) * (1.f / RatioViewport) * 1090.f;
	}

	UCanvasPanelSlot* ShadowSlot = Cast<UCanvasPanelSlot>(Slot);
	ShadowSlot->SetPosition(MousePisitionInHUD);
}
