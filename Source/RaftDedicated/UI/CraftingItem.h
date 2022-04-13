// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../RaftDedicatedGameInstance.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CraftingItem.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UCraftingItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBorder*							m_CraftingItemIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_CraftingItemName_BG;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_CraftingItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString								m_CraftingItemName;

	class UCraftingList*				m_CraftingListWidget;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void SetCraftingListWidget(class UCraftingList* Widget) {m_CraftingListWidget = Widget;}
	const FString&	GetCraftingItemName() { return m_CraftingItemName;}
	UTexture2D* GetCraftingItemTexture() {return Cast<UTexture2D>(m_CraftingItemIconImage->Background.GetResourceObject());}
	
public:
	void SetCraftingItem(const FItemIconTableInfo* Info);
};

