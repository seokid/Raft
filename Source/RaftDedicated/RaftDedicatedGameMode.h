// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaftDedicatedGameMode.generated.h"

UCLASS(minimalapi)
class ARaftDedicatedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARaftDedicatedGameMode();
	
protected:
	class ABoat* m_Boat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



