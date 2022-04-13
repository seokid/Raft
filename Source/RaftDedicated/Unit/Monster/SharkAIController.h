// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIController.h"
#include "SharkAIController.generated.h"


UCLASS()
class RAFTDEDICATED_API ASharkAIController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	ASharkAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
