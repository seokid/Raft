// Fill out your copyright notice in the Description page of Project Settings.


#include "Storage.h"
#include "../../RaftDedicatedCharacter.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "RaftDedicated/UI/MainHUD.h"

AStorage::AStorage()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Storage/Storage_Bottom.Storage_Bottom'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = TEXT("Storage");
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;

	m_JointComponent.Add(CreateDefaultSubobject<USceneComponent>(TEXT("Joint")));
	m_StorageDoorMeshComponent.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StorageDoor")));
	m_JointComponent[0]->SetupAttachment(m_RootComponent);
	m_StorageDoorMeshComponent[0]->SetupAttachment(m_JointComponent[0]);
	m_StorageDoorMeshComponent[0]->SetRelativeScale3D(RAFT_SCALE);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StorageTopMeshAsset(TEXT("StaticMesh'/Game/Item/Building/Storage/Storage_Top.Storage_Top'"));

	if(StorageTopMeshAsset.Succeeded())
	{
		m_StorageDoorMeshComponent[0]->SetStaticMesh(StorageTopMeshAsset.Object);
	}

	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox")));
	m_ArrayInteractionComponent[0]->SetupAttachment(m_RootComponent);
	m_ArrayInteractionComponent[0]->SetCollisionProfileName(TEXT("Interaction"));

	// Storage Action Audio
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StorageActionAudio"));
	m_AudioComponent->SetupAttachment(m_RootComponent);
	
	m_StorageAccTime = 0.f;
	m_StorageMaxTime = 1.f;
	m_AngleMax.Add(-110.f);
	
	m_Open = false;
	m_Interaction = false;
	m_StorageAction = false;
}

void AStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StorageAction(DeltaTime);
}

void AStorage::SetBuildingStateType(const EBuildingStateType& StateType)
{
	Super::SetBuildingStateType(StateType);

	for(auto DoorMeshCompoent : m_StorageDoorMeshComponent)
	{
		DoorMeshCompoent->SetMaterial(0, m_MeshComponent->GetMaterial(0));
	}
	//m_StorageDoorMeshComponent[0]->SetMaterial(0, m_MeshComponent->GetMaterial(0));
}

bool AStorage::IsBuildPossible(APlayerCharacter* BuildOwnerActor)
{
	//Super::IsBuildPossible(BuildOwnerActor);

	SetActorLocation(BuildOwnerActor->GetRaycastLocation());
	FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BuildOwnerActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
	LookAtRoataor.Pitch = 0.f;
	LookAtRoataor.Roll = 0.f;
	SetActorRotation(LookAtRoataor);

	ABuilding* FloorBuilding = RaycastToFloor();
				
	if(FloorBuilding)
	{
		FVector Loc = GetActorLocation();
		Loc.Z = FloorBuilding->GetActorLocation().Z;
		SetActorLocation(Loc);
		m_BuildingInfo.m_IdxX = FloorBuilding->GetBuildingInfo().m_IdxX;
		m_BuildingInfo.m_IdxY = FloorBuilding->GetBuildingInfo().m_IdxY;
		m_BuildingInfo.m_Layer = FloorBuilding->GetBuildingInfo().m_Layer;
		
		if(IsBuildPossible())
		{
			SetBuildingStateType(EBuildingStateType::Green);
			return true;
		}
	}
	SetBuildingStateType(EBuildingStateType::Red);
	return false;
}

bool AStorage::IsBuildPossible()
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
				if(BuildingActor->GetName() == GetName()) {return false;}
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

void AStorage::BuildCompleted()
{
	Super::BuildCompleted();

	SetBurnEffectEnable(true);
}

void AStorage::BreakCompleted()
{
	Super::BreakCompleted();

	SetBurnEffectEnable(true, -1.f);
}

void AStorage::OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(m_BuildingState != EBuildingState::Build) { return; }
	Super::OnInteractionEnter(Player, HitComponent);
	
	Player->GetMainHUD()->StorageInteractionEnter(this);
	
	m_Open = true;
	m_StorageAction = true;

	m_AudioComponent->SetSound(m_OpenSound);
	m_AudioComponent->Play();

	if(m_AngleMax.Num() > 0 && m_JointComponent.Num() > 0)
	{
		FRotator JointRotator = m_JointComponent[0]->GetRelativeRotation();
		m_StorageAccTime = (JointRotator.Roll / m_AngleMax[0]) * m_StorageMaxTime;
	}
}

