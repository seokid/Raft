// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Building.h"
#include "BlockFloorWood.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABlockFloorWood : public ABuilding
{
	GENERATED_BODY()
public:
	ABlockFloorWood();

public:
	virtual bool IsBuildPossible() override;

public: // 플레이어의 레이와 충돌
	virtual void OnCollisionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollision(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
};
