// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CraftingItemPart.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UCraftingItemPart : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_CraftingItemIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_CraftingItemTextBacbgroundImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_CraftingItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_CraftingItemCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float								m_CraftingDisableOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector4							m_CraftingDisableColor;
	
	bool								m_PartFull;
	FString								m_PartName;
	int32								m_PartCount;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	bool IsPartEnough() { return m_PartFull; }
	int32 GetPartCount() { return m_PartCount; }
	const FString& GetPartName() {return m_PartName;}

public:
	bool SetCraftingItemPart(const FString& Name, int32 PlayerItemCount, int32 ItemPartCount);
	
private:
	bool SetCraftingItemNameText(const FString& Name);
	void SetCraftingItemIconImage(UTexture2D* ImageTexture);
	bool SetCraftingItemCount(int32 PlayerItemCount, int32 ItemPartCount);
	void SetPartEnough();
};
