// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAnim.h"
#include "SharkAnim.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API USharkAnim : public UMonsterAnim
{
	GENERATED_BODY()

public:
	USharkAnim();
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:

	UFUNCTION()
	void AnimNotify_OpenMouth();
	
	UFUNCTION()
	void AnimNotify_Dash();
	
	UFUNCTION()
	void AnimNotify_Swim();

	
	
};
