// Fill out your copyright notice in the Description page of Project Settings.


#include "Male.h"

AMale::AMale()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelAsset(TEXT("SkeletalMesh'/Game/Unit/Player/Male/Mesh/SK_PC_00.SK_PC_00'"));

	if (SkelAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkelAsset.Object);
	}
	
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Unit/Player/Male/ABP_Male.ABP_Male_C'"));

	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("AnimMontage'/Game/Unit/Player/Male/AM_Male_Hook.AM_Male_Hook'"));
	
	if (Montage.Succeeded())
	{
		m_AnimMontage = Montage.Object;
	}
	

	m_PlayerInfoName = TEXT("Male");
	m_NoseBoneName = TEXT("nose");
}