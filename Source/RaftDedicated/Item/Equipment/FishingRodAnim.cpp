// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRodAnim.h"

UFishingRodAnim::UFishingRodAnim()
{
	m_Owner = nullptr;
}


void UFishingRodAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Owner = Cast<AFishingRod>(GetSkelMeshComponent()->GetOwner());
	
}

void UFishingRodAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
}

void UFishingRodAnim::AnimNotify_Return()
{
	LOG(TEXT("Return"));
	if (m_Owner)
		m_Owner->Return();
}
