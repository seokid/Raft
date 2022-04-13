// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalMaterialBase.h"

UPhysicalMaterialBase::UPhysicalMaterialBase()
{
	m_Sound = nullptr;
	m_Particle = nullptr;
}

void UPhysicalMaterialBase::SetSound(const FString& Path)
{
	m_Sound = LoadObject<USoundBase>(this, *Path);
}

void UPhysicalMaterialBase::SetParticle(const FString& Path)
{
	m_Particle = LoadObject<UParticleSystem>(this, *Path);
}
