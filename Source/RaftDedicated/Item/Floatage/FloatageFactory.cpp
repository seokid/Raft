// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatageFactory.h"
#include "Floatage.h"

// Sets default values
AFloatageFactory::AFloatageFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PoolCount = 0;
	
	m_InitExpansionCount = 100;
	m_ExpansionCount = 20;

	m_InitSpawnCount = 100;
	m_SpawnCount = 10;
	
	m_SpawnLifeTime = 1.f;

	m_Range = {100.f, 100.f};
	m_WaveDir = {1.f, 0.f, 0.f};
	
	m_AccTime = 0.f;
}

// Called when the game starts or when spawned
void AFloatageFactory::BeginPlay()
{
	Super::BeginPlay();
	Spawn(m_InitSpawnCount);
}

// Called every frame
void AFloatageFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_SpawnFloatages.Num())
		return;

	m_AccTime += DeltaTime;

	if (m_SpawnLifeTime <= m_AccTime)
	{
		m_AccTime -= m_SpawnLifeTime;
		Spawn(m_SpawnCount);
	}
}

void AFloatageFactory::Return(AFloatage* Floatage)
{
	Floatage->SetActive(false);
	m_SpawnPool.Push(Floatage);
}

void AFloatageFactory::Spawn(int32 Count)
{
	while (Count > m_SpawnPool.Num())
	{
		Expansion(m_ExpansionCount);
	}

	for (int32 i = 0 ; i < Count; ++i)
	{
		if (AFloatage* Floatage = m_SpawnPool.Pop())
		{
			FVector Loc = GetActorLocation();
			Loc.X += FMath::RandRange(-m_Range.X, m_Range.X);
			Loc.Y += FMath::RandRange(-m_Range.Y, m_Range.Y);
			
			Floatage->SetActorLocation(Loc);
			Floatage->SetActive(true);
		}
	}
}

void AFloatageFactory::Expansion(int32 Count)
{
	m_PoolCount += Count;
	
	for (int32 i = 0 ; i < Count; ++i)
	{
		const int32 Idx = FMath::RandRange(0, m_SpawnFloatages.Num() - 1);

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		if (AFloatage* Floatage = GetWorld()->SpawnActor<AFloatage>(m_SpawnFloatages[Idx], FVector::ZeroVector, FRotator::ZeroRotator, param))
		{
			Floatage->SetFactory(this);
			Floatage->SetActive(false);
			m_SpawnPool.Push(Floatage);
		}
	}
}

