// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "Bow.generated.h"

UENUM(BlueprintType)
enum class EBowType : uint8
{
	Idle,
	Load,
	Throw,
};

UCLASS()
class RAFTDEDICATED_API ABow : public AEquipment
{
	GENERATED_BODY()

public:
	ABow();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBowType	m_Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float		m_ThrowPower;

	TSubclassOf<class AArrow>	m_ArrowClass;

public:
	EBowType GetBowType() const { return m_Type; }

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Use() override;
	virtual void UseEnd() override;
	
	virtual void LButtonAction(bool bPress) override;
	
};
