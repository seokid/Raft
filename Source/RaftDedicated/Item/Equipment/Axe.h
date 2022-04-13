// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment.h"
#include "../../Component/DataTableAudioComponent.h"
#include "Axe.generated.h"


UENUM(BlueprintType)
enum class EAxeType : uint8
{
    Idle,
    Hit,
};

class ACutFoliage;

UCLASS()
class RAFTDEDICATED_API AAxe : public AEquipment
{
	GENERATED_BODY()
    
public:
    AAxe();

protected:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UDataTableAudioComponent*        m_AudioComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    ACutFoliage*			m_CutFoliage;

    EAxeType                m_Type;
    

protected:
    virtual void BeginPlay() override;
public:
    virtual void Tick(float DeltaTime) override;
    
    virtual void Use() override;
    virtual void UseEnd() override;
    virtual void LButtonAction(bool bPress) override;
    
};
