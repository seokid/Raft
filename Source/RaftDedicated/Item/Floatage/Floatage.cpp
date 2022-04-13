// Fill out your copyright notice in the Description page of Project Settings.


#include "Floatage.h"
#include "BuoyancyComponent.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "FloatageFactory.h"

AFloatage::AFloatage()
{
	PrimaryActorTick.bCanEverTick = true;
	
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(m_Mesh);

	m_Mesh->SetSimulatePhysics(true);
	m_Mesh->SetLinearDamping(0.5f);
	m_Mesh->SetCollisionProfileName(TEXT("Floatage"));
	
	m_Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Bouyancy"));
	m_Buoyancy->AddCustomPontoon(100.f, FVector::ZeroVector);
	m_Factory = nullptr;
}


void AFloatage::SetActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
}

void AFloatage::BeginPlay()
{
	Super::BeginPlay();
}

void AFloatage::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!m_Factory)
		return;
	
	if (!m_Mesh->IsSimulatingPhysics())
		return;
	
	if (100.f > m_Mesh->GetPhysicsLinearVelocity().Size())
	{
		m_Mesh->AddForce(m_Factory->GetWaveDir() * 100.f * m_Mesh->GetBodyInstance()->GetMassOverride());
	}
}

void AFloatage::TakeItem(APlayerCharacter* Player)
{
	AddItemToPlayer(Player, m_ArrayItemDropInfo);
	
	m_Factory->Return(this);
}

void AFloatage::OnInteractionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionEnter(Player, HitComponent);

	UAnimMontage* Montage = Player->GetGrabMontage();
	if (Montage)
	{
		Player->GetAnim()->Montage_SetPosition(Player->GetGrabMontage(), 0.f);
		Player->GetAnim()->Montage_Play(Player->GetGrabMontage());
	}
	TakeItem(Player);
}

void AFloatage::OnInteractionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionExit(Player, HitComponent);
}

void AFloatage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);
	Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Pick Up Item"));
}


void AFloatage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	Player->SetItemInteractionUIText(false);
}

