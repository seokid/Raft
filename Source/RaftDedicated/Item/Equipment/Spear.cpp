// Fill out your copyright notice in the Description page of Project Settings.


#include "Spear.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "../../Effect/Effect.h"

ASpear::ASpear()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_Mesh"));
	Mesh->SetCollisionProfileName(TEXT("Equipment"), false);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RodAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Spear.SM_Spear'"));
	if (RodAsset.Succeeded())
	{
		Mesh->SetStaticMesh(RodAsset.Object);
	}
	
	m_Mesh = Mesh;
	SetRootComponent(m_Mesh);


	m_AttackDistance = 3000.f;
	m_SocketName = TEXT("hand_r_spear");
	
}

void ASpear::BeginPlay()
{
	Super::BeginPlay();
}

void ASpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpear::Use()
{
	Super::Use();
	
	FVector HeadLoc = m_Mesh->GetSocketLocation(TEXT("Head"));
	

    FCollisionQueryParams params(NAME_None, false, this);
    
    FHitResult result;
    
    bool Sweep = GetWorld()->SweepSingleByChannel(result
    	, HeadLoc
    	, HeadLoc + m_Owner->GetActorForwardVector() * m_AttackDistance
    	, FQuat::Identity
    	, ECollisionChannel::ECC_GameTraceChannel9
    	, FCollisionShape::MakeSphere(50.f), params);
    
    FHitResult CollisionArray = {};

	
// #if ENABLE_DRAW_DEBUG
// 	FColor DrawColor = Sweep ? FColor::Red : FColor::Green;
//
// 	FVector Center = HeadLoc + m_Owner->GetActorForwardVector() * (m_AttackDistance / 2.f);
// 	DrawDebugCapsule(GetWorld(), Center, m_AttackDistance / 2.f
// 		, 50.f, FRotationMatrix::MakeFromZ(m_Owner->GetActorForwardVector()).ToQuat()
// 		, DrawColor, false, 0.5f);
// #endif
    
    if (Sweep)
    {
    	FActorSpawnParameters param;
    	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    	AEffect* Effect = GetWorld()->SpawnActor<AEffect>(AEffect::StaticClass(), result.ImpactPoint, result.ImpactNormal.Rotation(), param);
    	
    	Effect->LoadParticle(TEXT("ParticleSystem'/Game/FXVillesBloodVFXPack/Particles/Cascade/Real/P_Blood_Spurt_Small.P_Blood_Spurt_Small'"));
    	Effect->SetActorScale3D(FVector(3.f, 3.f, 3.f));
    	
    	FDamageEvent DmgEvent = {};
    	
    	result.GetActor()->TakeDamage(m_Owner->GetPlayerInfo().Attack, DmgEvent, m_Owner->GetController(), this);
    }
}

void ASpear::UseEnd()
{
	Super::UseEnd();
	
	m_Type = ESpearType::Idle;
	
}

void ASpear::LButtonAction(bool bPress)
{
	Super::LButtonAction(bPress);

	if (ESpearType::Idle != m_Type)
		return;
		
	if (bPress)
	{
		m_OwnerAnim->Montage_JumpToSection(TEXT("Hit"), m_OwnerAnim->GetCurrentActiveMontage());
		m_Type = ESpearType::Hit;
	}
}
