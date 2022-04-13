// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Building.h"
#include "ItemNet.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AItemNet : public ABuilding
{
	GENERATED_BODY()

public:
	AItemNet();

public:
	virtual void BuildCompleted() override;
	virtual void BreakCompleted() override;

};
