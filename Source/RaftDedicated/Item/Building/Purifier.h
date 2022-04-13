// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "Purifier.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPurifierInteractionComponentType : uint8
{
	Firewood,
	SaltWater,
	FreshWater,
};


UCLASS()
class RAFTDEDICATED_API APurifier : public ABuilding
{
	GENERATED_BODY()

public:
	APurifier();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*>	m_ArrayFirewood;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_FreshWater; // 담수가 있는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_SaltWater; // 소금물이 있는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_WaterAccTime; // 물 정수 진행 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_WaterMaxTime; // 물 정수 완료 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodAccTime; // 장작 진행 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodMaxTime; // 장작 완료 시간
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodCount; // 장작 현재 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodMaxCount; // 장작 최대 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*			m_SaltWaterMeshComponent; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*			m_FreshWaterMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent*		m_ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent*				m_AudioComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	virtual void SetBuildingStateType(const EBuildingStateType& StateType) override;
	virtual bool IsBuildPossible(class APlayerCharacter* BuildOwnerActor) override; // Boat에서 호출 함수
	virtual bool IsBuildPossible() override;
	virtual void BuildCompleted() override;
	virtual void BreakCompleted() override;

private:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

public:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollision(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

protected:
	virtual void SetBurnEffectEnable(bool Enable, float EffectSpeed = 1.f) override;
	virtual void ExecuteBurnEffect(float DeltaTime) override;
	virtual void EffectFinished() override;

	virtual void SetOutLineEffectEnable(bool Enable);
	
private:
	void SaltWaterToFreshWaterAction(float DeltaTime);
	bool IsFirewoodComponent(UPrimitiveComponent* HitComponent) { return nullptr != HitComponent && m_ArrayInteractionComponent[static_cast<int32>(EPurifierInteractionComponentType::Firewood)]->GetName() == HitComponent->GetName(); }
	bool IsSaltWaterComponent(UPrimitiveComponent* HitComponent) { return nullptr != HitComponent && m_ArrayInteractionComponent[static_cast<int32>(EPurifierInteractionComponentType::SaltWater)]->GetName() == HitComponent->GetName(); }
	bool IsFreshWaterComponent(UPrimitiveComponent* HitComponent) { return nullptr != HitComponent && m_ArrayInteractionComponent[static_cast<int32>(EPurifierInteractionComponentType::FreshWater)]->GetName() == HitComponent->GetName(); }
	
protected:
	virtual void SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
};

