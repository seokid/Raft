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
	UMaterialInterface*				m_FirewoodMaterial; // ���� Material

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_FoodEnable; // ���� �ִ��� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_FoodFinish; // ���� ���� �Ϸ� ����
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FoodAccTime; // ���� ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FoodMaxTime; // ���� ���� �Ϸ� �ð�
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodAccTime; // ���� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float							m_FirewoodMaxTime; // ���� �Ϸ� �ð�
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodCount; // ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32							m_FirewoodMaxCount; // ���� �ִ� ����

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
	virtual bool IsBuildPossible(class APlayerCharacter* BuildOwnerActor) override; // Boat���� ȣ�� �Լ�
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



