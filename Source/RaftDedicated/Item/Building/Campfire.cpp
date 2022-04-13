// Fill out your copyright notice in the Description page of Project Settings.


#include "Campfire.h"
#include "../../RaftDedicatedCharacter.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "../Equipment/Food.h"
#include "Kismet/KismetMathLibrary.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "RaftDedicated/UI/QuickItemSlotList.h"

ACampfire::ACampfire()
{
	// 본체 메쉬
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Campfire/Campfire_model.Campfire_model'"));

	if(BlockAsset.Succeeded())
	{
		m_Mesh = BlockAsset.Object;
		m_SourceMaterial = m_Mesh->GetMaterial(0);
		m_BuildingInfo.m_Name = TEXT("Compfire");
	}
	m_MeshComponent->SetStaticMesh(m_Mesh);
	
	m_BuildingInfo.m_BuildType = EBuildType::Install;

	// 장작 메쉬
	m_ArrayFirewood.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firewood0")));
	m_ArrayFirewood.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firewood1")));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Firewood0Asset(TEXT("StaticMesh'/Game/Item/Building/Campfire/Campfire_Firewood_0.Campfire_Firewood_0'"));
	if(Firewood0Asset.Succeeded())
	{
		m_ArrayFirewood[0]->SetStaticMesh(Firewood0Asset.Object);
		m_ArrayFirewood[0]->SetupAttachment(m_RootComponent);
		m_ArrayFirewood[0]->SetRelativeScale3D(RAFT_SCALE);
		m_FirewoodMaterial = m_ArrayFirewood[0]->GetMaterial(0); 
		m_ArrayFirewood[0]->SetMaterial(0, m_AlphaMaterial);
		m_ArrayFirewood[0]->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Firewood1Asset(TEXT("StaticMesh'/Game/Item/Building/Campfire/Campfire_Firewood_1.Campfire_Firewood_1'"));
	if(Firewood1Asset.Succeeded())
	{
		m_ArrayFirewood[1]->SetStaticMesh(Firewood1Asset.Object);
		m_ArrayFirewood[1]->SetupAttachment(m_RootComponent);
		m_ArrayFirewood[1]->SetRelativeScale3D(RAFT_SCALE);
		m_ArrayFirewood[1]->SetMaterial(0, m_AlphaMaterial);
		m_ArrayFirewood[1]->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 상호작용 박스
	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionFirewood")));
	m_ArrayInteractionComponent.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionFood")));
	for(auto InteractionComponent : m_ArrayInteractionComponent)
	{
		InteractionComponent->SetupAttachment(m_RootComponent);
		InteractionComponent->SetCollisionProfileName(TEXT("Interaction"));
	}

	// Food Location Scene Component
	m_FoodLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Food"));
	m_FoodLocationComponent->SetupAttachment(m_RootComponent);

	// Fire Particle System
	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleSystem"));
	m_ParticleSystem->SetupAttachment(m_RootComponent);

	// Fire Audio
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudio"));
	m_AudioComponent->SetupAttachment(m_RootComponent);
	
	m_FoodEnable = false;
	m_FoodFinish = false;
	m_FoodAccTime = 0.f;
	m_FoodMaxTime = 10.f;
	m_FirewoodAccTime = 0.f;
	m_FirewoodMaxTime = 15.f;
	m_FirewoodCount = 0;
	m_FirewoodMaxCount = m_ArrayFirewood.Num();
	
	m_FoodActor = nullptr;
}

void ACampfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CookingAction(DeltaTime);
}

void ACampfire::SetBuildingStateType(const EBuildingStateType& StateType)
{
	Super::SetBuildingStateType(StateType);
}

bool ACampfire::IsBuildPossible(APlayerCharacter* BuildOwnerActor)
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

bool ACampfire::IsBuildPossible()
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

void ACampfire::BuildCompleted()
{
	Super::BuildCompleted();

	SetBurnEffectEnable(true);
}

void ACampfire::BreakCompleted()
{
	Super::BreakCompleted();

	SetBurnEffectEnable(true, -1.f);
}

void ACampfire::OnInteractionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	//Super::OnInteractionEnter(Player, HitComponent);

	if(m_BuildingState != EBuildingState::Build) { return; }

	// InteractionFirewood
	if(IsFirewoodComponent(HitComponent))
	{
		// 최대 장작 보다 현재 장작이 적다면 장작 추가
		if(m_FirewoodCount < m_FirewoodMaxCount)
		{
			int ItemCount = 1;
			if(Player->GetMainHUD()->RemoveItem(TEXT("Plank"), ItemCount))
			{
				// 장작 추가
				m_ArrayFirewood[m_FirewoodCount]->SetMaterial(0, m_FirewoodMaterial);
				++m_FirewoodCount;
				SetOutLineEffectEnable(true);
			}
		}
	}
	// InteractionFood
	else if(IsFoodComponent(HitComponent))
	{
		// 음식이 있고, 조리 완료 라면
		if(m_FoodEnable && m_FoodFinish)
		{
			int FoodItemCound = 1;
			if(Player->GetMainHUD()->AddItem(m_FoodName, FoodItemCound))
			{
				m_FoodEnable = false;
				m_FoodFinish = false;
				m_FoodName = "";
				SetFoodActor(m_FoodName);
			}
			
			// Player 에게 음식 보내주기
			LOG(TEXT("Take Food"));
		}
		// 음식이 있고, 조리 완료 x
		else if(m_FoodEnable && false == m_FoodFinish)
		{
			// 아무 일 없음
		}
		// 음식이 없음 -> 조리 당연 x
		else if(false == m_FoodEnable)
		{
			// Player 에게 음식 받아오기
			AFood* Food = Cast<AFood>(Player->GetEquipment());
			if(nullptr == Food) { return; }
			if(Food->IsCookedFood()) {return;}
			
			int RemoveItemCount = 1;
			if(Player->GetMainHUD()->GetQuickItemSlotListWidget()->RemoveSelectItem(Food->GetFoodName(), RemoveItemCount))
			{
				m_FoodName = Food->GetFoodName();
				SetFoodActor(m_FoodName);
				m_FoodEnable = true;
			}
			
			LOG(TEXT("Add Food"));
		}
	}
}

