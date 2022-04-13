// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockFoundation.h"
#include "BuoyancyComponent.h"
#include "RaftDedicated/Effect/NormalSound.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ABlockFoundation::ABlockFoundation()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Foundation/Block_Foundation.Block_Foundation'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);

	m_Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Bouyancy"));
    m_Buoyancy->AddCustomPontoon(100.f, FVector::ZeroVector);
	
	m_BuildingInfo.m_BuildType = EBuildType::Bottom;

	m_MaxTime = 5.f;
}

void ABlockFoundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlockFoundation::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);

	if (m_BuildingState != EBuildingState::Build)
		return;
	
	if(nullptr != Player->GetEquipment() && Cast<AToolHammer>(Player->GetEquipment()))
	{
		m_MeshComponent->SetRenderCustomDepth(true);
		m_MeshComponent->SetCustomDepthStencilValue(255);
	}
}

void ABlockFoundation::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
}

void ABlockFoundation::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	
	m_MeshComponent->SetRenderCustomDepth(false);
	m_MeshComponent->SetCustomDepthStencilValue(255);
}

void ABlockFoundation::BuildCompleted()
{
	Super::BuildCompleted();

	LOG(TEXT("BurnEffect Enable"));

	SetBurnEffectEnable(true);

	//SoundManager::GetInstance()->LoadSound(TEXT("SoundWave'/Game/Sound/1Up.1Up'"), m_Owner->GetActorLocation(), GetWorld());

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), GetActorRotation(), param);
	// Effect->LoadSound(TEXT("SoundWave'/Game/Sound/1Up.1Up'"));
	// Effect->Play();
}

void ABlockFoundation::BreakCompleted()
{
	Super::BreakCompleted();

	SetBurnEffectEnable(true, -1.f);
}
