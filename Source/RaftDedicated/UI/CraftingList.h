// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CraftingList.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UCraftingList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*							m_BackgroundImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*						m_CategoryNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*							m_CategoryIconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UCraftingItem*>	m_ArrayCraftingItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCraftingItemInfo*		m_CraftingItemInfoWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*							m_SelectImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_CraftingItemSpacing;

	ENotebookCraftingCategoryType	m_CategoryType;

	class UNotebookCrafting*		m_NotebookCraftingWidget;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	ENotebookCraftingCategoryType GetCategoryType() {return m_CategoryType;}
	void SetNotebookCraftingWidget(class UNotebookCrafting* Widget) {m_NotebookCraftingWidget = Widget;}
	UNotebookCrafting* GetNotebookCraftingWidget() {return m_NotebookCraftingWidget;}
	
public:
	void SetCategoryInfo(ENotebookCraftingCategoryType CategoryType, UTexture2D* CategoryIconImage);
	void SetCraftingList(const TArray<FString>& CraftingArray);

	void OnCraftingItemMouseEnter(class UCraftingItem* ItemWidget);
	void OnCraftingItemMouseLeave(class UCraftingItem* ItemWidget);
	void OnCraftingItemMouseDown(class UCraftingItem* ItemWidget);
	
private:
	void SetCategoryNameText();
	void SetBackgroundSize(int32 ListCount);
	
};

