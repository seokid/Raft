// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item.h"
#include "Foliage.generated.h"



UCLASS()
class RAFTDEDICATED_API AFoliage : public AItem
{
	GENERATED_BODY()
	
public:
	AFoliage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*			m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EFeedbackIconType			m_FeedbackIconType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FItemDropInfo>		m_ArrayItemDropInfo;

	EFeedbackIconType			m_IconType;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Gather(class APlayerCharacter* Player);

protected:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
public:
	virtual void OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	virtual void OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
};
