// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalSound.h"

// Sets default values
ANormalSound::ANormalSound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	m_AudioComponent->OnAudioFinished.AddDynamic(this, &ANormalSound::SoundFinished);
	m_AudioComponent->bOverrideAttenuation = true;
	SetRootComponent(m_AudioComponent);
	m_Roop = false;
}

// Called when the game starts or when spawned
void ANormalSound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANormalSound::LoadSound(const FString& Path)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);
	
	if (Sound)
	{
		m_AudioComponent->SetSound(Sound);
		//m_AudioComponent->Play();
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
}

void ANormalSound::LoadSoundAsync(const FString& Name)
{
}

void ANormalSound::LoadSoundAsyncComplete()
{
}

void ANormalSound::Play(float Volume, bool Roop, bool BGM)
{
	m_AudioComponent->SetVolumeMultiplier(Volume);
	m_Roop = Roop;

	m_AudioComponent->bOverrideAttenuation = !BGM;
	m_AudioComponent->bIsUISound = BGM;
	
	m_AudioComponent->Play();
}

void ANormalSound::Stop()
{
	m_Roop = false;
	m_AudioComponent->Stop();
}

void ANormalSound::Pause()
{
	m_AudioComponent->SetPaused(true);
}

void ANormalSound::SoundFinished()
{
	if(m_Roop)
	{
		m_AudioComponent->Play();
	}
	else
	{
		Destroy();
	}
}

