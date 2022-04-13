// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"

#include "Bobber.h"
#include "FishingRodAnim.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"

AFishingRod::AFishingRod()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USkeletalMeshComponent* SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("m_Mesh"));
	SkelMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RodAsset(TEXT("SkeletalMesh'/Game/Item/Equipment/Weapon/Mesh/SK_FishingRod.SK_FishingRod'"));
	if (RodAsset.Succeeded())
	{
		SkelMesh->SetSkeletalMesh(RodAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Item/Equipment/Weapon/ABP_FishingRod.ABP_FishingRod_C'"));
	
	if (AnimAsset.Succeeded())
	{
		SkelMesh->SetAnimInstanceClass(AnimAsset.Class);
	}
	
	m_Mesh = SkelMesh;
	SetRootComponent(m_Mesh);
	
	// m_Bobber = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_Bobber"));
	// m_Bobber->SetupAttachment(m_RootComponent);
	// m_Bobber->SetMassOverrideInKg(NAME_None, 30.f);
	//
	// // m_Bobber->SetSimulatePhysics(true);
	// // m_Bobber->SetMassOverrideInKg(NAME_None, 30.f);
	// // m_Bobber->SetupAttachment(m_Mesh, TEXT("Bone"));
	// // m_Bobber->SetRelativeScale3D(FVector(0.01f));
	// // m_Bobber->AttachToComponent(m_Mesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bone"));
	// m_Bobber->SetCollisionProfileName(TEXT("Equipment"), false);
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> BobberAsset(TEXT("StaticMesh'/Game/Item/Equipment/Mesh/SM_Bobber.SM_Bobber'"));
	// if (BobberAsset.Succeeded())
	// {
	// 	m_Bobber->SetStaticMesh(BobberAsset.Object);
	// }
	// m_Rope = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Rope"));
	// m_Rope->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));	//부모 스케일 영향 상쇄
	// m_Rope->SetupAttachment(m_Mesh, TEXT("Head"));
	//
	// m_Rope->ComponentName1 = {TEXT("m_Mesh")};
	// m_Rope->ComponentName2 = {TEXT("m_Bobber")};
	//
	// m_Rope->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 50.f);
	// m_Rope->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 50.f);
	// m_Rope->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 50.f);
	//
	// m_Rope->SetLinearVelocityDrive(true, true, true);
	// m_Rope->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	// m_Rope->SetAngularVelocityDriveTwistAndSwing(true, true);
	//
	// m_Rope->ConstraintInstance.bScaleLinearLimits = true;
	// m_Rope->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
	// m_Rope->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 2000.f;
	// m_Rope->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 10.f;
	// m_Rope->ConstraintInstance.ProfileInstance.LinearLimit.ContactDistance = 5.f;
	//
	//
	m_Cable = CreateDefaultSubobject<UCableComponent>(TEXT("m_Cable"));
	m_Cable->SetRelativeScale3D(FVector(0.01f));	//부모 스케일 영향 상쇄
	m_Cable->SetupAttachment(m_Mesh, TEXT("Head"));
	
	m_Cable->EndLocation = FVector::ZeroVector;
	m_Cable->CableLength = 30.f;
	m_Cable->CableWidth = 1.f;
	m_Cable->bEnableStiffness = true;
	
	m_AudioComponent = CreateDefaultSubobject<UDataTableAudioComponent>(TEXT("Audio"));
	m_AudioComponent->SetupAttachment(GetRootComponent());

	m_SocketName = TEXT("hand_r_fishingrod");
	
	m_ChargingTime = 2.f;
	
	m_Type = EFishingType::Idle;

	m_DropIdx = -1;
}

void AFishingRod::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<UFishingRodAnim>(Cast<USkeletalMeshComponent>(m_Mesh)->GetAnimInstance());

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_Bobber = GetWorld()->SpawnActor<ABobber>(ABobber::StaticClass(), m_Mesh->GetComponentLocation(), FRotator::ZeroRotator, param);
	
	if (m_Bobber)
	{
		m_Bobber->AttachToMove(m_Mesh, 0.2f, FVector::ZeroVector, FRotator::ZeroRotator, TEXT("IdleBobber"));
		m_Cable->SetAttachEndTo(m_Bobber, TEXT("m_Mesh"), TEXT("Head"));
	}
	
}

void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AFishingRod::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);

	if (EFishingType::Idle == m_Type && bPress)
	{
		m_Bobber->AttachToMove(m_Mesh, 1.f, FVector::ZeroVector, FRotator::ZeroRotator, TEXT("HeadBobber"));
		m_OwnerAnim->Montage_JumpToSection(TEXT("Load"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EFishingType::Load;
	}
	else if(EFishingType::Load == m_Type && !bPress)
	{
		m_OwnerAnim->Montage_SetNextSection(TEXT("LoadWait"), TEXT("Throw"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EFishingType::Throw;
	}
	else if (EFishingType::Wait == m_Type && !bPress)
	{
		Pull();
	}
}

void AFishingRod::UnEquip()
{
	Super::UnEquip();
	if (m_Bobber)
		m_Bobber->Destroy();
}

void AFishingRod::Use()
{
	Super::Use();

	m_AudioComponent->PlayDataTableSound(TEXT("Escape"));
	m_Type = EFishingType::Wait;
	m_Bobber->Throw(m_Owner->GetCamera()->GetForwardVector() * 2000.f * (1.f + GetChargingRatio()));
	
	m_DropIdx = -1;
}

void AFishingRod::UseEnd()
{
	Super::UseEnd();

	m_Type = EFishingType::Idle;

	m_OwnerAnim->Montage_SetPosition(m_Owner->GetEquipInfo().Montage, 0.f);
	m_OwnerAnim->Montage_Play(m_Owner->GetEquipInfo().Montage);
	
}

void AFishingRod::Pull()
{
	const int32 Num = FMath::RandRange(0, 99);
	
	int32 Sum = 0;
	for (int32 i = 0; i < m_ArrayItemDropInfo.Num(); ++i)
	{
		if (Sum <= Num && Num < Sum + m_ArrayItemDropInfo[i].DropPercentage)
		{
			m_DropIdx = i;
			Fighting();
			return;
		}
		Sum += m_ArrayItemDropInfo[i].DropPercentage;
	}
	Retract();
}

void AFishingRod::Fighting()
{
	m_Type = EFishingType::Fighting;
	m_AudioComponent->PlayDataTableSound(TEXT("StrectchLong"));
	m_OwnerAnim->Montage_JumpToSection(TEXT("Fighting"), m_OwnerAnim->GetCurrentActiveMontage());
}

void AFishingRod::Retract()
{
	m_Type = EFishingType::Retract;
	m_OwnerAnim->Montage_JumpToSection(TEXT("Retract"), m_OwnerAnim->GetCurrentActiveMontage());
}

void AFishingRod::Return()
{
	if (-1 < m_DropIdx)
	{
		int32 ItemCount = FMath::RandRange(m_ArrayItemDropInfo[m_DropIdx].DropMinCount, m_ArrayItemDropInfo[m_DropIdx].DropMaxCount);
		m_Owner->GetMainHUD()->AddItem(m_ArrayItemDropInfo[m_DropIdx].Name, ItemCount);
	}
	
	m_AudioComponent->PlayDataTableSound(TEXT("Strectch"));
	m_Bobber->AttachToMove(m_Mesh, 0.2f, FVector::ZeroVector, FRotator::ZeroRotator, TEXT("IdleBobber"));
}



