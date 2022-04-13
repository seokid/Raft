// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolHammer.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/HammerItemList.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "../../RaftDedicatedGameInstance.h"
#include "RaftDedicated/Effect/NormalSound.h"
#include "RaftDedicated/Item/Building/Building.h"

TArray<FString> AToolHammer::m_BuildingList;

AToolHammer::AToolHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);

	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolHammer"));
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HammerAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Tool_Hammer.SM_Tool_Hammer'"));
	if (HammerAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(HammerAsset.Object);
	}
	m_Mesh = StaticMesh;
	m_Mesh->SetupAttachment(m_RootComponent);

	m_Type = EToolHammerType::Idle;
	m_SocketName = TEXT("hand_r_hammer");

	m_BuildingRotationIdx = 0;
	m_BuldingInfoIdx = 0;

	m_BuildingList  = {TEXT("Block_Foundation")
					, TEXT("Block_Stair_Wood")
					, TEXT("Block_Floor_Wood")
					, TEXT("Block_Stair_Wood_Half")
					, TEXT("Block_Pillar_Wood")
					, TEXT("Block_Pillar_Wood_Half")};

	// Use Audio
	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("UseAudio"));
	m_AudioComponent->SetupAttachment(m_RootComponent);
}


void AToolHammer::BeginPlay()
{
	Super::BeginPlay();
}

void AToolHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AToolHammer::OnEquip(APlayerCharacter* EquipOwner)
{
	if (!Super::OnEquip(EquipOwner))
		return false;

	m_Owner->SetBuildEnable(false);
	m_BuildingRotationIdx = 0;
	m_Owner->GetMainHUD()->SetHammerItemListWidget(false);
	
	// m_Owner->SetBuildEnable(true);
	// m_BuildingRotationIdx = 0;
	// m_Owner->GetMainHUD()->SetHammerItemListWidget(true, m_BuildingList[m_BuldingInfoIdx]);
	// m_Owner->SetBuildInfo(m_BuildingList[m_BuldingInfoIdx]);
	return true;
}

void AToolHammer::UnEquip()
{
	if (!m_Owner)
		return;
	
	m_Owner->SetBuildEnable(false);
	m_Owner->GetMainHUD()->SetHammerItemListWidget(false);
	Super::UnEquip();
}

void AToolHammer::Use()
{
	Super::Use();
}

void AToolHammer::UseEnd()
{
	Super::UseEnd();

	m_Type = EToolHammerType::Idle;
}

void AToolHammer::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);
	
	if (bPress && m_Owner->GetBuildEnable())
	{
		if (EToolHammerType::Hit == m_Type)
			m_OwnerAnim->Montage_SetPosition(m_OwnerAnim->GetCurrentActiveMontage(), 0.f);
		m_OwnerAnim->Montage_JumpToSection(TEXT("Hit"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EToolHammerType::Hit;

		if(m_Owner->GetMainHUD()->GetHammerItemListWidget()->IsBuildPossible())
		{
			if(m_Owner->Building_Build())
			{
				m_Owner->SetBuildInfo(m_BuildingList[m_BuldingInfoIdx]);
				m_Owner->GetMainHUD()->GetHammerItemListWidget()->Building_Build();
				m_Owner->GetMainHUD()->SetHammerItemListWidget(true, m_BuildingList[m_BuldingInfoIdx]);

				m_AudioComponent->SetSound(m_BuildSound);
				m_AudioComponent->Play();
			}
		}
	}
}

void AToolHammer::RButtonAction(bool bPress)
{
	Super::RButtonAction(bPress);

	if (bPress)
	{
		if (EToolHammerType::Hit == m_Type)
			m_OwnerAnim->Montage_SetPosition(m_OwnerAnim->GetCurrentActiveMontage(), 0.f);
		m_OwnerAnim->Montage_JumpToSection(TEXT("Hit"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EToolHammerType::Hit;

		m_Owner->SetBuildEnable(true);
		if(m_Owner->Building_Break())
		{
			m_AudioComponent->SetSound(m_BreakSound);
			m_AudioComponent->Play();
		}
		m_Owner->SetBuildEnable(false);
		
		m_Owner->GetMainHUD()->SetHammerItemListWidget(false);
	}
}

void AToolHammer::Key_R_Action()
{
	m_BuildingRotationIdx = (m_BuildingRotationIdx + 1) % 4;
}

void AToolHammer::Key_T_Action()
{
	ChangeBuilding();
}

void AToolHammer::ChangeBuilding()
{
	if(m_Owner->GetBuildEnable())
	{
		m_BuldingInfoIdx = (m_BuldingInfoIdx + 1) % m_BuildingList.Num();
	}
	m_Owner->SetBuildInfo(m_BuildingList[m_BuldingInfoIdx]);
	m_Owner->SetBuildEnable(true);
	m_Owner->GetMainHUD()->SetHammerItemListWidget(true, m_BuildingList[m_BuldingInfoIdx]);
}
