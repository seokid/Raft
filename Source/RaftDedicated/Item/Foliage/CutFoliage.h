// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "StaticFoliage.h"
#include "CutFoliage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ACutFoliage : public AStaticFoliage
{
	GENERATED_BODY()
public:
	ACutFoliage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent*	m_Body;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_GatherCount;

	
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Gather(class APlayerCharacter* Player) override;

public:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
};
