// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "FishingRod.h"
#include "FishingRodAnim.generated.h"




UCLASS()
class RAFTDEDICATED_API UFishingRodAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFishingRodAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AFishingRod*			m_Owner;


public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);


public:
	UFUNCTION()
	void AnimNotify_Return();
};
