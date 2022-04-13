// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodContainer.h"
#include "Food.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"

AFoodContainer::AFoodContainer()
{
	PrimaryActorTick.bCanEverTick = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);
	
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodContainer"));
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"));
	m_Mesh = StaticMesh;
	m_Mesh->SetupAttachment(m_RootComponent);

	m_Food = nullptr;
}

void AFoodContainer::BeginPlay()
{
	Super::BeginPlay();
}

void AFoodContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFoodContainer::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);
	
	if (bPress)
		m_OwnerAnim->Montage_JumpToSection(TEXT("Eat"), m_OwnerAnim->GetCurrentActiveMontage());
}



void AFoodContainer::Use()
{
	Super::Use();

	if (!m_Food)
		return;
	
	m_Food->Use();
	ThrowOut();
}

void AFoodContainer::PutIn(AFood* Food)
{
	//내용물이 있으면
	if (m_Food)
		return;
	
	m_Food = Food;
	m_Food->OnEquip(m_Owner, m_Mesh);
}

void AFoodContainer::ThrowOut()
{
	if (!m_Food)
		return;
	
	m_Food = nullptr;
}
