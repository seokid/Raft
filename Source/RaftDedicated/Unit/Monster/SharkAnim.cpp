// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAnim.h"
#include "Shark.h"

USharkAnim::USharkAnim()
{
}

void USharkAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USharkAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USharkAnim::AnimNotify_OpenMouth()
{
	if (AShark* Shark = Cast<AShark>(TryGetPawnOwner()))
	{
		Shark->OpenMouth();	
	}
}

void USharkAnim::AnimNotify_Dash()
{
	if (AShark* Shark = Cast<AShark>(TryGetPawnOwner()))
	{
		Shark->Dash();	
	}
}

void USharkAnim::AnimNotify_Swim()
{
	if (AShark* Shark = Cast<AShark>(TryGetPawnOwner()))
	{
		Shark->Swim();	
	}
	
}
