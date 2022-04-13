// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ItemInteraction.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UItemInteraction : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*		m_InteractionButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*		m_ItemDescText;
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetInteractionButtonText(const FString& Text) { m_InteractionButtonText->SetText(FText::FromString(Text)); }
	void SetIemDescText(const FString& Text) { m_ItemDescText->SetText(FText::FromString(Text)); }
};
