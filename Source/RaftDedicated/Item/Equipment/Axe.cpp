// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "../Foliage/CutFoliage.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"

AAxe::AAxe()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe"));
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	m_Mesh = StaticMesh;
	SetRootComponent(m_Mesh);

	m_AudioComponent = CreateDefaultSubobject<UDataTableAudioComponent>(TEXT("Audio"));
	if (m_AudioComponent)
	{
		m_AudioComponent->SetupAttachment(GetRootComponent());
		m_AudioComponent->bOverrideAttenuation = true;
		m_AudioComponent->bAutoActivate = false;
	}
	
	m_Type = EAxeType::Idle;
	m_CutFoliage = nullptr;
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();
}

void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AAxe::Use()
{
	Super::Use();

	ACutFoliage* Foliage = Cast<ACutFoliage>(m_Owner->GetInteractionActor());
	if (Foliage)
	{
		Foliage->Gather(m_Owner);
		m_AudioComponent->PlayDataTableSound(TEXT("Impact"));
	}
}

void AAxe::UseEnd()
{
	Super::UseEnd();

	m_Type = EAxeType::Idle;
}

void AAxe::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);

	if (bPress && EAxeType::Idle == m_Type)
    {
		m_OwnerAnim->Montage_SetPosition(m_OwnerAnim->GetCurrentActiveMontage(), 0.f);
		m_OwnerAnim->Montage_JumpToSection(TEXT("Hit"), m_OwnerAnim->GetCurrentActiveMontage());
		m_AudioComponent->PlayDataTableSound(TEXT("Wield"));
		m_Type = EAxeType::Hit;
    }
}
