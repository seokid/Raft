// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleFoliage.h"

#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"

ADestructibleFoliage::ADestructibleFoliage()
{
	PrimaryActorTick.bCanEverTick = true;
	
	m_Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DesturctibleMesh"));
	m_Mesh->SetupAttachment(m_RootComponent);
	m_Mesh->SetCollisionProfileName(TEXT("Foliage"));
	m_bDestroy = false;

	m_EffectAccTime = 0.f;
	m_EffectMaxTime = 5.f;
	m_EffectBoundary = 0.5f;
	m_EffectSpeed = -1.f;
}

void ADestructibleFoliage::BeginPlay()
{
	Super::BeginPlay();

	
}

void ADestructibleFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_bDestroy)
	{
		ExecuteBurnEffect(DeltaTime);
	}
}

void ADestructibleFoliage::Gather(APlayerCharacter* Player, float Damage, float Impulse)
{
	Super::Gather(Player);
	Destroy(Damage, m_Mesh->GetComponentLocation(), Player->GetActorForwardVector(), Impulse);
}

void ADestructibleFoliage::Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	if (!m_bDestroy)
	{
		m_bDestroy = true;
		m_Mesh->ApplyDamage(Damage, HitLocation, ImpulseDir, Impulse);
		SetBurnEffectEnable(m_bDestroy, -1.f);
	}
}

void ADestructibleFoliage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if (!m_bDestroy)
		Super::OnCollisionEnter(Player, HitComponent);
}

void ADestructibleFoliage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	if (!m_bDestroy)
		Super::OnCollisionExit(Player, HitComponent);
}

void ADestructibleFoliage::SetBurnEffectEnable(bool Enable, float EffectSpeed)
{
	if(Enable)
	{
		m_Mesh->SetScalarParameterValueOnMaterials(TEXT("Enable"), 1.f);
	}
	else
	{
		m_Mesh->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
	}
	
	m_EffectSpeed = EffectSpeed;
	if(EffectSpeed > 0.f)
	{
		m_EffectAccTime = 0.f;
	}
	else
	{
		m_EffectAccTime = m_EffectMaxTime;
	}
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_EffectAccTime);
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("MaxTime"), m_EffectMaxTime);
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("Ramp Boundary"), m_EffectBoundary);
}

void ADestructibleFoliage::ExecuteBurnEffect(float DeltaTime)
{
	m_EffectAccTime += (DeltaTime * m_EffectSpeed);
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_EffectAccTime);

	if((m_EffectSpeed > 0.f && m_EffectAccTime >= m_EffectMaxTime) || (m_EffectSpeed < 0.f && m_EffectAccTime <= 0.f))
	{
		EffectFinished();
	}
}

void ADestructibleFoliage::EffectFinished()
{
	m_EffectAccTime = 0.f;
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("Enable"), -1.f);
	m_Mesh->SetScalarParameterValueOnMaterials(TEXT("AccTime"), m_EffectAccTime);
	Super::Destroy();
}
