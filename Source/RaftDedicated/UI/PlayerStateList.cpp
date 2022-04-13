// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateList.h"

void UPlayerStateList::NativeConstruct()
{
	Super::NativeConstruct();

	m_ThirstProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Thirst")));
	m_HungerProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Hunger")));
	m_HealthProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Health")));
}

void UPlayerStateList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerStateList::SetStatePercent(float ThirstPercent, float HungerPercent, float HealthPercent)
{
	SetThirstPercent(ThirstPercent);
	SetHungerPercent(HungerPercent);
	SetHealthPercent(HealthPercent);
}

void UPlayerStateList::SetThirstPercent(float Percent)
{
	m_ThirstProgressBar->SetPercent(Percent);
}

void UPlayerStateList::SetHungerPercent(float Percent)
{
	m_HungerProgressBar->SetPercent(Percent);
}

void UPlayerStateList::SetHealthPercent(float Percent)
{
	m_HealthProgressBar->SetPercent(Percent);
}
