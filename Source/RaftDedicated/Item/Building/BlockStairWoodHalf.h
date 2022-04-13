// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "BlockStairWoodHalf.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABlockStairWoodHalf : public ABuilding
{
	GENERATED_BODY()

public:
	ABlockStairWoodHalf();
	
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