void ACampfire::OnInteractionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionExit(Player, HitComponent);
}

void ACampfire::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);
	if(EBuildingState::Build == m_BuildingState)
	{
		SetOutLineEffectEnable(true);
		SetItemInteractionUIText(Player, HitComponent);
	}
}

void ACampfire::OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollision(Player, HitComponent);
	SetItemInteractionUIText(Player, HitComponent);
}

void ACampfire::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	SetOutLineEffectEnable(false);
	Player->SetItemInteractionUIText(false);
}

void ACampfire::SetBurnEffectEnable(bool Enable, float EffectSpeed)
{
	//Super::SetBurnEffectEnable(Enable, EffectSpeed);
	m_ParticleSystem->Deactivate();
	m_AudioComponent->Stop();

	if(nullptr != m_FoodActor)
	{
		m_FoodActor->Destroy();
	}
	
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

void ACampfire::ExecuteBurnEffect(float DeltaTime)
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

void ACampfire::EffectFinished()
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

void ACampfire::SetOutLineEffectEnable(bool Enable)
{
	m_MeshComponent->SetRenderCustomDepth(Enable);
	m_MeshComponent->SetCustomDepthStencilValue(255);
	
	for(int32 i = 0; i < m_FirewoodCount; ++i)
	{
		m_ArrayFirewood[i]->SetRenderCustomDepth(Enable);
		m_ArrayFirewood[i]->SetCustomDepthStencilValue(255);
	}
}

void ACampfire::CookingAction(float DeltaTime)
{
	// 장작이 없음 // 음식이 없음 // 음식 조리 완료 
	if(m_FirewoodCount <= 0 || false == m_FoodEnable || m_FoodFinish)
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
	
	m_FoodAccTime += DeltaTime;
	m_FirewoodAccTime += DeltaTime;

	if(m_FirewoodAccTime >= m_FirewoodMaxTime)
	{
		m_FirewoodAccTime = 0.f;
		m_ArrayFirewood[m_FirewoodCount -1]->SetMaterial(0, m_AlphaMaterial);
		--m_FirewoodCount;
	}
	
	if(m_FoodAccTime >= m_FoodMaxTime)
	{
		m_FoodAccTime = 0.f;
		m_FoodFinish = true;

		// 음식 완료 된 것으로 Material 바꾸기
		FoodCooked();

		m_ParticleSystem->Deactivate();
		m_AudioComponent->Stop();
	}
}

void ACampfire::FoodCooked()
{
	m_FoodName = m_FoodActor->GetFoodName() + "_Cooked";
	SetFoodActor(m_FoodName);
}

void ACampfire::SetFoodActor(const FString& FoodName)
{
	if(nullptr != m_FoodActor)
	{
		m_FoodActor->Destroy();
	}
	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr == GameInst){ return; }

	const FEquipmentTableInfo* Info = GameInst->FindEquipmentInfo(FoodName);
	if (nullptr == Info || nullptr == Info->Montage) { return; }
	
	
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_FoodActor = GetWorld()->SpawnActor<AFood>(Info->Equipment, FVector::ZeroVector, FRotator::ZeroRotator, param);
	m_FoodActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	m_FoodActor->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, m_FoodLocationComponent->GetRelativeLocation(), FVector::OneVector));
}


void ACampfire::SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if(EBuildingState::Build != m_BuildingState) { return; }
	
	if(IsFirewoodComponent(HitComponent) && m_FirewoodCount < m_FirewoodMaxCount)
	{
		Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Add Firewood"));
	}
	// InteractionFood
	else if(IsFoodComponent(HitComponent))
	{
		// 음식이 있고, 조리 완료 라면
		if(m_FoodEnable && m_FoodFinish)
		{
			Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Take Food"));
		}
		// 음식이 있고, 조리 완료 x
		else if(m_FoodEnable && false == m_FoodFinish)
		{
			// 아무 일 없음
			Player->SetItemInteractionUIText(false);
		}
		// 음식이 없음 -> 조리 당연 x
		else if(false == m_FoodEnable)
		{
			Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Add Food"));
		}
		else
		{
			Player->SetItemInteractionUIText(false);
		}
	}
	else
	{
		Player->SetItemInteractionUIText(false);
	}
}
