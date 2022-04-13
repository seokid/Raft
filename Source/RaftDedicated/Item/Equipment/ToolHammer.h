// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "ToolHammer.generated.h"

UENUM(BlueprintType)
enum class EToolHammerType : uint8
{
	Idle,
	Hit,
};

UCLASS()
class RAFTDEDICATED_API AToolHammer : public AEquipment
{
	GENERATED_BODY()

private:
	static TArray<FString>		m_BuildingList;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*			m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EToolHammerType				m_Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString						m_BuildingName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32						m_BuildingRotationIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32						m_BuldingInfoIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent*		m_AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_BuildSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_BreakSound;
	
public:
	AToolHammer();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool OnEquip(APlayerCharacter* EquipOwner) override;
	virtual void UnEquip() override;
	
	virtual void Use() override;
	virtual void UseEnd() override;
	virtual void LButtonAction(bool bPress) override;
	virtual void RButtonAction(bool bPress) override;

	virtual void Key_R_Action() override;
	virtual void Key_T_Action() override;

public:
	int32 GetBuildingRotationIdx() { return m_BuildingRotationIdx; }

private:
	void ChangeBuilding();



};
