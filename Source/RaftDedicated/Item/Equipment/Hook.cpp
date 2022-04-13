// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"
#include "Rope.h"
#include "../Floatage/Floatage.h"
#include "../../Effect/NormalSound.h"

#include "BuoyancyComponent.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"

#include "../Foliage/ScrapFoliage.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"));
	StaticMesh->SetConstraintMode(EDOFMode::SixDOF);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HookAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Hook.SM_Hook'"));
	if (HookAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(HookAsset.Object);
	}

	m_AudioComponent = CreateDefaultSubobject<UDataTableAudioComponent>(TEXT("Audio"));
	if (m_AudioComponent)
	{
		m_AudioComponent->SetupAttachment(GetRootComponent());
		m_AudioComponent->bOverrideAttenuation = true;
		m_AudioComponent->bAutoActivate = false;
	}
	
	m_Mesh = StaticMesh;
	m_SocketName = TEXT("hand_r_hook");

	m_Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Bouyancy"));
	m_Buoyancy->AddCustomPontoon(100.f, FVector::ZeroVector);

	m_ThrowPower = 6000.f;
	m_ScratchPower = 3000.f;
	m_OriginPos = FVector::ZeroVector;
	
	m_ThrowCharingTime = 2.f;
	m_ChargingTime = m_ThrowCharingTime;

	m_TailSocketName = TEXT("Tail");

	m_Type = EHookType::Idle;
}

void AHook::BeginPlay()
{
	Super::BeginPlay();
	
	m_Mesh->OnComponentBeginOverlap.AddDynamic(this, &AHook::HookBeginOverlap);
	m_Mesh->OnComponentEndOverlap.AddDynamic(this, &AHook::HookEndOverlap);
}

void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_Owner)
		return;
	
	AScrapFoliage* Foliage = Cast<AScrapFoliage>(m_Owner->GetInteractionActor());
	if (m_ScrapFoliage != Foliage)
	{
		m_ScrapFoliage = Foliage;
		//m_Owner->SetItemInteractionUIText(Foliage, TEXT("LButton"), TEXT("Gather"));
		if (!Foliage)
			GatherFinish();
	}
	
	if (!m_Mesh->IsSimulatingPhysics())
		return;
	
	if (EHookType::Pull == m_Type)
	{
		FVector Dir = m_Owner->GetActorLocation() - m_Mesh->GetRelativeLocation();
		
		if (150.f > Dir.Size())
			ScratchFinish();
		else
		{
			Dir.Normalize();
			m_Mesh->AddForce(Dir * m_ScratchPower * m_Mesh->GetBodyInstance()->GetMassOverride(), m_TailSocketName);
		}
	}
}

bool AHook::OnEquip(APlayerCharacter* EquipOwner)
{
	if (!Super::OnEquip(EquipOwner))
		return false;
	
	FActorSpawnParameters param;
    param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    m_Rope = GetWorld()->SpawnActor<ARope>(ARope::StaticClass(), m_Mesh->GetComponentLocation(), FRotator::ZeroRotator, param);
    if (m_Rope)
    {
    	m_Rope->OnEquip(m_Owner);
    	m_Rope->RopeConnect(this, m_TailSocketName);
    }

	return true;
}

void AHook::UnEquip()
{
	m_Rope->UnEquip();
	Super::UnEquip();
}

void AHook::Use()
{
	Super::Use();
	Throw();
}

