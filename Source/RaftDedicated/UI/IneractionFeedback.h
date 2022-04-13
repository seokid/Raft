// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "IneractionFeedback.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UIneractionFeedback : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage*					m_FeedbackIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EFeedbackIconType, UTexture2D*>		m_MapFeedbackIconTypeTexture;	
		
protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetFeedbackIconType(EFeedbackIconType Type);
	
};
