// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RaftDedicatedGameInstance.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "RaftDedicatedCharacter.generated.h"

UCLASS(config=Game)
class ARaftDedicatedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARaftDedicatedCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool	m_BuildEnable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	m_BuildingName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	n_BuildingRotationIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBuildType	m_BuildType;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	const FBuildingTableInfo* 		m_NewbuildingClassInfo;

	int32	m_BuldingInfoIdx;

	class AItem*			m_InteractionItem;
	UPrimitiveComponent*	m_ItemHitComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainHUD>		m_MainHUDClass;
	
	UMainHUD*						m_MainHUDWidget;

	float							m_RaycastDist;
	
public:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Build();
	void BreakBlock();
	void BuildEnable();
	void ChangeBuildInfo();
	void BuildingRotation();
	void Interaction();

	void InteractionRaycast();

	void ItemSlotLeft();
	void ItemSlotRight();

	void ShowInventory();
public:
	int32 GetBuildingRotationIdx() {return n_BuildingRotationIdx;}
	EBuildType GetBuildType() {return m_BuildType;}
	const FBuildingTableInfo& GetNewBuildingClassInfo() {return *m_NewbuildingClassInfo;}

	bool RaycastFromCamera(struct FHitResult& OutHit);

	void SetItemInteractionUIText(bool SlateVisibilty, const FString& InteractionButtonText = "", const FString& ItemDescText = "");
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	public:
	UCameraComponent* GetFollowCamera() {return FollowCamera;}
};

