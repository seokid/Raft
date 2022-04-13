// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item.h"
#include "Floatage.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FFloatageItemList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString		Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32		Count;
};


class AFloatageFactory;

UCLASS()
class RAFTDEDICATED_API AFloatage : public AItem
{
	GENERATED_BODY()

public:
	AFloatage();
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AFloatageFactory*		m_Factory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*		m_Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBuoyancyComponent*	m_Buoyancy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FItemDropInfo>		m_ArrayItemDropInfo;
	
public:
	UStaticMeshComponent* GetMesh() const { return m_Mesh; }
	void SetFactory(AFloatageFactory* Factory) { m_Factory = Factory; }
	void SetActive(bool bActive);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void TakeItem(APlayerCharacter* Player);
	
private:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
public:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
};
