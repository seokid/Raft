// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrapFoliage.h"

#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"
#include "../../Effect/Effect.h"
#include "RaftDedicated/Item/Equipment/Hook.h"

AScrapFoliage::AScrapFoliage()
{
	PrimaryActorTick.bCanEverTick = true;

	m_IconType = EFeedbackIconType::Hook;
	m_GatherTime = 1.f;
}

void AScrapFoliage::BeginPlay()
{
	Super::BeginPlay();
}

void AScrapFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AScrapFoliage::Gather(APlayerCharacter* Player, float Damage, float Impulse)
{
	Super::Gather(Player, Damage, Impulse);

	m_Mesh->SetRenderCustomDepth(false);
	m_Mesh->SetCustomDepthStencilValue(255);

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEffect* Effect = GetWorld()->SpawnActor<AEffect>(AEffect::StaticClass(), GetActorLocation(), GetActorRotation(), param);
		
	Effect->LoadParticle(TEXT("ParticleSystem'/Game/Effect/Luos8Elements/Particles/Wind/Par_Dust.Par_Dust'"));
	Effect->SetActorScale3D(FVector(1.f, 1.f, 1.f));
}

void AScrapFoliage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);

	if(nullptr != Player->GetEquipment() && Cast<AHook>(Player->GetEquipment()))
	{
		m_Mesh->SetRenderCustomDepth(true);
		m_Mesh->SetCustomDepthStencilValue(255);
	}
}

void AScrapFoliage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);

	m_Mesh->SetRenderCustomDepth(false);
	m_Mesh->SetCustomDepthStencilValue(255);
}
