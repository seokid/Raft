// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	
	SetRootComponent(m_Body);
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
    StaticMesh->SetCollisionProfileName(TEXT("Equipment"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Arrow_Stone.SM_Arrow_Stone'"));
	if (ArrowAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(ArrowAsset.Object);
	}
	m_Mesh = StaticMesh;
	m_Mesh->SetupAttachment(m_Body);

	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	m_Movement->SetUpdatedComponent(m_Body);
	m_Movement->InitialSpeed = 100.f;
	
	m_SocketName = TEXT("hand_l_arrow");
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

