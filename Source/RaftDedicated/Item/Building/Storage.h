// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "Storage.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AStorage : public ABuilding
{
	GENERATED_BODY()

public:
	AStorage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*>		m_JointComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*>	m_StorageDoorMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_StorageAccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_StorageMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float>			m_AngleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_Open;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_StorageAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_ItemSlotMaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FString>			m_ArrayItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<int32>			m_ArrayItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent*		m_AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_OpenSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase*				m_CloseSound;
	
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
	
protected:
	virtual void StorageAction(float DeltaTime);

protected:
	virtual void SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;

public:
	int32 GetItemSlotMaxCount() {return m_ItemSlotMaxCount; }

public:
	TArray<FString>& GetArrayItemName() { if(m_ArrayItemName.Num() == 0) { m_ArrayItemName.Init(TEXT(""), m_ItemSlotMaxCount);} return m_ArrayItemName;}
	TArray<int32>& GetArrayItemCount() { if(m_ArrayItemCount.Num() == 0) { m_ArrayItemCount.Init(0, m_ItemSlotMaxCount);} return m_ArrayItemCount;}
};
