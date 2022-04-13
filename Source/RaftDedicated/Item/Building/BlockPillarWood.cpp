// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockPillarWood.h"
#include "Boat.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ABlockPillarWood::ABlockPillarWood()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Pillar_Wood/Block_Pilla_Wood.Block_Pilla_Wood'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;

	m_AddBuildingLayer = 2;
}

bool ABlockPillarWood::IsBuildPossible()
{
	//bool ParentResult = Super::IsBuildPossible();

	//if(false == ParentResult) {return false;}

	TArray<AActor*> ArrayResult;
	m_MeshComponent->GetOverlappingActors(ArrayResult, ABuilding::StaticClass());

	if(ArrayResult.Num() <= 0) {return true;}
	
	if(m_BuildingInfo.m_BuildType == EBuildType::Install)
	{
		for(auto result : ArrayResult)
		{
			ABuilding* BuildingActor = Cast<ABuilding>(result);
			if(BuildingActor)
			{
				if(BuildingActor->GetName() == GetName()) {return false;}
				if(BuildingActor->GetBuildingInfo().m_BuildType == EBuildType::Install)
				{
					if(BuildingActor->GetBuildingInfo().m_Layer == m_BuildingInfo.m_Layer)
					{
						return false;
					}
				}
				//if(BuildingActor->GetBuildingInfo().m_Layer != m_BuildingInfo.m_Layer) {return false;}
			}
		}
	}
	

	if(false == IsFloorExist()) {return false;}

	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return false;}

	for(int32 i = 0; i < m_AddBuildingLayer; ++i)
	{
		ABuilding* FloorActor = BoatActor->GetBuilding(m_BuildingInfo.m_Layer + i + 1, m_BuildingInfo.m_IdxX, m_BuildingInfo.m_IdxY);
		if(FloorActor) {return false;}
	}
	
	return true;
}

void ABlockPillarWood::BuildCompleted()
{
	Super::BuildCompleted();

	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return;}
	
	BoatActor->AddBuildingLayer(m_AddBuildingLayer + m_BuildingInfo.m_Layer, m_BuildingInfo.m_IdxX, m_BuildingInfo.m_IdxY);

	SetBurnEffectEnable(true);
}

void ABlockPillarWood::BreakCompleted()
{
	Super::BreakCompleted();

	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return;}
	
	BoatActor->BreakInstallLayer(m_AddBuildingLayer + m_BuildingInfo.m_Layer, m_BuildingInfo.m_IdxX, m_BuildingInfo.m_IdxY);

	SetBurnEffectEnable(true, -1.f);
}

void ABlockPillarWood::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
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

void ABlockPillarWood::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
}

void ABlockPillarWood::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);

	m_MeshComponent->SetRenderCustomDepth(false);
	m_MeshComponent->SetCustomDepthStencilValue(255);
}
