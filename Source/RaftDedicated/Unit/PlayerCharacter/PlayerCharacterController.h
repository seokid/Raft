// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

enum class EKeyInputType : int32
{
	LButton = 0,
	RButton,
	Crouch,
	Run,
	End,
};

UCLASS()
class RAFTDEDICATED_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<bool>	m_ArrKeyPress;
	
	float			m_Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString			m_EquipName;
	
		
public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void SwimmingUp(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void CameraZoom(float Value);

	
	void RButton();
	void Jump();

	UFUNCTION(Server, Reliable)
	void LButtonPress();

	UFUNCTION(Server, Reliable)
	void LButtonRelease();
	
	
	UFUNCTION(Server, Reliable)
	void Run();
	
	void Crouch();

	UFUNCTION(Server, Reliable)
	void UseItem1();
	void UseItem2();

	
	void KeyDown(EKeyInputType KeyType);

	void Interaction();
	void Key_R();
	void Key_T();
	
	void ItemSlotLeft();
	void ItemSlotRight();

	void ShowMainHUD();

	void Build();
	
public:
	UFUNCTION()
virtual void PlayerControllerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void PlayerControllerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};




