// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Storage.h"
#include "StorageBigger.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AStorageBigger : public AStorage
{
	GENERATED_BODY()

public:
	AStorageBigger();

public:
	virtual void BuildCompleted() override;
	virtual void BreakCompleted() override;
	
protected:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
protected:
	virtual void StorageAction(float DeltaTime) override;
};
