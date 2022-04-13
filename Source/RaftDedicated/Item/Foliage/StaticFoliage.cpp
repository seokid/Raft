// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticFoliage.h"

AStaticFoliage::AStaticFoliage()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));;
	m_Mesh->SetupAttachment(m_RootComponent);
	m_Mesh->SetCollisionProfileName(TEXT("Foliage"));
}

void AStaticFoliage::BeginPlay()
{
	Super::BeginPlay();
}

void AStaticFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStaticFoliage::Gather(APlayerCharacter* Player)
{
	Super::Gather(Player);
	Destroy();
}