// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "FoodContainer.generated.h"


class AFood;

UCLASS()
class RAFTDEDICATED_API AFoodContainer : public AEquipment
{
	GENERATED_BODY()

public:
	AFoodContainer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*		m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AFood* m_Food;

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;

	virtual void LButtonAction(bool bPress) override;



protected:
	virtual void PutIn(AFood* Food);
	virtual void ThrowOut();

	
};
