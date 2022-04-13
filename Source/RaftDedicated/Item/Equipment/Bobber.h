// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "Bobber.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABobber : public AEquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UBuoyancyComponent*	m_Buoynacy;

public:
	ABobber();

protected:
	virtual void BeginPlay() override;
	
public:
	void AttachToMove(UMeshComponent* Mesh, float OverTime, const FVector& Location, const FRotator& Rotation, FName SocketName = NAME_None);
	void Throw(FVector Velocity);
	
public:
    UFUNCTION()
    virtual void BobberBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
    , UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void BobberEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
    	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
