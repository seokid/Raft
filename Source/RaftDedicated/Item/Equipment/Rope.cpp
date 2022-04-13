// Fill out your copyright notice in the Description page of Project Settings.


#include "Rope.h"
#include "CableComponent/Classes/CableComponent.h"

ARope::ARope()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rope"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RopeAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Hook_Rope.SM_Hook_Rope'"));
	if (RopeAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(RopeAsset.Object);
	}
	m_Mesh = StaticMesh;
	
	m_SocketName = TEXT("hand_l_equip");

	m_Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	m_Cable->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> RopeMtrlAsset(TEXT("Material'/Game/Item/Equipment/Weapon/Material/M_Rope.M_Rope'"));
	if (RopeMtrlAsset.Succeeded())
	{
		m_Cable->SetMaterial(0, RopeMtrlAsset.Object);
	}
	
	m_Cable->EndLocation = FVector::ZeroVector;
	m_Cable->CableLength = 10.f;
	m_Cable->SolverIterations = 3;
	m_Cable->NumSegments = 5;
	m_Cable->CableWidth = 5.f;
	m_Cable->NumSides = 5;
	m_Cable->TileMaterial = 10.f;
}

void ARope::BeginPlay()
{
	Super::BeginPlay();
	m_Cable->AttachToComponent(m_Mesh, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
}

void ARope::SetRopeLength(float Length)
{
	m_Cable->CableLength = Length;
}




void ARope::RopeConnect(AEquipment* Equip, FName SocketName)
{
	if (Equip)
		m_Cable->SetAttachEndTo(Equip,TEXT("m_Mesh"), SocketName);
}
