// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateList.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UPlayerStateList : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar*				m_ThirstProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar*				m_HungerProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar*				m_HealthProgressBar;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetStatePercent(float ThirstPercent, float HungerPercent, float HealthPercent);
	void SetThirstPercent(float Percent);
	void SetHungerPercent(float Percent);
	void SetHealthPercent(float Percent);
};
