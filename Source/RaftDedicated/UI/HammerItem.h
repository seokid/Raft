// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "HammerItem.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UHammerItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UHammerItemList*				m_HammerItemListWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*								m_ItemIconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_ItemCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool								m_PartFull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString								m_ItemPartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32								m_ItemPartCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D*							m_Background_Bright;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D*							m_Background_Red;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetHammerItemListWidget(UHammerItemList* Widget) {m_HammerItemListWidget = Widget;}
	bool IsPartEnough() { return m_PartFull; }
	
public:
	void SetItemPart(const FString& ItemName, int32 ItemCount);
	void Building_Build();
	void ReloadItemCount();
	
private:
	void SetBackground();
	void SetItemIcon(UTexture2D* ItemIconTexture);
	void SetItemCount(int32 PlayerItemCount, int32 ItemCount);
	void SetPartFull();
};
