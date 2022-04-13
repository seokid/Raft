// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "Spear.generated.h"


UENUM(BlueprintType)
enum class ESpearType : uint8
{
	Idle,
	Hit,
};

UCLASS()
class RAFTDEDICATED_API ASpear : public AEquipment
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*				m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESpearType						m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_AttackDistance;
	
public:
	ASpear();


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Use() override;
	virtual void UseEnd() override;
	virtual void LButtonAction(bool bPress) override;
	
};
