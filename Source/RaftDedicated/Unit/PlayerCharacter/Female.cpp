// Fill out your copyright notice in the Description page of Project Settings.


#include "Female.h"

AFemale::AFemale()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelAsset(TEXT("SkeletalMesh'/Game/Unit/Player/Female/Mesh/SK_Female.SK_Female'"));

    if (SkelAsset.Succeeded())
    {
    	GetMesh()->SetSkeletalMesh(SkelAsset.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Unit/Player/Female/ABP_Female.ABP_Female_C'"));

    if (AnimAsset.Succeeded())
    {
    	GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
    }

	m_PlayerInfoName = TEXT("Female");
}
