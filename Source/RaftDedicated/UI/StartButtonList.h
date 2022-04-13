// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "StartButtonList.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EStartButtonType : uint8
{
	GameStart,
	Development,
	GameExit,

	End,
};

UCLASS()
class RAFTDEDICATED_API UStartButtonList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UButton*>		m_ArrayButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UTextBlock*>		m_ArrayText;

	UPROPERTY()
	class UStartHUD*		m_StartHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_HoveredSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_ClickedSound;

	EStartButtonType		m_HoveredButtonType;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetStartHUD(class UStartHUD* StartHUDWidget) { m_StartHUDWidget = StartHUDWidget; }
	
public:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void StartButtonHovered();

	UFUNCTION()
	void StartButtonUnHovered();

private:
	EStartButtonType GetHoveredButtonIdx();
	void ExecuteStartButtonClicked(EStartButtonType Type);
	void ExecuteStartButtonHovered(EStartButtonType Type);
	void ExecuteStartButtonUnHovered(EStartButtonType Type);

	void SetTextFontOutLineColorA(EStartButtonType Type, float ColorA);
};




