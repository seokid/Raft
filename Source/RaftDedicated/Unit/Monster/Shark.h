// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "PatrolSpline.h"
#include "Shark.generated.h"


UCLASS()
class RAFTDEDICATED_API AShark : public AMonster
{
	GENERATED_BODY()
	
public:
	AShark();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent*	m_Mouth;
	
	bool	m_bPatrol;
	float	m_Dir;
    
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void NormalAttack() override;

public:
	void OpenMouth();
	void Dash();
	void Swim();
};
