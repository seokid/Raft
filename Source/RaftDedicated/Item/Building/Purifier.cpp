// Fill out your copyright notice in the Description page of Project Settings.


#include "Purifier.h"
#include "../../RaftDedicatedCharacter.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "RaftDedicated/UI/QuickItemSlotList.h"
#include "Kismet/KismetMathLibrary.h"
#include "RaftDedicated/Item/Equipment/WaterCup.h"

APurifier::APurifier()
{
	// 본처ㅔ 메쉬
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Purifier.Purifier'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = TEXT("Purifier");
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;
	
	// 장작 메쉬
	m_ArrayFirewood.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firewood0")));
	m_ArrayFirewood.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firewood1")));
	m_ArrayFirewood.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firewood2")));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Firewood0Asset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Purifier_Firewood_0.Purifier_Firewood_0'"));
	if(Firewood0Asset.Succeeded())
	{
		m_ArrayFirewood[0]->SetStaticMesh(Firewood0Asset.Object);
		m_ArrayFirewood[0]->SetupAttachment(m_RootComponent);
		m_ArrayFirewood[0]->SetRelativeScale3D(RAFT_SCALE);
		m_ArrayFirewood[0]->SetMaterial(0, m_AlphaMaterial);
		m_ArrayFirewood[0]->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Firewood1Asset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Purifier_Firewood_1.Purifier_Firewood_1'"));
	if(Firewood1Asset.Succeeded())
	{
		m_ArrayFirewood[1]->SetStaticMesh(Firewood1Asset.Object);
		m_ArrayFirewood[1]->SetupAttachment(m_RootComponent);
		m_ArrayFirewood[1]->SetRelativeScale3D(RAFT_SCALE);
		m_ArrayFirewood[1]->SetMaterial(0, m_AlphaMaterial);
		m_ArrayFirewood[1]->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Firewood2Asset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Purifier_Firewood_2.Purifier_Firewood_2'"));
	if(Firewood2Asset.Succeeded())
	{
		m_ArrayFirewood[2]->SetStaticMesh(Firewood2Asset.Object);
		m_ArrayFirewood[2]->SetupAttachment(m_RootComponent);
		m_ArrayFirewood[2]->SetRelativeScale3D(RAFT_SCALE);
		m_ArrayFirewood[2]->SetMaterial(0, m_AlphaMaterial);
		m_ArrayFirewood[2]->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 상호작용 박스
	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionFirewood")));
	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionSaltWater")));
	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionFreshWater")));
	for(auto InteractionComponent : m_ArrayInteractionComponent)
	{
		InteractionComponent->SetupAttachment(m_RootComponent);
		InteractionComponent->SetCollisionProfileName(TEXT("Interaction"));
	}

	// 소금물 메쉬
	m_SaltWaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SaltWater"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SlatWaterAsset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Salt_Water.Salt_Water'"));
	if(SlatWaterAsset.Succeeded())
	{
		m_SaltWaterMeshComponent->SetStaticMesh(SlatWaterAsset.Object);
		m_SaltWaterMeshComponent->SetupAttachment(m_RootComponent);
		m_SaltWaterMeshComponent->SetRelativeScale3D(RAFT_SCALE);
		m_SaltWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
		m_SaltWaterMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 담수 메쉬
	m_FreshWaterMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FreshWater"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FreshWaterAsset(TEXT("StaticMesh'/Game/Item/Building/Purifier/Fresh_Water.Fresh_Water'"));
	if(FreshWaterAsset.Succeeded())
	{
		m_FreshWaterMeshComponent->SetStaticMesh(FreshWaterAsset.Object);
		m_FreshWaterMeshComponent->SetupAttachment(m_RootComponent);
		m_FreshWaterMeshComponent->SetRelativeScale3D(RAFT_SCALE);
		m_FreshWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
		m_FreshWaterMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// Fire Particle System
	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleSystem"));
	m_ParticleSystem->SetupAttachment(m_RootComponent);

	// Fire Audio
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudio"));
	m_AudioComponent->SetupAttachment(m_RootComponent);
	
	m_FreshWater = false;
	m_SaltWater = false;
	m_WaterAccTime = 0.f;
	m_WaterMaxTime = 30.f;
	m_FirewoodCount = 0;
	m_FirewoodMaxCount = m_ArrayFirewood.Num();
	m_FirewoodAccTime = 0.f;
	m_FirewoodMaxTime = 15.f;
}

void APurifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SaltWaterToFreshWaterAction(DeltaTime);
}


void APurifier::SetBuildingStateType(const EBuildingStateType& StateType)
{
	Super::SetBuildingStateType(StateType);
}

bool APurifier::IsBuildPossible(APlayerCharacter* BuildOwnerActor)
{
	//return Super::IsBuildPossible(BuildOwnerActor);

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

bool APurifier::IsBuildPossible()
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

void APurifier::BuildCompleted()
{
	Super::BuildCompleted();

	SetBurnEffectEnable(true);
}

void APurifier::BreakCompleted()
{
	Super::BreakCompleted();

	SetBurnEffectEnable(true, -1.f);

	
}

void APurifier::OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	//Super::OnInteractionEnter(Player, HitComponent);

	if(m_BuildingState != EBuildingState::Build) { return; }

	if(IsFirewoodComponent(HitComponent))
	{
		// 최대 장작 보다 현재 장작이 적다면 장작 추가
		if(m_FirewoodCount < m_FirewoodMaxCount)
		{
			int ItemCount = 1;
			if(Player->GetMainHUD()->RemoveItem(TEXT("Plank"), ItemCount))
			{
				// 장작 추가
				m_ArrayFirewood[m_FirewoodCount]->SetMaterial(0, m_SourceMaterial);
				++m_FirewoodCount;
				SetOutLineEffectEnable(true);
			}
		}
	}
	else if(IsSaltWaterComponent(HitComponent))
	{
		// 담수가 없고 정수 중이지 않으면(정수 중이면 소금물이 이미 있다) -> 소금물이 없다
		// 담수가 없고 소금물도 없을 경우 -> 담수가 있다면 못줌
		if(false == m_FreshWater && false == m_SaltWater)
		{
			// Player 에게 음식 받아오기
			AWaterCup* WaterCup = Cast<AWaterCup>(Player->GetEquipment());
			if(nullptr == WaterCup) { return; }
			if(false == WaterCup->IsSaltWater()) {return;}
			WaterCup->ChangeWater(EWaterType::Empty);
			// 소금물 생성
			m_SaltWater = true;
			m_SaltWaterMeshComponent->SetMaterial(0, m_SourceMaterial);
		}
	}
	else if(IsFreshWaterComponent(HitComponent))
	{
		// 플레이어가 컨테이너(물담는 용기)가 있는지 검사 후 물 줘야 함

		// 담수가 있다면
		if(m_FreshWater)
		{
			AWaterCup* WaterCup = Cast<AWaterCup>(Player->GetEquipment());
			if(nullptr == WaterCup) { return; }
			if(false == WaterCup->IsEmptyWater()) {return;}
			WaterCup->ChangeWater(EWaterType::FreshWater);
			// 담수 플레이어에게 주고 삭제
			m_FreshWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
			m_FreshWater = false;
		}
	}


	
}

void APurifier::OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionExit(Player, HitComponent);
}

