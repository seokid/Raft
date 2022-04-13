// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Equipment.h"
#include "Food.generated.h"

/**
 * 
 */



USTRUCT(BlueprintType)
struct FFoodInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	Hungry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	Thirsty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EFoodTypeInfo	Type;
};



UCLASS()
class RAFTDEDICATED_API AFood : public AEquipment
{
	GENERATED_BODY()

public:
	AFood();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*		m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent*        m_AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FFoodInfo	m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString		m_FoodName;
	
protected:
    virtual void BeginPlay() override;
	
public:
    virtual void Tick(float DeltaTime) override;
	
	virtual void Use() override;
	virtual void LButtonAction(bool bPress) override;


private:
	virtual void Eat();

public:
	UStaticMeshComponent* GetFoodStaticMeshComponent() {return Cast<UStaticMeshComponent>(m_Mesh);}
	FString GetFoodName() {return m_FoodName;}
	bool IsRawFood() {return EFoodTypeInfo::Raw == m_Info.Type;}
	bool IsCookedFood() {return EFoodTypeInfo::Cooked == m_Info.Type;}
};


