// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class APlayerCharacter;

UCLASS()
class RAFTDEDICATED_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UShapeComponent*>	m_ArrayInteractionComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public: // 플레이어와 상호 작용 (플레이어의 E 키)
	virtual void OnInteraction(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent);

protected:
	virtual void OnInteractionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) { m_Interaction = true; }	
	virtual void OnInteractionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) { m_Interaction = false; }

public: // 플레이어의 레이와 충돌
	virtual void OnCollisionEnter(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) {};
	virtual void OnCollision(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) {};
	virtual void OnCollisionExit(class APlayerCharacter* Player, UPrimitiveComponent* HitComponent) {};

protected:
	virtual void SetItemInteractionUIText(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) {};

protected:
	void AddItemToPlayer(APlayerCharacter* Player, const TArray<FItemDropInfo>& ArrayItemDropInfo);
};
