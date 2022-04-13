// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item.h"
#include "Equipment.generated.h"


class APlayerCharacter;
class UPlayerCharacterAnim;


UCLASS()
class RAFTDEDICATED_API AEquipment : public AItem
{
	GENERATED_BODY()

public:
	AEquipment();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName					m_SocketName;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	APlayerCharacter*		m_Owner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMeshComponent*			m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPlayerCharacterAnim*	m_OwnerAnim;
	
	bool					m_bCharging;
	
	bool					m_bChargingFull;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_ChargingTime;
	
	float					m_ChargingAccTime;


public:
	float GetChargingTime() const { return m_ChargingTime; }
	float GetChargingRatio() const;
	bool IsCharging() const { return m_bCharging; }
	bool IsChargeEquip() const { return m_ChargingTime > 0.f; }
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual bool OnEquip(APlayerCharacter* EquipOwner);
	virtual bool OnEquip(APlayerCharacter* EquipOwner, UMeshComponent* Mesh);
	virtual void UnEquip();

	virtual void LButtonAction(bool bPress) {}
	virtual void RButtonAction(bool bPress) {}

	virtual void Key_R_Action() {}
	virtual void Key_T_Action() {}
	virtual void Key_E_Action() {}
	
	virtual void Use();
	virtual void UseEnd();
	
	virtual void BeginCharging();
	virtual void EndCharging();
	virtual void FullCharging();
	
protected:
	void AttachItem(UMeshComponent* Mesh);
	
};


