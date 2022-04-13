// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingList.h"
#include "CraftingItem.h"
#include "CraftingItemInfo.h"
#include "MainHUD.h"
#include "NotebookCrafting.h"
#include "../RaftDedicatedGameInstance.h"
#include "Components/CanvasPanelSlot.h"

void UCraftingList::NativeConstruct()
{
	Super::NativeConstruct();

	m_CraftingItemCount = 0;
	m_CraftingItemMaxCount = 8;

	m_CraftingItemOffset = 120;
	m_CraftingItemSpacing = 80;

	m_CategoryType = ENotebookCraftingCategoryType::None;
	
	m_BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Background")));
	m_CategoryNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CategoryNameText")));
	m_CategoryIconImage = Cast<UImage>(GetWidgetFromName(TEXT("CategoryIconImage")));

	for(int32 i = 0; i < m_CraftingItemMaxCount; ++i)
	{
		m_ArrayCraftingItem.Add(Cast<UCraftingItem>(GetWidgetFromName(*FString::Printf(TEXT("UI_CraftingItem_%d"), i))));
		m_ArrayCraftingItem[i]->SetCraftingListWidget(this);
	}

	m_SelectImage = Cast<UImage>(GetWidgetFromName(TEXT("Crafting_SelectImage")));

	m_CraftingItemInfoWidget = Cast<UCraftingItemInfo>(GetWidgetFromName(TEXT("UI_CraftingInfo")));
	m_CraftingItemInfoWidget->SetCraftingListWidget(this);
}

void UCraftingList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCraftingList::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if( ESlateVisibility::Collapsed == InVisibility)
	{
		m_CategoryType = ENotebookCraftingCategoryType::None;
		
		m_CraftingItemInfoWidget->SetVisibility(InVisibility);
	}
	else
	{
		m_CraftingItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCraftingList::SetCategoryInfo(ENotebookCraftingCategoryType CategoryType, UTexture2D* CategoryIconImage)
{
	m_CategoryType = CategoryType;
	m_CategoryIconImage->SetBrushFromTexture(CategoryIconImage);

	SetCategoryNameText();
}

void UCraftingList::SetCraftingList(const TArray<FString>& CraftingArray)
{
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }

	for(int32 i = 0; i < m_CraftingItemMaxCount; ++i)
	{
		if(i < CraftingArray.Num())
		{
			const FItemIconTableInfo* Info = GameInst->FindItemIconInfo(CraftingArray[i]);
			m_ArrayCraftingItem[i]->SetCraftingItem(Info);
			m_ArrayCraftingItem[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			m_ArrayCraftingItem[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	SetBackgroundSize(CraftingArray.Num());
}

void UCraftingList::OnCraftingItemMouseEnter(UCraftingItem* ItemWidget)
{
	m_SelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	
	UCanvasPanelSlot* ItemWidgetSlot = Cast<UCanvasPanelSlot>(ItemWidget->Slot);
	if(nullptr == ItemWidgetSlot) { return; }
	
	UCanvasPanelSlot* SelectSlot = Cast<UCanvasPanelSlot>(m_SelectImage->Slot);
	if(nullptr == SelectSlot) { return; }

	m_SelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	FVector2D Position = SelectSlot->GetPosition();
	Position.Y = ItemWidgetSlot->GetPosition().Y;
	SelectSlot->SetPosition(Position);

	UGameplayStatics::PlaySound2D(GetWorld(), m_NotebookCraftingWidget->GetMainHudWidget()->GetUISound(EUISoundType::Hovered));
}

void UCraftingList::OnCraftingItemMouseLeave(UCraftingItem* ItemWidget)
{
	m_SelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 0)));
}

void UCraftingList::OnCraftingItemMouseDown(UCraftingItem* ItemWidget)
{
	m_CraftingItemInfoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_CraftingItemInfoWidget->SetCraftingItemName(ItemWidget->GetCraftingItemName());
	UGameplayStatics::PlaySound2D(GetWorld(), m_NotebookCraftingWidget->GetMainHudWidget()->GetUISound(EUISoundType::MenuClicked));
}

void UCraftingList::SetCategoryNameText()
{
	FString Name;
	switch (m_CategoryType)
	{
	case ENotebookCraftingCategoryType::FoodWater: {Name = TEXT("FoodWater"); break;}
	case ENotebookCraftingCategoryType::Other: {Name = TEXT("Other"); break;}
	case ENotebookCraftingCategoryType::Utilities:{Name = TEXT("Utilities"); break;}
	case ENotebookCraftingCategoryType::Equipment:{Name = TEXT("Equipment"); break;}
	case ENotebookCraftingCategoryType::Materials:{Name = TEXT("Materials"); break;}
	case ENotebookCraftingCategoryType::Navigation:{Name = TEXT("Navigation"); break;}
	case ENotebookCraftingCategoryType::Decoration:{Name = TEXT("Decoration"); break;}
	case ENotebookCraftingCategoryType::None:{Name = TEXT(""); break;}
	}

	
	m_CategoryNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Name)));
}

void UCraftingList::SetBackgroundSize(int32 ListCount)
{
	UCanvasPanelSlot* BackgroundSlot = Cast<UCanvasPanelSlot>(m_BackgroundImage->Slot);
	FVector2D Size = BackgroundSlot->GetSize();
	Size.Y = m_CraftingItemOffset + m_CraftingItemSpacing * ListCount;
	BackgroundSlot->SetSize(Size);
}