void APurifier::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);
	if(EBuildingState::Build == m_BuildingState)
	{
		SetOutLineEffectEnable(true);
		SetItemInteractionUIText(Player, HitComponent);
	}
}

void APurifier::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);

	SetItemInteractionUIText(Player, HitComponent);
}

void APurifier::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	SetOutLineEffectEnable(false);
	SetItemInteractionUIText(Player, nullptr);
}

void APurifier::SetBurnEffectEnable(bool Enable, float EffectSpeed)
{
	//Super::SetBurnEffectEnable(Enable, EffectSpeed);

	m_SaltWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
	m_FreshWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
	m_ParticleSystem->Deactivate();
	m_AudioComponent->Stop();

	if(Enable)
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);
		for(int32 i = 0; i < m_FirewoodCount; ++i)
		{
			m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);;
		}
	}
	else
	{
		m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
		for(int32 i = 0; i < m_FirewoodCount; ++i)
		{
			m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);;
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

	for(int32 i = 0; i < m_FirewoodCount; ++i)
	{
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("MaxTime"), m_MaxTime);
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("Ramp Boundary"), m_EffectBoundary);
	}
}

void APurifier::ExecuteBurnEffect(float DeltaTime)
{
	//Super::ExecuteBurnEffect(DeltaTime);
	if(false == m_EffectEnable) { return; }
	
	m_AccTime += (DeltaTime * m_EffectSpeed);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);

	for(int32 i = 0; i < m_FirewoodCount; ++i)
	{
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	}

	if((m_EffectSpeed > 0.f && m_AccTime >= m_MaxTime) || (m_EffectSpeed < 0.f && m_AccTime <= 0.f))
	{
		EffectFinished();
	}
}

