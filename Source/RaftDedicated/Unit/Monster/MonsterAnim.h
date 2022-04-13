// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnim.generated.h"


UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Move,
	Attack,
	Damaged,
	Dead,
};
/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UMonsterAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnim();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_Direction;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	m_Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMonsterAnimType	m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool	m_bAttack;


public:
	
	EMonsterAnimType GetAnimType() const { return m_AnimType; }
    
	void SetAnimType(EMonsterAnimType AnimType) { m_AnimType = AnimType; }

	bool IsAttack() const { return m_bAttack; }
	void SetAttack(bool bEnable) { m_bAttack  = bEnable; }
    
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	
	UFUNCTION()
	void AnimNotify_NormalAttack();

	UFUNCTION()
	void AnimNotify_NormalAttackEnd();
};
