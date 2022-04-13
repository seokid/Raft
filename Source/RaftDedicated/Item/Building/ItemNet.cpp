// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemNet.h"

AItemNet::AItemNet()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Item_Net/Item_Net.Item_Net'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = BlockAsset.Object->GetName();
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Bottom;
}

void AItemNet::BuildCompleted()
{
	Super::BuildCompleted();
}

void AItemNet::BreakCompleted()
{
	Super::BreakCompleted();

	Destroy();
}
