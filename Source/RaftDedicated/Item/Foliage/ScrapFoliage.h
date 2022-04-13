// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "DestructibleFoliage.h"
#include "ScrapFoliage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AScrapFoliage : public ADestructibleFoliage
{
	GENERATED_BODY()
	
public:
    AScrapFoliage();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    float						m_GatherTime;

public:
    float GetGatherTime() const { return m_GatherTime; }
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    virtual void Gather(APlayerCharacter* Player, float Damage, float Impulse) override;

public:
    virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
    virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

};


