// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "NotebookCrafting.generated.h"

/**
 * 
 */



UCLASS()
class RAFTDEDICATED_API UNotebookCrafting : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UBorder*>		m_ArrayCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_CategorySelectImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCraftingList*	m_CraftingListWidget;
	
	int32					m_PressedCategoryIdx;
	int32					m_HoveredCategoryIdx;

	ENotebookCraftingCategoryType	m_PressedCategoryType;
	ENotebookCraftingCategoryType	m_HoveredCategoryType;

	TMap<ENotebookCraftingCategoryType, TArray<FString>> CraftingMap;

	class UMainHUD*			m_MainHUDWidget;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	void SetMainHudWidget(class UMainHUD* MainHUD) { m_MainHUDWidget = MainHUD; }
	UMainHUD* GetMainHudWidget() {return m_MainHUDWidget;}

public:
	void AddCraftingItem(ENotebookCraftingCategoryType Type, const FString& CraftingItemName);
	
public:
	UFUNCTION()
	FEventReply OnCategoryBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnCategoryBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnCategoryBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	
private:
	ENotebookCraftingCategoryType GetHoveredCategoryType();
	void SetSelectCategory(ENotebookCraftingCategoryType HoveredType);
	void ClearCategorySelectImage();
	void CategoryClicked();
	void CategoryClear();
};






