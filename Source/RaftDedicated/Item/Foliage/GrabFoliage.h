// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticFoliage.h"
#include "GrabFoliage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AGrabFoliage : public AStaticFoliage
{
	GENERATED_BODY()

protected:
	AGrabFoliage();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	
private:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
public:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
};
