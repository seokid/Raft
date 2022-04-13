// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Arrow.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"

ABow::ABow()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("m_Mesh"));
	SkelMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RodAsset(TEXT("SkeletalMesh'/Game/Item/Equipment/Weapon/Mesh/SK_Bow.SK_Bow'"));
	if (RodAsset.Succeeded())
	{
		SkelMesh->SetSkeletalMesh(RodAsset.Object);
	}

	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Item/Equipment/Weapon/ABP_Bow.ABP_Bow_C'"));
	
	if (AnimAsset.Succeeded())
	{
		SkelMesh->SetAnimInstanceClass(AnimAsset.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> ArrowAsset(TEXT("Blueprint'/Game/Item/Equipment/Weapon/BP_Arrow.BP_Arrow_C'"));
	if (ArrowAsset.Succeeded())
	{
		m_ArrowClass = ArrowAsset.Class;
	}
	
	m_ChargingTime = 2.f;
	m_Mesh = SkelMesh;
	m_Type = EBowType::Idle;
	m_ThrowPower = 100.f;
	SetRootComponent(m_Mesh);

	m_SocketName = TEXT("hand_r_bow");
}

void ABow::BeginPlay()
{
	Super::BeginPlay();
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABow::Use()
{
	Super::Use();

	LOG(TEXT("Use"));

	int ItemCount = 1;
	if(m_Owner->GetMainHUD()->RemoveItem(TEXT("Arrow"), ItemCount))
	{
		AArrow* Arrow = GetWorld()->SpawnActor<AArrow>(m_ArrowClass, m_Mesh->GetSocketLocation(TEXT("StringMiddle")), m_Owner->GetCamera()->GetForwardVector().Rotation());
		if (Arrow)
		{
			Arrow->SetInitSpeed(2000.f * (1.f + GetChargingRatio()));
		}
	}
}

void ABow::UseEnd()
{
	Super::UseEnd();

	LOG(TEXT("UseEnd"));

	m_Type = EBowType::Idle;
}

void ABow::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);

	if (EBowType::Idle == m_Type && bPress)
	{
		m_OwnerAnim->Montage_JumpToSection(TEXT("Load"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EBowType::Load;
	}
	else if (EBowType::Load == m_Type && !bPress)
	{
		m_OwnerAnim->Montage_JumpToSection(TEXT("Throw"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EBowType::Throw;
	}
	
}
