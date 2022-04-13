// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AArrow : public AEquipment
{
	GENERATED_BODY()

	
public:
	AArrow();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* m_Body;

protected:
	float	m_Speed;

public:
	void SetInitSpeed(float Speed) { m_Movement->InitialSpeed = Speed; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	
	
};
