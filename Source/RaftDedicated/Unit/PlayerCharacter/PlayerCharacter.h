// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Unit.h"
#include "PlayerCharacterAnim.h"
#include "../../Item/Equipment/Equipment.h"
#include "../../RaftDedicatedGameInstance.h"
#include "PlayerCharacter.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	HPMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	Hungry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HungryMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HungrySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HungryAccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	Thirsty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	ThirstyMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	ThirstySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	ThirstyAccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool	bBreathAble;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	Breath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	BreathMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	BreathSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	BreathAccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HookDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	HookAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	AttackSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	SwimSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	FastSwimSpeed;

};




USTRUCT(BlueprintType)
struct FEquipmentInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage*	Montage;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEquipment>	Equipment;

};





UCLASS()
class RAFTDEDICATED_API APlayerCharacter : public AUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:

	//Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent*			m_Arm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent*				m_Camera;

	
	//Breath Head
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent*				m_Head;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName							m_NoseBoneName;
	
	//PlayerInfo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString							m_PlayerInfoName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo						m_PlayerInfo;

	uint8							m_bBreathRecovery : 1;
	uint8							m_bBreathLow : 1;
	float							m_BreathLowPercent;
	uint8							m_bHungry : 1;
	float							m_HungryPercent;
	uint8							m_bThirsty : 1;
	float							m_ThirstyPercent;
	
	//Anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPlayerCharacterAnim*			m_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage*					m_GrabMontage;
	
	//Equipment
	FEquipmentInfo					m_EquipmentInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AEquipment*				m_Equipment;

	// MainHUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainHUD>		m_MainHUDClass;
	
	UPROPERTY()
	UMainHUD*						m_MainHUDWidget;


	//PostEffect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection*	m_PostEffectCollection;
	
	uint8							m_bBMRScreen : 1;

	// Item Interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AItem*			m_InteractionItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent*	m_ItemHitComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor*					m_InteractionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float					m_RaycastDist;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector					m_RaycastOffset;

	// Build
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool					m_BuildEnable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString						m_BuildingName;
	
	const FBuildingTableInfo* 		m_NewBuildingInfo;

	// Respone
	float					m_RespawnTime;
	float					m_RespawnAccTime;
	
public:
	void SetBloodScreen(float Ratio);
	void SetBMRScreen(float Ratio);
	const FVector& GetRaycastOffset() const { return m_RaycastOffset; }
	UAnimMontage* GetGrabMontage() const { return m_GrabMontage; }
	AActor* GetInteractionActor() const { return m_InteractionActor; }
	USpringArmComponent* GetSpringArm() const { return m_Arm; }
	UCameraComponent* GetCamera() const { return m_Camera; }
	UPlayerCharacterAnim* GetAnim() const {return m_AnimInstance;}
	const FPlayerInfo& GetPlayerInfo() const {return m_PlayerInfo;}
	const FEquipmentInfo& GetEquipInfo() const {return m_EquipmentInfo;}
	class AEquipment* GetEquipment() {return m_Equipment;}
	bool GetBuildEnable() {return m_BuildEnable;}
	
	const FBuildingTableInfo& GetNewBuildingClassInfo() {return *m_NewBuildingInfo;}
	void SetBuildName(const FString& Name) { SetBuildInfo(Name); }
	void EatFood(int32 Hungry, int32 Thirsty);

	UMainHUD* GetMainHUD() { return m_MainHUDWidget; }

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	virtual void UnPossessed() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	//기초대사량(BMR : Basal Metabolic Rate) : 배고픔, 목마름, 숨 관리
	void BMR(float DeltaTime);
	
	
public:
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void SwimmingUp(float Value);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Run(bool bRun);
	
	virtual void CameraZoom(float Value);
	
	virtual void Jump() override;
	virtual void Crouch(bool bCrouch) override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void LButtonAction(bool bPress);
	virtual void RButtonAction(bool bPress);

	virtual void Key_R_Action();
	virtual void Key_T_Action();
	
	virtual void UseEquipment();
	virtual void UseEndEquipment();

	virtual void GrabEnd();

	virtual void Swim();
	
	virtual void BeginChargingEquipment();
	virtual void EndChargingEquipment();


public:
	UFUNCTION(NetMulticast, Reliable)
	void OnEquipmentItem(const FString& Name);
	
	void UnEquipmentItem();

protected:
	void InteractionRaycast();

	// Controller -> Player
public:
	void Interaction();
	void ItemSlotLeft();
	void ItemSlotRight();
	void ShowMainHUD();

	// Item -> Player
public:
	void SetItemInteractionUIText(bool SlateVisibilty, const FString& InteractionButtonText = "", const FString& ItemDescText = "");
	bool Building_Build();
	bool Building_Break();

	// Build 
public:
	void SetBuildInfo(const FString& Name);
	bool RaycastFromCamera(struct FHitResult& OutHit);
	FVector GetRaycastLocation();
	int32 GetBuildingRotationIdx();
	void SetBuildEnable(bool Enable);
	void BuildCompleted(const FString& ItemName);

public:
	UFUNCTION()
    virtual void HeadBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
    , UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void HeadEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
    	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void WaterIn();
	void WaterOut();
	void BreathLow();
	void Drowning();
	void BreathRecoveryStart();
	void BreathRecoveryEnd();
	void Thirsty();
	void ThirstyRecovery();
	void Hungry();
	void HungryRecovery();
	void Respawn();
	void Death();
};
