// Fill out your copyright notice in the Description page of Project Settings.


#include "NotebookCrafting.h"
#include "Components/CanvasPanelSlot.h"
#include "CraftingList.h"
#include "MainHUD.h"

void UNotebookCrafting::NativeConstruct()
{
	Super::NativeConstruct();

	m_PressedCategoryType = ENotebookCraftingCategoryType::None;
	m_HoveredCategoryType = ENotebookCraftingCategoryType::None;
	
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryFoodWaterBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryOtherBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryUtilitiesBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryEquipmentBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryMaterialsBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryNavigationBorder"))));
	m_ArrayCategory.Add(Cast<UBorder>(GetWidgetFromName(TEXT("CategoryDecorationBorder"))));

	m_CategorySelectImage = Cast<UImage>(GetWidgetFromName(TEXT("Category_SelectImage")));

	for(int32 i = 0; i < m_ArrayCategory.Num(); ++i)
	{
		m_ArrayCategory[i]->OnMouseButtonDownEvent.BindDynamic(this, &UNotebookCrafting::OnCategoryBorderPressed);
		m_ArrayCategory[i]->OnMouseButtonUpEvent.BindDynamic(this, &UNotebookCrafting::OnCategoryBorderReleased);
		m_ArrayCategory[i]->OnMouseMoveEvent.BindDynamic(this, &UNotebookCrafting::OnCategoryBorderHovered);
	}

	m_CraftingListWidget = Cast<UCraftingList>(GetWidgetFromName(TEXT("UI_CraftingList")));
	m_CraftingListWidget->SetNotebookCraftingWidget(this);

	CraftingMap.Add(ENotebookCraftingCategoryType::FoodWater);
	CraftingMap.Add(ENotebookCraftingCategoryType::Other);
	CraftingMap.Add(ENotebookCraftingCategoryType::Utilities);
	CraftingMap.Add(ENotebookCraftingCategoryType::Equipment);
	CraftingMap.Add(ENotebookCraftingCategoryType::Materials);
	CraftingMap.Add(ENotebookCraftingCategoryType::Navigation);
	CraftingMap.Add(ENotebookCraftingCategoryType::Decoration);
	
	// CraftingMap[ENotebookCraftingCategoryType::FoodWater].Add(TEXT("Cup"));
	// CraftingMap[ENotebookCraftingCategoryType::FoodWater].Add(TEXT("Purifier"));
	// CraftingMap[ENotebookCraftingCategoryType::FoodWater].Add(TEXT("Grill"));
	//
	// CraftingMap[ENotebookCraftingCategoryType::Other].Add(TEXT("Storage"));
	//
	// CraftingMap[ENotebookCraftingCategoryType::Utilities].Add(TEXT("Hammer"));
	// CraftingMap[ENotebookCraftingCategoryType::Utilities].Add(TEXT("PlasticHook"));
	// CraftingMap[ENotebookCraftingCategoryType::Utilities].Add(TEXT("FishingRod"));
	//
	// CraftingMap[ENotebookCraftingCategoryType::Equipment].Add(TEXT("WoodSpear"));
	//
	// CraftingMap[ENotebookCraftingCategoryType::Materials].Add(TEXT("Rope"));
	// CraftingMap[ENotebookCraftingCategoryType::Materials].Add(TEXT("Nail"));
}

void UNotebookCrafting::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNotebookCrafting::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	SetSelectCategory(ENotebookCraftingCategoryType::None);
	CategoryClear();
}

void UNotebookCrafting::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetSelectCategory(ENotebookCraftingCategoryType::None);
}

void UNotebookCrafting::AddCraftingItem(ENotebookCraftingCategoryType Type, const FString& CraftingItemName)
{
	if(INDEX_NONE == CraftingMap[Type].Find(CraftingItemName))
	{
		CraftingMap[Type].Add(CraftingItemName);
	}
}

FEventReply UNotebookCrafting::OnCategoryBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		m_PressedCategoryType = m_HoveredCategoryType;
	}
	else
	{
		m_PressedCategoryType = ENotebookCraftingCategoryType::None;
	}
	
	return result;
}

FEventReply UNotebookCrafting::OnCategoryBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	if(MouseEvent.GetEffectingButton() == FKey(TEXT("LeftMouseButton")))
	{
		if(ENotebookCraftingCategoryType::None != m_PressedCategoryType && m_PressedCategoryType == m_HoveredCategoryType)
		{
			CategoryClicked();
		}
	}
	
	return result;
}

FEventReply UNotebookCrafting::OnCategoryBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply result;

	SetSelectCategory(GetHoveredCategoryType());
	return result;
}

ENotebookCraftingCategoryType UNotebookCrafting::GetHoveredCategoryType()
{
	for(int32 i = 0; i < m_ArrayCategory.Num(); ++i)
	{
		if(m_ArrayCategory[i]->IsHovered())
		{
			return static_cast<ENotebookCraftingCategoryType>(i);
		}
	}

	return ENotebookCraftingCategoryType::None;
}

void UNotebookCrafting::SetSelectCategory(ENotebookCraftingCategoryType HoveredType)
{
	if(ENotebookCraftingCategoryType::None == HoveredType)
	{
		m_HoveredCategoryType = HoveredType;
		ClearCategorySelectImage();
		return;
	}

	if(HoveredType == m_HoveredCategoryType) { return; }

	UGameplayStatics::PlaySound2D(GetWorld(), m_MainHUDWidget->GetUISound(EUISoundType::Hovered));
	
	m_HoveredCategoryType = HoveredType;
	
	UCanvasPanelSlot* CategorySlot = Cast<UCanvasPanelSlot>(m_ArrayCategory[static_cast<int32>(m_HoveredCategoryType)]->Slot);
	if(nullptr == CategorySlot) { return; }
	
	UCanvasPanelSlot* SelectSlot = Cast<UCanvasPanelSlot>(m_CategorySelectImage->Slot);
	if(nullptr == SelectSlot) { return; }

	m_CategorySelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 255)));
	SelectSlot->SetPosition(CategorySlot->GetPosition());
}

void UNotebookCrafting::ClearCategorySelectImage()
{
	m_CategorySelectImage->SetColorAndOpacity(FLinearColor(FColor(255, 255, 255, 0)));
}

void UNotebookCrafting::CategoryClicked()
{
	if(m_CraftingListWidget->GetCategoryType() == m_PressedCategoryType) { return; }

	UGameplayStatics::PlaySound2D(GetWorld(), m_MainHUDWidget->GetUISound(EUISoundType::MenuClicked));
	
	m_CraftingListWidget->SetVisibility(ESlateVisibility::Visible);
	m_CraftingListWidget->SetCategoryInfo(m_PressedCategoryType, Cast<UTexture2D>(m_ArrayCategory[static_cast<int32>(m_PressedCategoryType)]->Background.GetResourceObject()));
	m_CraftingListWidget->SetCraftingList(CraftingMap[m_PressedCategoryType]);
}

void UNotebookCrafting::CategoryClear()
{
	m_CraftingListWidget->SetVisibility(ESlateVisibility::Collapsed);
}
