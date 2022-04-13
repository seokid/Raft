// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "AddItemList.generated.h"

/**
 * 
 */


UCLASS()
class RAFTDEDICATED_API UAddItemList : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UAddItemInfo*>				m_ArrayAddItemInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32									m_AddItemInfoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float									m_AddItemInfoSpacing;

	TArray<class UAddItemInfo*>				m_ArrayAddItemInfoWidgetEnable;
	TArray<class UAddItemInfo*>				m_ArrayAddItemInfoWidgetWait;
	TArray<class UAddItemInfo*>				m_ArrayAddItemInfoWidgetDisable;
	
	//TQueue<class UAddItemInfo*>				m_QueAddItemInfoWidgetDisable;
	
	TQueue<std::pair<FString, int32>>		m_QueAddItemInfo;

	bool									m_ExcuteAddItemMove;

	float									m_AccTime;
	float									m_MaxTime;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void AddItem(const FString& ItemName, int32 ItemCount);
	void DisableAddItemInfo(class UAddItemInfo* Widget);

private:
	void ExcuteAddItemInfoWidgetAdd();
	void ExcuteAddItemInfoWidgetMove(float InDeltaTime);
	void ExcuteAddItemInfoWidgetEanble();
};

