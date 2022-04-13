// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnimType : uint8
{
	Ground		,
	Jump		,
	Fall		,
	Swimming	,
	Death		,
	Crouch		,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageSectionEnd);

UCLASS()
class RAFTDEDICATED_API UPlayerCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerCharacterAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerAnimType m_AnimType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool	m_bOnEquip;


public:
	void SetOnEquip(bool Enable) { m_bOnEquip = Enable; }
	float GetSpeed() const {return m_Speed;}
	float GetDirection() const {return m_Direction;}
	EPlayerAnimType GetAnimType() const { return m_AnimType; }
	
	void SetAnimType(EPlayerAnimType AnimType) { m_AnimType = AnimType; }

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	
	UFUNCTION()
	void AnimNotify_Use();

	UFUNCTION()
	void AnimNotify_UseEnd();

	UFUNCTION()
	void AnimNotify_GrabEnd();

	UFUNCTION()
	void AnimNotify_Swim();

	UFUNCTION()
	void AnimNotify_BeginCharging();

	UFUNCTION()
	void AnimNotify_EndCharging();

};
