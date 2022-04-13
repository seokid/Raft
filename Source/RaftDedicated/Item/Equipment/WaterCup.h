// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FoodContainer.h"
#include "Food.h"
#include "WaterCup.generated.h"

/**
 * 
 */



class AWater;

UCLASS()
class RAFTDEDICATED_API AWaterCup : public AFoodContainer
{
	GENERATED_BODY()

public:
    AWaterCup();
    
protected:
    
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AFood>      m_SaltWaterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AFood>      m_FreshWaterClass;

    bool                m_bInteractionWater;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent*           m_WaterStaticMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UAudioComponent*        m_AudioComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    USoundBase*             m_DrinkSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    USoundBase*             m_WaterCupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UMaterialInterface*				m_EmptyWaterMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UMaterialInterface*				m_SaltWaterMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UMaterialInterface*				m_FreshWaterMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EWaterType                      m_WaterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FFoodInfo	                m_SaltWaterInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FFoodInfo	                m_FreshWaterInfo;
    
protected:
    virtual void BeginPlay() override;
    
public:
    virtual void Tick(float DeltaTime) override;

    virtual bool OnEquip(APlayerCharacter* EquipOwner) override;
    virtual void UnEquip() override;

    virtual void Use() override;

    virtual void LButtonAction(bool bPress) override;
    
    virtual void Key_E_Action() override;
    virtual void RButtonAction(bool bPress) override;

public:
    bool IsEmptyWater() { return EWaterType::Empty == m_WaterType; }
    bool IsSaltWater() { return EWaterType::SaltWater == m_WaterType; }
    bool IsFreshWater() { return EWaterType::FreshWater == m_WaterType; }

public:
    void ChangeWater(EWaterType Type, bool ChangeIcon = true);
    
private:
    bool IsRaycastToOcean();
    void InteractionToOcean();
    
};
