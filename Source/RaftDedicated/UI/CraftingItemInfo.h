// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/UserWidget.h"
#include "CraftingItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UCraftingItemInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_CraftingItemIconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_CraftingItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_CraftingItemDescText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBorder*							m_CraftingBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString								m_CraftingItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UCraftingItemPart*>	m_ArrayCraftingItemPartWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32								m_CraftingItemPartMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32								m_CraftingItemPartOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32								m_CraftingItemPartSpacing;

	class UCraftingList*				m_CraftingListWidget;

	bool								m_CraftingBoderPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D*							m_CraftButtonNormalTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D*							m_CraftButtonDisabledTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*							m_CraftingSound;
	
	bool								m_CraftingEnable;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	void SetCraftingListWidget(UCraftingList* Widget) {m_CraftingListWidget = Widget;}
	
	
public:
	UFUNCTION()
	FEventReply OnCraftingBorderPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnCraftingBorderReleased(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	FEventReply OnCraftingBorderHovered(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

public:
	void SetCraftingItemName(const FString& CraftingItemName);

private:
	void SetCraftingItemInfo();
	void SetCraftingItemIconImage(UTexture2D* CraftingItemIconTexture);
	void SetCraftingItemNameText(const FString&	CraftingItemName);
	void SetCraftingItemDescText(const FString&	CraftingItemDesc);
	void SetCraftingItemParts(const TArray<FCraftingItemParts>& ArrayParts);
	void SetBackgroundSize(int32 PartCount);
	void SetCraftingEnable();

	void ExcuteCraftingItem();
};





