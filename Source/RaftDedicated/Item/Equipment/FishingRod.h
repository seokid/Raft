// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CableComponent.h"
#include "Equipment.h"
#include "../../Component/DataTableAudioComponent.h"
#include "FishingRod.generated.h"


UENUM(BlueprintType)
enum class EFishingType : uint8
{
	Idle,
	Load,
	Throw,
	Wait,
	Fighting,
	Retract,
};

UCLASS()
class RAFTDEDICATED_API AFishingRod : public AEquipment
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*		m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTableAudioComponent*        m_AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCableComponent*	m_Cable;
	
;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EFishingType			m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UFishingRodAnim*	m_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ABobber*			m_Bobber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FItemDropInfo>		m_ArrayItemDropInfo;

	int32					m_DropIdx;

	
public:
	AFishingRod();

public:
	EFishingType GetType() const { return m_Type; }

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	virtual void Use() override;
	virtual void UseEnd() override;

	virtual void LButtonAction(bool bPress) override;
	virtual void UnEquip() override;
	void Return();

private:
	void Pull();
	void Fighting();
	void Retract();

	
	
};