void APurifier::EffectFinished()
{
	//Super::EffectFinished();
	m_AccTime = 0.f;
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
	m_MeshComponent->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	m_EffectEnable = false;

	for(int32 i = 0; i < m_FirewoodCount; ++i)
	{
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
		m_ArrayFirewood[i]->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_AccTime);
	}
	
	if(EBuildingState::Break == m_BuildingState)
	{
		Destroy();
	}
}

void APurifier::SetOutLineEffectEnable(bool Enable)
{
	m_MeshComponent->SetRenderCustomDepth(Enable);
	m_MeshComponent->SetCustomDepthStencilValue(255);
	
	for(int32 i = 0; i < m_FirewoodCount; ++i)
	{
		m_ArrayFirewood[i]->SetRenderCustomDepth(Enable);
		m_ArrayFirewood[i]->SetCustomDepthStencilValue(255);
	}
}

void APurifier::SaltWaterToFreshWaterAction(float DeltaTime)
{
	if(m_FreshWater || false == m_SaltWater || m_FirewoodCount <= 0)
	{
		if(m_ParticleSystem->IsActive())
		{
			m_ParticleSystem->Deactivate();
			m_AudioComponent->Stop();
		}

		return;
	}

	if(false == m_ParticleSystem->IsActive())
	{
		m_ParticleSystem->Activate(true);
		m_AudioComponent->Play();
	}

	m_WaterAccTime += DeltaTime;
	m_FirewoodAccTime += DeltaTime;

	if(m_FirewoodAccTime >= m_FirewoodMaxTime)
	{
		m_FirewoodAccTime = 0.f;
		m_ArrayFirewood[m_FirewoodCount -1]->SetMaterial(0, m_AlphaMaterial);
		--m_FirewoodCount;
	}
	
	if(m_WaterAccTime >= m_WaterMaxTime)
	{
		m_ParticleSystem->Deactivate();
		m_AudioComponent->Stop();
		m_WaterAccTime = 0.f;
		m_FreshWater = true;
		m_SaltWater = false;
		m_SaltWaterMeshComponent->SetMaterial(0, m_AlphaMaterial);
		m_FreshWaterMeshComponent->SetMaterial(0, m_SourceMaterial);
	}
}

void APurifier::SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(EBuildingState::Build != m_BuildingState) { return; }

	if(IsFirewoodComponent(HitComponent) && m_FirewoodCount < m_FirewoodMaxCount)
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Add Firewood"));
	}
	else if(IsSaltWaterComponent(HitComponent) && false == m_FreshWater && false == m_SaltWater)
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Add SaltWater"));
	}
	else if(IsFreshWaterComponent(HitComponent) && m_FreshWater)
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("take FreshWater"));
	}
	else
	{
		Player->SetItemInteractionUIText(false);
	}
}

