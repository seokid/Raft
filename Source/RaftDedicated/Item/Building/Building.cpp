// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "../../RaftDedicatedCharacter.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Boat.h"

ABuilding::ABuilding()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Foundation/Block_Foundation.Block_Foundation'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
	}

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_MeshComponent->ResetRelativeTransform();
	m_MeshComponent->SetRelativeScale3D(RAFT_SCALE);
	
	m_MeshComponent->SetCollisionProfileName(TEXT("Building"));
	SetActorEnableCollision(true);

	//SetRootComponent(m_MeshComponent);
	SetRootComponent(m_RootComponent);
	m_MeshComponent->SetupAttachment(m_RootComponent);
	
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BlockFoundationGreenMaterialAsset(TEXT("Material'/Game/Item/Building/Common/Building_Green.Building_Green'"));
	if(BlockFoundationGreenMaterialAsset.Succeeded())
	{
		m_GreenMaterial = BlockFoundationGreenMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BlockFoundationAlphaMaterialAsset(TEXT("Material'/Game/Item/Building/Common/Building_Alpha.Building_Alpha'"));
	if(BlockFoundationAlphaMaterialAsset.Succeeded())
	{
		m_AlphaMaterial = BlockFoundationAlphaMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BlockFoundationRedMaterialAsset(TEXT("Material'/Game/Item/Building/Common/Building_Red.Building_Red'"));
	if(BlockFoundationRedMaterialAsset.Succeeded())
	{
		m_RedMaterial = BlockFoundationRedMaterialAsset.Object;
	}

	m_BuildingInfo.m_Name = TEXT("Building");
	m_BuildingInfo.m_StateType = EBuildingStateType::Build;
	m_MeshComponent->SetCollisionProfileName(TEXT("Building"));
	m_BuildingInfo.m_IdxX = 0;
	m_BuildingInfo.m_IdxY = 0;

	m_BuildingState = EBuildingState::None;

	m_AccTime = 0.f;
	m_MaxTime = 1.f;
	m_EffectBoundary = 0.5f;
	m_EffectEnable = false;
	m_EffectSpeed = 1.f;
	
	//SetActorRelativeScale3D(RAFT_SCALE);
	//SetActorScale3D(RAFT_SCALE);
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_EffectEnable)
	{
		ExecuteBurnEffect(DeltaTime);
	}
}

void ABuilding::SetBuildingInfo(const FBuildingInfo& Info)
{
	m_BuildingInfo = Info;
	SetBuildingStateType(m_BuildingInfo.m_StateType);
}

void ABuilding::SetBuildingStateType(const EBuildingStateType& StateType)
{
	switch (StateType)
	{
	case EBuildingStateType::Build:
		{
			m_MeshComponent->SetMaterial(0, m_SourceMaterial);
			m_BuildingInfo.m_StateType = EBuildingStateType::Build;
			m_MeshComponent->SetCollisionProfileName(TEXT("Building"));
			SetActorEnableCollision(true);
			break;
		}
	case EBuildingStateType::Green:
		{
			m_MeshComponent->SetMaterial(0, m_GreenMaterial);
			m_BuildingInfo.m_StateType = EBuildingStateType::Green;
			m_MeshComponent->SetCollisionProfileName(TEXT("Building_Green"));
			SetActorEnableCollision(true);
			break;
		}
	case EBuildingStateType::Red:
		{
			m_MeshComponent->SetMaterial(0, m_RedMaterial);
			m_BuildingInfo.m_StateType = EBuildingStateType::Red;
			m_MeshComponent->SetCollisionProfileName(TEXT("Building_Green"));
			SetActorEnableCollision(true);
			break;
		}
	case EBuildingStateType::Alpha:
		{
			m_MeshComponent->SetMaterial(0, m_AlphaMaterial);
			m_BuildingInfo.m_StateType = EBuildingStateType::Alpha;
			m_MeshComponent->SetCollisionProfileName(TEXT("Building_Green"));
			SetActorEnableCollision(true);
			break;
		}
	}
}

void ABuilding::SetBuildingMesh(UStaticMesh* Mesh)
{
	m_Mesh = Mesh;
	m_MeshComponent->SetStaticMesh(Mesh);
}

ABuilding* ABuilding::RaycastToFloor()
{
	FHitResult	result;
	FCollisionQueryParams params(NAME_None, false, this);
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(),
				GetActorLocation() + FVector(0.f, 0.f, -50.f), ECollisionChannel::ECC_GameTraceChannel2, params);

	if(Hit)
	{
		ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
		if(SelectBuilding)
		{
			if((SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Bottom || SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Floor) && SelectBuilding->GetBuildingInfo().m_StateType == EBuildingStateType::Build)
			{
				return SelectBuilding;
			}
		}
	}

	return nullptr;
}

float ABuilding::GetBuildingInstallYawToFloor(float LookAtYaw)
{
	float CurYaw = LookAtYaw + 180.f;
	
	if(45.f <= CurYaw && CurYaw <= 135.f)
	{
		CurYaw = -90.f;
	}
	else if(135.f <= CurYaw && CurYaw <= 225.f)
	{
		CurYaw = 0.f;
	}
	else if(225.f <= CurYaw && CurYaw <= 315.f)
	{
		CurYaw = 90.f;
	}
	else
	{
		CurYaw = 180.f;
	}

	return CurYaw;
}