void AHook::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);
	
	if (EHookType::Idle == m_Type && bPress)
	{
		if (m_ScrapFoliage)
		{
			GatherStart();
		}
		else if (m_Owner->GetCharacterMovement()->IsMovingOnGround())
		{
			m_ChargingTime = m_ThrowCharingTime;
			m_OwnerAnim->Montage_JumpToSection(TEXT("Load"), m_OwnerAnim->GetCurrentActiveMontage());
			m_Type = EHookType::Load;	
		}
	}
	else if (EHookType::Load == m_Type && !bPress)
	{
		m_OwnerAnim->Montage_SetNextSection(TEXT("LoadWait"), TEXT("Throw"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = EHookType::Throw;
	}
	else if (EHookType::Gather == m_Type && !bPress)
	{
		GatherFinish();
	}
}

void AHook::RButtonAction(bool bPress)
{
	Super::RButtonAction(bPress);

	if ((EHookType::Throw == m_Type || EHookType::Pull == m_Type) && bPress)
    {
    	Withdraw();
    }
}

void AHook::FullCharging()
{
	Super::FullCharging();
	
	if (m_ScrapFoliage)
	{
		EndCharging();
		m_ScrapFoliage->Gather(m_Owner, 1.f, 1.f);
		m_AudioComponent->PlayDataTableSound(TEXT("Impact"));
		GatherFinish();
	}
}

void AHook::Throw()
{
	m_AudioComponent->PlayDataTableSound(TEXT("Throw"));
	m_Mesh->SetSimulatePhysics(true);
	m_OriginPos = m_Mesh->GetRelativeLocation();
	m_Mesh->AddImpulse(m_Owner->GetCamera()->GetForwardVector() * m_ThrowPower * m_Mesh->GetBodyInstance()->GetMassOverride() * (1.f + GetChargingRatio()), TEXT("Head"));
	m_Rope->SetRopeLength(100.f);
}

void AHook::ScratchStart()
{
	m_Type = EHookType::Pull;

	m_AudioComponent->PlayDataTableSound(TEXT("Pull"));
	FBodyInstance* BodyInst = m_Mesh->GetBodyInstance();
	m_Mesh->AddImpulse(m_Mesh->GetPhysicsLinearVelocity() * -1.f * BodyInst->GetMassOverride());
	m_Mesh->SetRelativeRotation(FRotator(0.f, m_Mesh->GetComponentRotation().Yaw,-50.f));
	
	BodyInst->bLockXRotation = true;
	BodyInst->bLockYRotation = true;
	BodyInst->bLockZRotation = true;
}

void AHook::ScratchFinish()
{
	for (const auto& Floatge : m_TakeFloatage)
	{
		Floatge->TakeItem(m_Owner);
	}
	m_AudioComponent->Stop();
	m_TakeFloatage.Empty();

	Withdraw();
}

void AHook::Withdraw()
{
	m_Type = EHookType::Idle;

	m_Mesh->SetSimulatePhysics(false);
	FBodyInstance* BodyInst = m_Mesh->GetBodyInstance();
	BodyInst->bLockXRotation = false;
	BodyInst->bLockYRotation = false;
	BodyInst->bLockZRotation = false;
	
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	m_Mesh->AttachToComponent(m_Owner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform, m_SocketName);
	UKismetSystemLibrary::MoveComponentTo(m_Mesh,FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.2f, true, EMoveComponentAction::Move, Info);
	m_Rope->SetRopeLength(10.f);
	
	m_OwnerAnim->Montage_SetPosition(m_Owner->GetEquipInfo().Montage, 0.f);
	m_OwnerAnim->Montage_Play(m_Owner->GetEquipInfo().Montage);

	for (const auto& Floatge : m_TakeFloatage)
	{
		Floatge->Destroy();
	}
	m_TakeFloatage.Empty();
}

void AHook::GatherStart()
{
	m_ChargingTime = m_ScrapFoliage->GetGatherTime();
	m_OwnerAnim->Montage_JumpToSection(TEXT("Gather"), m_OwnerAnim->GetCurrentActiveMontage());
    BeginCharging();
    m_Type = EHookType::Gather;
}

void AHook::GatherFinish()
{
	m_OwnerAnim->Montage_JumpToSection(TEXT("Idle"), m_OwnerAnim->GetCurrentActiveMontage());
	EndCharging();
	m_Type = EHookType::Idle;
}

void AHook::HookBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (EHookType::Throw == m_Type)
	{
		ScratchStart();
		
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, param);
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/watersplash_big02.watersplash_big02'"));
		Effect->Play();	
	}
	
	if (EHookType::Pull == m_Type)
	{
		AFloatage* Cath = Cast<AFloatage>(OtherActor);
		if (Cath)
		{
			m_AudioComponent->PlayDataTableSound(TEXT("Attach"));
			Cath->GetMesh()->SetSimulatePhysics(false);
			Cath->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			Cath->AttachToComponent(m_Mesh ,FAttachmentTransformRules::KeepWorldTransform);
			UKismetSystemLibrary::MoveComponentTo(Cath->GetMesh(),FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.1f, true, EMoveComponentAction::Move, Info);
			m_TakeFloatage.Add(Cath);
		}
	}
	
}

void AHook::HookEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}
