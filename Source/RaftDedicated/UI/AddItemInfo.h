// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "AddItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UAddItemInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UImage*								m_AddItemIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_AddItemNameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*							m_AddItemCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float								m_AccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float								m_MaxTime;

	int32								m_Idx;
	bool								m_ExcuteEnable;

	class UAddItemList*					m_AddItemListWidget;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	void SetIdx(int32 Idx) {m_Idx = Idx;}
	int32 GetIdx() {return m_Idx;}

	void SetAddItemListWidget(class UAddItemList* Widget) {m_AddItemListWidget = Widget;}
	
public:
	bool SetAddItemInfo(const FString& ItemName, int32 ItemCount);

private:
	void ExcuteEnable(float InDeltaTime);
};