void ABuilding::GetNewIdxToLayer(int32& OutputIdxX, int32& OutputIdxY, int32 AddLayer)
{
	float CurYaw = GetRootComponent()->GetRelativeRotation().Yaw + 180.f;

	OutputIdxX = m_BuildingInfo.m_IdxX;
	OutputIdxY = m_BuildingInfo.m_IdxY;
	if(80.f <= CurYaw && CurYaw <= 100.f)
	{
		OutputIdxY += AddLayer;
	}
	else if(170.f <= CurYaw && CurYaw <= 190.f)
	{
		OutputIdxX -= AddLayer;
	}
	else if(260.f <= CurYaw && CurYaw <= 280.f)
	{
		OutputIdxY -= AddLayer;
	}
	else
	{
		OutputIdxX += AddLayer;
	}
}

bool ABuilding::IsFloorExist()
{
	ABoat* BoatActor =  Cast<ABoat>(GetAttachParentActor());
	if(nullptr == BoatActor) { return false;}
	ABuilding* FloorActor = BoatActor->GetBuilding(m_BuildingInfo.m_Layer, m_BuildingInfo.m_IdxX, m_BuildingInfo.m_IdxY);

	if(FloorActor && FloorActor->GetBuildingInfo().m_StateType == EBuildingStateType::Build)
	{
		return true;
	}

	
	
	return false;
}

void ABuilding::OnInteraction(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(EBuildingState::Break == m_BuildingState) { return; }
	Super::OnInteraction(Player, HitComponent);
}

bool ABuilding::IsBuildPossible()
{
	// 충돌 검사 해야함
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
				if(BuildingActor->GetBuildingInfo().m_BuildType == EBuildType::Install) {return false;}
				//if(BuildingActor->GetBuildingInfo().m_Layer != m_BuildingInfo.m_Layer) {return false;}
			}
		}
	}
	return true;
}

bool ABuilding::IsBuildPossible(APlayerCharacter* BuildOwnerActor)
{
	if(m_BuildingInfo.m_BuildType == EBuildType::Install)
	{
		SetActorLocation(BuildOwnerActor->GetRaycastLocation());
		FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BuildOwnerActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
		LookAtRoataor.Pitch = 0.f;
		LookAtRoataor.Roll = 0.f;
		SetActorRotation(LookAtRoataor);
	
		ABuilding* FloorBuilding = RaycastToFloor();
				
		if(FloorBuilding)
		{
			LookAtRoataor.Yaw = GetBuildingInstallYawToFloor(LookAtRoataor.Yaw);
			LookAtRoataor.Yaw += FloorBuilding->GetActorRotation().Yaw;
			LookAtRoataor.Yaw += 90.f * BuildOwnerActor->GetBuildingRotationIdx();
			SetActorLocation(FloorBuilding->GetActorLocation());
			SetActorRotation(LookAtRoataor);
			m_BuildingInfo.m_IdxX = FloorBuilding->GetBuildingInfo().m_IdxX;
			m_BuildingInfo.m_IdxY = FloorBuilding->GetBuildingInfo().m_IdxY;
			m_BuildingInfo.m_Layer = FloorBuilding->GetBuildingInfo().m_Layer; 
				
			if(IsBuildPossible())
			{
				SetBuildingStateType(EBuildingStateType::Green);
				return true;
			}
			else
			{
				SetBuildingStateType(EBuildingStateType::Red);
				return false;
			}
		}
		else
		{
			LookAtRoataor.Yaw += 90.f * BuildOwnerActor->GetBuildingRotationIdx();
			SetBuildingStateType(EBuildingStateType::Red);
			SetActorRotation(LookAtRoataor);
			return false;
		}
	}
	else
	{

		FRotator BuildingRotator = GetActorRotation();
		BuildingRotator.Yaw = 90.f * BuildOwnerActor->GetBuildingRotationIdx();
		SetActorRelativeRotation(BuildingRotator);
	
		SetBuildingMesh(BuildOwnerActor->GetNewBuildingClassInfo().SourceMesh);

		if(m_BuildingInfo.m_BuildType != BuildOwnerActor->GetNewBuildingClassInfo().BuildType)
		{
			SetBuildingStateType(EBuildingStateType::Red);
			return false;
		}
		else
		{
			if(IsBuildPossible())
			{
				SetBuildingStateType(EBuildingStateType::Green);
				return true;
			}
			else
			{
				SetBuildingStateType(EBuildingStateType::Red);
				return false;
			}
		}
	}
}

void ABuilding::SetBurnEffectEnable(bool Enable, float EffectSpeed)
{
	if(Enable)
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);
	}
	else
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
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
}

void ABuilding::ExecuteBurnEffect(float DeltaTime)
{
	if(false == m_EffectEnable) { return; }
	
	m_AccTime += (DeltaTime * m_EffectSpeed);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);

	if((m_EffectSpeed > 0.f && m_AccTime >= m_MaxTime) || (m_EffectSpeed < 0.f && m_AccTime <= 0.f))
	{
		EffectFinished();
	}
}

void ABuilding::EffectFinished()
{
	LOG(TEXT("%s"), *GetName());

	m_AccTime = 0.f;
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	m_EffectEnable = false;
	
	if(EBuildingState::Break == m_BuildingState)
	{
		Destroy();
	}
}

