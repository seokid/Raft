// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockFloorWood.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ABlockFloorWood::ABlockFloorWood()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Floor_Wood/Block_Floor_Wood.Block_Floor_Wood'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	m_BuildingInfo.m_Layer = 1;
}

bool ABlockFloorWood::IsBuildPossible()
{
	return true;
}

void ABlockFloorWood::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
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

void ABlockFloorWood::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
}

void ABlockFloorWood::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);

	m_MeshComponent->SetRenderCustomDepth(false);
	m_MeshComponent->SetCustomDepthStencilValue(255);
}
