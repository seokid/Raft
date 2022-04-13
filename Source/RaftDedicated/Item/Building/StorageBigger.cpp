// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageBigger.h"
#include "Kismet/KismetMathLibrary.h"

AStorageBigger::AStorageBigger()
{
	for(auto DoorMeshCompoent : m_StorageDoorMeshComponent)
	{
		DoorMeshCompoent->DestroyComponent();
	}
	m_StorageDoorMeshComponent.Empty();

	for(auto JointCompoent : m_JointComponent)
	{
		JointCompoent->DestroyComponent();
	}
	m_JointComponent.Empty();
	
	m_AngleMax.Empty();
	
	// Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StorageAsset(TEXT("StaticMesh'/Game/Item/Building/Storage/Storage_Bigger.Storage_Bigger'"));

	if(StorageAsset.Succeeded())
	{
		m_Mesh = StorageAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = TEXT("Storage_Bigger");
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);

	// Joint Left, Door Left
	m_JointComponent.Add(CreateDefaultSubobject<USceneComponent>(TEXT("JointLeft")));
	m_StorageDoorMeshComponent.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StorageDoorL")));
	m_JointComponent[0]->SetupAttachment(m_RootComponent);
	m_StorageDoorMeshComponent[0]->SetupAttachment(m_JointComponent[0]);
	m_StorageDoorMeshComponent[0]->SetRelativeScale3D(RAFT_SCALE);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StorageDoorLMeshAsset(TEXT("StaticMesh'/Game/Item/Building/Storage/Storage_Bigger_Door_L.Storage_Bigger_Door_L'"));

	if(StorageDoorLMeshAsset.Succeeded())
	{
		m_StorageDoorMeshComponent[0]->SetStaticMesh(StorageDoorLMeshAsset.Object);
	}

	// Joint Right, Door Right
	m_JointComponent.Add(CreateDefaultSubobject<USceneComponent>(TEXT("JointRight")));
	m_StorageDoorMeshComponent.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StorageDoorR")));
	m_JointComponent[1]->SetupAttachment(m_RootComponent);
	m_StorageDoorMeshComponent[1]->SetupAttachment(m_JointComponent[1]);
	m_StorageDoorMeshComponent[1]->SetRelativeScale3D(RAFT_SCALE);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StorageDoorRMeshAsset(TEXT("StaticMesh'/Game/Item/Building/Storage/Storage_Bigger_Door_R.Storage_Bigger_Door_R'"));

	if(StorageDoorRMeshAsset.Succeeded())
	{
		m_StorageDoorMeshComponent[1]->SetStaticMesh(StorageDoorRMeshAsset.Object);
	}

	
	m_AngleMax.Add(30.f);
	m_AngleMax.Add(-210.f);

	//m_AngleMax.Add(120.f);
	//m_AngleMax.Add(-120.f);
}

void AStorageBigger::BuildCompleted()
{
	Super::BuildCompleted();
	
	SetBurnEffectEnable(true);
}

void AStorageBigger::BreakCompleted()
{
	Super::BreakCompleted();
	
	SetBurnEffectEnable(true, -1.f);
}

void AStorageBigger::OnInteractionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(m_BuildingState != EBuildingState::Build) { return; }
	
	Super::OnInteractionEnter(Player, HitComponent);

	m_Open = true;
	m_StorageAction = true;

	if(m_AngleMax.Num() > 0 && m_JointComponent.Num() > 0)
	{
		FRotator JointRotator = m_JointComponent[0]->GetRelativeRotation();
		m_StorageAccTime = ((JointRotator.Yaw + 90.f) / (m_AngleMax[0] + 90.f)) * m_StorageMaxTime;
	}
}

void AStorageBigger::OnInteractionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(m_BuildingState != EBuildingState::Build) { return; }	
	
	Super::OnInteractionExit(Player, HitComponent);

	m_Open = false;
	m_StorageAction = true;

	if(m_AngleMax.Num() > 0 && m_JointComponent.Num() > 0)
	{
		FRotator JointRotator = m_JointComponent[0]->GetRelativeRotation();
		m_StorageAccTime = (1.f - ((JointRotator.Yaw + 90.f) / (m_AngleMax[0] + 90.f))) * m_StorageMaxTime;
	}
}

void AStorageBigger::StorageAction(float DeltaTime)
{
	//Super::StorageAction(DeltaTime);

	if(false == m_StorageAction) { return; }

	m_StorageAccTime += DeltaTime;

	for(int32 i = 0; i < m_JointComponent.Num(); ++i)
	{
		FRotator JointRotator = m_JointComponent[i]->GetRelativeRotation();
		if(m_Open)
		{
			JointRotator.Yaw = UKismetMathLibrary::Lerp(-90.f, m_AngleMax[i], m_StorageAccTime / m_StorageMaxTime);
		}
		else
		{
			JointRotator.Yaw = UKismetMathLibrary::Lerp(m_AngleMax[i], -90.f, m_StorageAccTime / m_StorageMaxTime);
		}

		m_JointComponent[i]->SetRelativeRotation(JointRotator);
	}

	if(m_StorageAccTime >= m_StorageMaxTime)
	{
		m_StorageAccTime = 0.f;
		m_StorageAction = false;
	}
}
