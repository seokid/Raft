// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockStairWoodHalf.h"
#include "Boat.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ABlockStairWoodHalf::ABlockStairWoodHalf()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Stair_Wood/Block_Stair_Wood_Half_2.Block_Stair_Wood_Half_2'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;
	
	m_AddBuildingLayer = 1;
}

bool ABlockStairWoodHalf::IsBuildPossible()
{
	//return Super::IsBuildPossible();

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
				if(BuildingActor->GetBuildingInfo().m_BuildType == EBuildType::Install)
				{
					if(BuildingActor->GetBuildingInfo().m_Layer >= m_BuildingInfo.m_Layer)
					{
						return false;
					}
				}
			}
		}
	}
	return true;	
}

void ABlockStairWoodHalf::BuildCompleted()
{
	Super::BuildCompleted();

	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return;}

	int32 NewFloorIdxX = 0;
	int32 NewFloorIdxY = 0;

	GetNewIdxToLayer(NewFloorIdxX, NewFloorIdxY, m_AddBuildingLayer);
	
	BoatActor->AddBuildingLayer(m_AddBuildingLayer + m_BuildingInfo.m_Layer);

	SetBurnEffectEnable(true);
}

void ABlockStairWoodHalf::BreakCompleted()
{
	Super::BreakCompleted();

	SetBurnEffectEnable(true, -1.f);
}

void ABlockStairWoodHalf::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
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

void ABlockStairWoodHalf::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
}

void ABlockStairWoodHalf::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);

	m_MeshComponent->SetRenderCustomDepth(false);
	m_MeshComponent->SetCustomDepthStencilValue(255);
}
