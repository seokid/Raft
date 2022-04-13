// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "BlockPillarWoodHalf.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABlockPillarWoodHalf : public ABuilding
{
	GENERATED_BODY()

public:
	ABlockPillarWoodHalf();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_AddBuildingLayer;

public:
	virtual bool IsBuildPossible() override;
	virtual void BuildCompleted() override;
	virtual void BreakCompleted() override;

public: // 플레이어의 레이와 충돌
	virtual void OnCollisionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollision(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
};
