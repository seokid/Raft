// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Foliage.h"
#include "StaticFoliage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AStaticFoliage : public AFoliage
{
	GENERATED_BODY()
public:
	AStaticFoliage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*				m_Mesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Gather(class APlayerCharacter* Player);
};


