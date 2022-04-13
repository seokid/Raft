// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Building.h"
#include "BlockFoundation.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABlockFoundation : public ABuilding
{
	GENERATED_BODY()
	
public:
	ABlockFoundation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBuoyancyComponent*	m_Buoyancy;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public: // 플레이어의 레이와 충돌
	virtual void OnCollisionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollision(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

public:
	virtual void BuildCompleted() override;
	virtual void BreakCompleted() override;
};


