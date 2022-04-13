// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "HammerItemList.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UHammerItemList : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMainHUD*						m_MainHUDWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UHammerItem*>			m_ArrayHammerItemWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32								m_HammerItemMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float								m_HammerItemSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float								m_HammerItemOffset;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetMainHudWidget(class UMainHUD* MainHUD) { m_MainHUDWidget = MainHUD; }
	UMainHUD* GetMainHUDWidget() {return m_MainHUDWidget;}
	
public:
	void SetHammerItem(const FString& ItemName);
	bool IsBuildPossible();
	void Building_Build();
	void ReloadItemCount();

private:
	void SetHammerItemWidgetPosition(int32 ItemCount);
};
