// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"


// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_AudioComponent = CreateDefaultSubobject<UDataTableAudioComponent>(TEXT("Audio"));
	m_AudioComponent->SetupAttachment(RootComponent);
	m_AudioComponent->bOverrideAttenuation = true;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}