void AStorage::OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(m_BuildingState != EBuildingState::Build) { return; }
	
	Super::OnInteractionExit(Player, HitComponent);

	Player->GetMainHUD()->StorageInteractionExit(this);
	
	m_Open = false;
	m_StorageAction = true;

	m_AudioComponent->SetSound(m_CloseSound);
	m_AudioComponent->Play();

	if(m_AngleMax.Num() > 0 && m_JointComponent.Num() > 0)
	{
		FRotator JointRotator = m_JointComponent[0]->GetRelativeRotation();
		m_StorageAccTime = (1.f - (JointRotator.Roll / m_AngleMax[0])) * m_StorageMaxTime;
	}
}

void AStorage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);
	SetItemInteractionUIText(Player, HitComponent);

	if(EBuildingState::Build == m_BuildingState)
	{
		m_MeshComponent->SetRenderCustomDepth(true);
		m_MeshComponent->SetCustomDepthStencilValue(255);

		for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
		{
			m_StorageDoorMeshComponent[i]->SetRenderCustomDepth(true);
			m_StorageDoorMeshComponent[i]->SetCustomDepthStencilValue(255);
		}
	}
}

void AStorage::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
	SetItemInteractionUIText(Player, HitComponent);
}

void AStorage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	Player->SetItemInteractionUIText(false);

	if(EBuildingState::Build == m_BuildingState)
	{
		m_MeshComponent->SetRenderCustomDepth(false);
		for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
		{
			m_StorageDoorMeshComponent[i]->SetRenderCustomDepth(false);
		}
	}
}

void AStorage::SetBurnEffectEnable(bool Enable, float EffectSpeed)
{
	//Super::SetBurnEffectEnable(Enable, EffectSpeed);

	if(Enable)
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);
		for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
		{
			m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);
		}
	}
	else
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
		for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
		{
			m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
		}
	}
	
	m_EffectEnable = Enable;
	m_EffectSpeed = EffectSpeed;
	if(EffectSpeed > 0.f)
	{
		m_AccTime = 0.f;
	}
	else
	{
		m_AccTime = m_MaxTime;
	}
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("MaxTime"), m_MaxTime);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Ramp Boundary"), m_EffectBoundary);
	
	for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
	{
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("MaxTime"), m_MaxTime);
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("Ramp Boundary"), m_EffectBoundary);
	}
}

void AStorage::ExecuteBurnEffect(float DeltaTime)
{
	//Super::ExecuteBurnEffect(DeltaTime);

	if(false == m_EffectEnable) { return; }
	
	m_AccTime += (DeltaTime * m_EffectSpeed);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);

	for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
	{
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	}

	if((m_EffectSpeed > 0.f && m_AccTime >= m_MaxTime) || (m_EffectSpeed < 0.f && m_AccTime <= 0.f))
	{
		EffectFinished();
	}
}

void AStorage::EffectFinished()
{
	//Super::EffectFinished();

	m_AccTime = 0.f;
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);

	for(int32 i = 0; i < m_StorageDoorMeshComponent.Num(); ++i)
	{
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
		m_StorageDoorMeshComponent[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	}
	m_EffectEnable = false;
	
	if(EBuildingState::Break == m_BuildingState)
	{
		Destroy();
	}
}

void AStorage::StorageAction(float DeltaTime)
{
	if(false == m_StorageAction) { return; }

	m_StorageAccTime += DeltaTime;

	for(int32 i = 0; i < m_JointComponent.Num(); ++i)
	{
		FRotator JointRotator = m_JointComponent[i]->GetRelativeRotation();
		if(m_Open)
		{
			JointRotator.Roll = UKismetMathLibrary::Lerp(0.f, m_AngleMax[i], m_StorageAccTime / m_StorageMaxTime);
		}
		else
		{
			JointRotator.Roll = UKismetMathLibrary::Lerp(m_AngleMax[i], 0.f, m_StorageAccTime / m_StorageMaxTime);
		}

		m_JointComponent[i]->SetRelativeRotation(JointRotator);
	}

	if(m_StorageAccTime >= m_StorageMaxTime)
	{
		m_StorageAccTime = 0.f;
		m_StorageAction = false;
	}
}

void AStorage::SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(EBuildingState::Build != m_BuildingState) { return; }
	
	if(m_Open)
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Storage Close"));
	}
	else
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Storage Open"));
	}
}
