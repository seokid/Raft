// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"

AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	// SetRootComponent(m_RootComponent);

	m_Mesh = nullptr;
	
	m_Owner = nullptr;
	m_SocketName = NAME_None;
	m_ChargingTime = 0.f;
	m_bCharging = false;
	m_bChargingFull = false;
	m_ChargingAccTime = 0.f;
}

void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipment::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_bCharging)
	{
		m_ChargingAccTime += DeltaSeconds;

		if (!m_bChargingFull && 1.f <= m_ChargingAccTime / m_ChargingTime)
		{
			FullCharging();
		}
	}
}

float AEquipment::GetChargingRatio() const
{
	if (0.f >= m_ChargingTime)
		return 0.f;
	
	return FMath::Min(m_ChargingAccTime / m_ChargingTime, 1.f);
}

bool AEquipment::OnEquip(APlayerCharacter* EquipOwner)
{
	if (!EquipOwner || m_SocketName.IsNone() || !m_Mesh || !EquipOwner->GetAnim() )
		return false;
	
	m_Owner = EquipOwner;
	AttachItem(m_Owner->GetMesh());
	
	m_OwnerAnim = m_Owner->GetAnim();
	m_OwnerAnim->Montage_SetPosition(m_Owner->GetEquipInfo().Montage, 0.f);
	m_OwnerAnim->Montage_Play(m_Owner->GetEquipInfo().Montage);
	return true;
}

bool AEquipment::OnEquip(APlayerCharacter* EquipOwner, UMeshComponent* Mesh)
{
	if (!EquipOwner || m_SocketName.IsNone() || !m_Mesh || !EquipOwner->GetAnim() )
		return false;

	
	m_Owner = EquipOwner;
	m_OwnerAnim = m_Owner->GetAnim();
	
	AttachItem(Mesh);
	return true;
}

void AEquipment::UnEquip()
{
	if (m_Mesh)
	{
		m_Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	m_ChargingAccTime = 0.f;
	m_OwnerAnim = nullptr;
	m_Owner = nullptr;
	Destroy();
}


void AEquipment::BeginCharging()
{
	if (0.f >= m_ChargingTime)
		return;
		
	m_bCharging = true;
	m_bChargingFull = false;
}

void AEquipment::EndCharging()
{
	
	m_bCharging = false;
	m_ChargingAccTime = 0.f;
}

void AEquipment::FullCharging()
{
	m_bChargingFull = true;
}


void AEquipment::Use()
{
	if (m_bCharging)
	{
		EndCharging();
	}

}

void AEquipment::UseEnd()
{
}

void AEquipment::AttachItem(UMeshComponent* Mesh)
{
	AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, m_SocketName);
	SetActorRelativeLocation(FVector::ZeroVector);
}

