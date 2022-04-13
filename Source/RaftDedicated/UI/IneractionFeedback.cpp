// Fill out your copyright notice in the Description page of Project Settings.


#include "IneractionFeedback.h"

void UIneractionFeedback::NativeConstruct()
{
	Super::NativeConstruct();

	m_FeedbackIconImage = Cast<UImage>(GetWidgetFromName(TEXT("FeedbackIconImage")));
}

void UIneractionFeedback::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UIneractionFeedback::SetFeedbackIconType(EFeedbackIconType Type)
{
	if(EFeedbackIconType::None == Type)
	{
		if(ESlateVisibility::Collapsed != GetVisibility())
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		m_FeedbackIconImage->SetBrushFromTexture(m_MapFeedbackIconTypeTexture[Type]);
		if(ESlateVisibility::Collapsed == GetVisibility())
		{
			SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
