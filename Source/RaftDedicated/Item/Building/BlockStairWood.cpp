// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockStairWood.h"
#include "Boat.h"
#include "ComponentUtils.h"
#include "../../RaftDedicatedCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ABlockStairWood::ABlockStairWood()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Stair_Wood/Block_Stair_Wood.Block_Stair_Wood'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;

	m_AddBuildingLayer = 2;

	m_ColliderParentComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ColliderParent"));
	m_ColliderParentComponent->SetupAttachment(m_RootComponent);
}

void ABlockStairWood::BeginPlay()
{
	Super::BeginPlay();
}

void ABlockStairWood::SetBuildingStateType(const EBuildingStateType& StateType)
{
	Super::SetBuildingStateType(StateType);
	m_MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

bool ABlockStairWood::IsBuildPossible()
{
	TArray<USceneComponent*> ArrayBoxCollider = m_ColliderParentComponent->GetAttachChildren();
	for(auto Box : ArrayBoxCollider)
	{
		TArray<AActor*> ArrayResult;
		UBoxComponent* BoxCom = Cast<UBoxComponent>(Box);
		if(BoxCom)
		{
			BoxCom->GetOverlappingActors(ArrayResult, ABuilding::StaticClass());
			if(ArrayResult.Num() > 0)
			{
					
				for(auto result : ArrayResult)
				{
					ABuilding* BuildingActor = Cast<ABuilding>(result);
					if(GetName() == BuildingActor->GetName()) {continue;}
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
	}

	
	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return false;}
	ABuilding* FloorActor = BoatActor->GetBuilding(m_BuildingInfo.m_Layer, m_BuildingInfo.m_IdxX, m_BuildingInfo.m_IdxY);

	if(IsFloorExist())
	{
		if(2 == m_AddBuildingLayer)
		{
			float CurYaw = GetRootComponent()->GetRelativeRotation().Yaw + 180.f;
		
			int32 NewFloorIdxX = 0;
			int32 NewFloorIdxY = 0;

			GetNewIdxToLayer(NewFloorIdxX, NewFloorIdxY, m_AddBuildingLayer - 1);
		
			FloorActor = BoatActor->GetBuilding(m_BuildingInfo.m_Layer, NewFloorIdxX, NewFloorIdxY); 
			if(FloorActor && FloorActor->GetBuildingInfo().m_StateType == EBuildingStateType::Build)
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	
	return false;
}



void ABlockStairWood::BuildCompleted()
{
	Super::BuildCompleted();

	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return;}

	int32 NewFloorIdxX = 0;
	int32 NewFloorIdxY = 0;

	GetNewIdxToLayer(NewFloorIdxX, NewFloorIdxY, m_AddBuildingLayer);
	
	BoatActor->AddBuildingLayer(m_AddBuildingLayer + m_BuildingInfo.m_Layer);

	m_MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
		
	TArray<USceneComponent*> ArrayBoxCollider = m_ColliderParentComponent->GetAttachChildren();
	for(auto Box : ArrayBoxCollider)
	{
		UBoxComponent* BoxCom = Cast<UBoxComponent>(Box);
		if(BoxCom)
		{
			BoxCom->SetCollisionProfileName(TEXT("Building"));
		}
	}

	SetBurnEffectEnable(true);
}

void ABlockStairWood::BreakCompleted()
{
	Super::BreakCompleted();
	
	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return;}
	
	int32 NewFloorIdxX = 0;
	int32 NewFloorIdxY = 0;

	GetNewIdxToLayer(NewFloorIdxX, NewFloorIdxY, m_AddBuildingLayer);
	
	//BoatActor->BreakInstallLayer(m_AddBuildingLayer + m_BuildingInfo.m_Layer, NewFloorIdxX, NewFloorIdxY);
	SetBurnEffectEnable(true, -1.f);
}

void ABlockStairWood::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
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

void ABlockStairWood::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
}

void ABlockStairWood::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	
	m_MeshComponent->SetRenderCustomDepth(false);
	m_MeshComponent->SetCustomDepthStencilValue(255);
}
