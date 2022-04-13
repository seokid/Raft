// Fill out your copyright notice in the Description page of Project Settings.


#include "CutFoliage.h"

#include "RaftDedicated/Item/Equipment/Axe.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ACutFoliage::ACutFoliage()
{
	PrimaryActorTick.bCanEverTick = true;

	
	m_Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	m_Body->SetupAttachment(GetRootComponent());
	m_Body->SetCollisionProfileName(TEXT("Foliage"));
	
	m_Mesh->SetupAttachment(m_Body);
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_IconType = EFeedbackIconType::Axe;
	m_GatherCount = 1;
}

void ACutFoliage::BeginPlay()
{
	Super::BeginPlay();
}

void ACutFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACutFoliage::Gather(APlayerCharacter* Player)
{
	--m_GatherCount;
	if (0 >= m_GatherCount)
	{
		Super::Gather(Player);

		m_Mesh->SetRenderCustomDepth(false);
		m_Mesh->SetCustomDepthStencilValue(255);
	}
}

void ACutFoliage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);
	if(nullptr != Player->GetEquipment() && Cast<AAxe>(Player->GetEquipment()))
	{
		m_Mesh->SetRenderCustomDepth(true);
		m_Mesh->SetCustomDepthStencilValue(255);
	}
}

void ACutFoliage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	m_Mesh->SetRenderCustomDepth(false);
	m_Mesh->SetCustomDepthStencilValue(255);
}
