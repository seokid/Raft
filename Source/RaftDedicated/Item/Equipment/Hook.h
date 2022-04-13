// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Equipment.h"
#include "../../Component/DataTableAudioComponent.h"
#include "Hook.generated.h"


UENUM(BlueprintType)
enum class EHookType : uint8
{
	Idle,
	Load,
	Throw,
	Pull,
	Gather,
};

class AScrapFoliage;

UCLASS()
class RAFTDEDICATED_API AHook : public AEquipment
{
	GENERATED_BODY()

public:
	AHook();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ARope*			m_Rope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBuoyancyComponent*	m_Buoyancy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTableAudioComponent*        m_AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector					m_OriginPos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_ThrowPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    	float				m_ThrowCharingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_ScratchPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName					m_TailSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class AFloatage*>	m_TakeFloatage;

	EHookType				m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AScrapFoliage*			m_ScrapFoliage;


protected:

	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual bool OnEquip(APlayerCharacter* EquipOwner) override;
	virtual void UnEquip() override;
	
	virtual void Use() override;
	virtual void LButtonAction(bool bPress) override;
	virtual void RButtonAction(bool bPress) override;
	virtual void FullCharging() override;
	

private:
	void Throw();
	void ScratchStart();
	void ScratchFinish();
	void Withdraw();

	void GatherStart();
	void GatherFinish();



public:
	UFUNCTION()
	virtual void HookBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void HookEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
