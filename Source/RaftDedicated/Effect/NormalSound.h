// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "NormalSound.generated.h"

UCLASS()
class RAFTDEDICATED_API ANormalSound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalSound();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent*		m_AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_Roop;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void LoadSound(const FString& Path);
	void LoadSoundAsync(const FString& Name);
	void LoadSoundAsyncComplete();

	void Play(float Volume = 1.f, bool Roop = false, bool BGM = false);
	void Stop();
	void Pause();
	
public:
	UFUNCTION()
	void SoundFinished();
};
