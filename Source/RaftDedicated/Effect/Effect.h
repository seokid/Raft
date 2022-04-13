// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Effect.generated.h"

UCLASS()
class RAFTDEDICATED_API AEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffect();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent*	m_ParticleSystem;

	bool						m_bLoopEffect;

public:
	void LoadParticle(const FString& Path);
	
	void LoadSound(const FString& Path);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION()	//델리게이트 사용하려면 붙여야함
	void ParticleFinish(UParticleSystemComponent* ParticleSystem);

};
