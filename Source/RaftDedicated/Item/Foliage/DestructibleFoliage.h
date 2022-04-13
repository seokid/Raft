// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foliage.h"
#include "DestructibleComponent.h"
#include "DestructibleFoliage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ADestructibleFoliage : public AFoliage
{
	GENERATED_BODY()

public:
	ADestructibleFoliage();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDestructibleComponent*		m_Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool						m_bDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float						m_EffectSpeed;
	
	float						m_EffectAccTime;
	float						m_EffectMaxTime;
	float						m_EffectBoundary;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Gather(APlayerCharacter* Player, float Damage, float Impulse);

private:
	UFUNCTION()
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);

protected:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

private:
	virtual void Gather(APlayerCharacter* Player) override final {}
	void SetBurnEffectEnable(bool Enable, float EffectSpeed);
	void ExecuteBurnEffect(float DeltaTime);
	void EffectFinished();

};


