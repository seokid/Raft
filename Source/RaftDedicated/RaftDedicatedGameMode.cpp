// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaftDedicatedGameMode.h"
#include "Unit/PlayerCharacter/PlayerCharacterController.h"
#include "UObject/ConstructorHelpers.h"
#include "RaftDedicatedGameInstance.h"

ARaftDedicatedGameMode::ARaftDedicatedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Unit/Player/Male/BP_Male.BP_Male_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = APlayerCharacterController::StaticClass();
}

void ARaftDedicatedGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	
	FInputModeGameOnly Mode; // 디폴트 마우스 조작안됨
	Controller->SetInputMode(Mode);
	Controller->SetShowMouseCursor(false);


	URaftDedicatedGameInstance* GameInst = GetGameInstance<URaftDedicatedGameInstance>();
	if(nullptr != GameInst)
	{
		GameInst->PlayToBGM(TEXT("SoundWave'/Game/Sound/RAFT_BUILDINGANDCRAFTING.RAFT_BUILDINGANDCRAFTING'"));
	}
}
