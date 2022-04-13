// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"

#include "Rope.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ARope : public AEquipment
{
	GENERATED_BODY()

public:
	ARope();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCableComponent*	m_Cable;

public:
	void SetRopeLength(float Length);
public:
	void RopeConnect(AEquipment* Equip, FName SocketName = NAME_None);

public:
	virtual void BeginPlay() override;
	
};
