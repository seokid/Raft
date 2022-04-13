// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "RaftStartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ARaftStartGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARaftStartGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStartHUD>		m_StartHUDWidgetClass;

	class UStartHUD*					m_StartHUDWidget;
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
