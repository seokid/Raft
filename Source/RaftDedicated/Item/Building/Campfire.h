// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "RaftDedicated/Item/Equipment/Food.h"
#include "Campfire.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ACampfire : public ABuilding
{
	GENERATED_BODY()
	
public:
	ACampfire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*>	m_ArrayFirewood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*				m_FirewoodMaterial; // 장작 Material

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_FoodEnable; // 음식 있는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_FoodFinish; // 음식 조리 완료 여부
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FoodAccTime; // 음식 조리 진행 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FoodMaxTime; // 음식 조리 완료 시간
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodAccTime; // 장작 진행 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodMaxTime; // 장작 완료 시간
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodCount; // 장작 현재 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodMaxCount; // 장작 최대 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*				m_FoodLocationComponent; // Food Location Scene Component

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString							m_FoodName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AFood*							m_FoodActor;

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

protected:
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
	void CookingAction(float DeltaTime);
	bool IsFirewoodComponent(UPrimitiveComponent* HitComponent) {return nullptr != HitComponent && m_ArrayInteractionComponent[0]->GetName() == HitComponent->GetName();}
	bool IsFoodComponent(UPrimitiveComponent* HitComponent) {return nullptr != HitComponent && m_ArrayInteractionComponent[1]->GetName() == HitComponent->GetName();}

	void FoodCooked();

	void SetFoodActor(const FString& FoodName);
	
protected:
	virtual void SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
};



