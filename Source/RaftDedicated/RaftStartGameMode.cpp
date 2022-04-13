// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftStartGameMode.h"
#include "UI/StartHUD.h"
#include "RaftDedicatedGameInstance.h"

ARaftStartGameMode::ARaftStartGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> StartPawnClass(TEXT("Blueprint'/Game/Level/Start/BP_StartCamPawn.BP_StartCamPawn_C'"));

	if(StartPawnClass.Succeeded())
	{
		DefaultPawnClass = StartPawnClass.Class;
	}


	static ConstructorHelpers::FClassFinder<UStartHUD> StartHUDClass(TEXT("WidgetBlueprint'/Game/UI/StartHUD/UI_StartHUD.UI_StartHUD_C'"));

	if(StartHUDClass.Succeeded())
	{
		m_StartHUDWidgetClass = StartHUDClass.Class;
	}
}

void ARaftStartGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ARaftStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_StartHUDWidgetClass))
	{
		m_StartHUDWidget = Cast<UStartHUD>(CreateWidget(GetWorld(), m_StartHUDWidgetClass));
	
		if (m_StartHUDWidget)
		{
			m_StartHUDWidget->AddToViewport();
		}
	}

	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	
	FInputModeUIOnly Mode; // UI 조작 하는 것
		
	Controller->SetInputMode(Mode);
	Controller->SetShowMouseCursor(true);
	//FIntPoint ViewportSize = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();

	//LOG(TEXT("%d %d"), ViewportSize.X / 2, ViewportSize.Y / 2);
	// Controller->SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);

	URaftDedicatedGameInstance* GameInst = GetGameInstance<URaftDedicatedGameInstance>();
	if(nullptr != GameInst)
	{
		GameInst->PlayToBGM(TEXT("SoundWave'/Game/Sound/RAFT_EndlessOcean_.RAFT_EndlessOcean_'"));
	}
}
