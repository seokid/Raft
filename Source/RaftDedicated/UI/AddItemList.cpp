// Fill out your copyright notice in the Description page of Project Settings.


#include "AddItemList.h"



#include "AddItemInfo.h"
#include "Components/CanvasPanelSlot.h"

void UAddItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_AddItemInfoCount = 5;
	m_AddItemInfoSpacing = -70.f;

	for(int32 i = 0; i < m_AddItemInfoCount; ++i)
	{
		m_ArrayAddItemInfoWidget.Add(Cast<UAddItemInfo>(GetWidgetFromName(*FString::Printf(TEXT("UI_AddItemInfo_%d"), i))));
		m_ArrayAddItemInfoWidgetDisable.Add(m_ArrayAddItemInfoWidget[i]);
		m_ArrayAddItemInfoWidget[i]->SetIdx(-1);
		m_ArrayAddItemInfoWidget[i]->SetAddItemListWidget(this);
	}

	m_ExcuteAddItemMove = false;
	m_AccTime = 0.f;
	m_MaxTime = 0.5f;
}

void UAddItemList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	ExcuteAddItemInfoWidgetAdd();


	ExcuteAddItemInfoWidgetMove(InDeltaTime);
}

void UAddItemList::AddItem(const FString& ItemName, int32 ItemCount)
{
	m_QueAddItemInfo.Enqueue(std::make_pair(ItemName, ItemCount));
}

void UAddItemList::DisableAddItemInfo(UAddItemInfo* Widget)
{
	m_ArrayAddItemInfoWidgetDisable.Add(Widget);
	m_ArrayAddItemInfoWidgetEnable.Remove(Widget);
	Widget->SetVisibility(ESlateVisibility::Collapsed);
}

void UAddItemList::ExcuteAddItemInfoWidgetAdd()
{
	if(m_ExcuteAddItemMove || m_QueAddItemInfo.IsEmpty() || m_ArrayAddItemInfoWidgetDisable.Num() <= 0) { return; }

	for(int32 i = 0; i < m_AddItemInfoCount; ++i)
	{
		std::pair<FString, int32> PairInfo;
        m_QueAddItemInfo.Peek(PairInfo);
        
        UAddItemInfo* AddWidget = m_ArrayAddItemInfoWidgetDisable.Pop();
		if(AddWidget->SetAddItemInfo(PairInfo.first, PairInfo.second))
		{
			m_QueAddItemInfo.Pop();

			m_ArrayAddItemInfoWidgetWait.Add(AddWidget);
		}
		
		if(m_QueAddItemInfo.IsEmpty() || m_ArrayAddItemInfoWidgetDisable.Num() <= 0) { break; }	
	}

	if(m_ArrayAddItemInfoWidgetWait.Num() > 0)
	{
		m_ExcuteAddItemMove = true;
		m_AccTime = 0.f;
	}
}

void UAddItemList::ExcuteAddItemInfoWidgetMove(float InDeltaTime)
{
	if(false == m_ExcuteAddItemMove) { return; }

	m_AccTime += InDeltaTime;
	
	for(int32 i = 0; i < m_ArrayAddItemInfoWidgetEnable.Num(); ++i)
	{
		UCanvasPanelSlot* AddItemSlot = Cast<UCanvasPanelSlot>(m_ArrayAddItemInfoWidgetEnable[i]->Slot);
		int32 Idx = m_ArrayAddItemInfoWidgetEnable[i]->GetIdx();
		FVector2D SlotPosition = AddItemSlot->GetPosition();
		//SlotPosition.Y = (m_AddItemInfoSpacing * Idx * (1.f - (m_AccTime / m_MaxTime))) + (m_AddItemInfoSpacing * (Idx + m_ArrayAddItemInfoWidgetWait.Num()) * (m_AccTime / m_MaxTime));
		SlotPosition.Y = FMath::Lerp(m_AddItemInfoSpacing * Idx, m_AddItemInfoSpacing * (Idx + m_ArrayAddItemInfoWidgetWait.Num()), sinf(PI / 2.f) * (m_AccTime / m_MaxTime));
		AddItemSlot->SetPosition(SlotPosition);
	}
	
	if(m_AccTime >= m_MaxTime || m_ArrayAddItemInfoWidgetEnable.Num() <= 0)
	{
		for(int32 i = 0; i < m_ArrayAddItemInfoWidgetEnable.Num(); ++i)
		{
			m_ArrayAddItemInfoWidgetEnable[i]->SetIdx(m_ArrayAddItemInfoWidgetEnable[i]->GetIdx() + m_ArrayAddItemInfoWidgetWait.Num());
		}

		m_AccTime = 0.f;
		ExcuteAddItemInfoWidgetEanble();
		m_ExcuteAddItemMove = false;
	}
}


void UAddItemList::ExcuteAddItemInfoWidgetEanble()
{
	for(int32 i = 0; i < m_ArrayAddItemInfoWidgetWait.Num(); ++i)
	{
		m_ArrayAddItemInfoWidgetWait[i]->SetIdx(m_ArrayAddItemInfoWidgetWait.Num() - i - 1);
		UCanvasPanelSlot* AddItemSlot = Cast<UCanvasPanelSlot>(m_ArrayAddItemInfoWidgetWait[i]->Slot);
		FVector2D SlotPosition = AddItemSlot->GetPosition();
		SlotPosition.Y = m_AddItemInfoSpacing * m_ArrayAddItemInfoWidgetWait[i]->GetIdx();
		AddItemSlot->SetPosition(SlotPosition);
		m_ArrayAddItemInfoWidgetWait[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_ArrayAddItemInfoWidgetEnable.Add(m_ArrayAddItemInfoWidgetWait[i]);
	}

	m_ArrayAddItemInfoWidgetWait.Empty();
}
