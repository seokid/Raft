// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../Item.h"
#include "Building.generated.h"


UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	None,
	Build,
	Break,
};

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API ABuilding : public AItem
{
	GENERATED_BODY()

public:
	ABuilding();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMesh*			m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*		m_RootComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*	m_MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*				m_SourceMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*				m_GreenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*				m_RedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*				m_AlphaMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBuildingInfo			m_BuildingInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBuildingState					m_BuildingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float						m_AccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float						m_MaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float						m_EffectBoundary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool						m_EffectEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float						m_EffectSpeed;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	const FBuildingInfo& GetBuildingInfo() {return m_BuildingInfo;}
	bool IsBuildTypeFloor() {return m_BuildingInfo.m_BuildType == EBuildType::Floor;}
	bool IsBuildTypeInstall() {return m_BuildingInfo.m_BuildType == EBuildType::Install;}
	void SetBuildingName(const FString& Name) {m_BuildingInfo.m_Name = Name;}
	void SetBuildType(const EBuildType& NewType) {m_BuildingInfo.m_BuildType = NewType;}
	void SetIdx(int32 IdxX, int32 IdxY) {m_BuildingInfo.m_IdxX = IdxX; m_BuildingInfo.m_IdxY = IdxY;}
	void SetLayer(int32 Layer) {m_BuildingInfo.m_Layer = Layer;}
	UStaticMesh* GetBuildingMesh() {return m_Mesh;}
	UStaticMeshComponent* GetBuildingMeshComponent() {return m_MeshComponent;}
	void SetBuildingState(EBuildingState State) {m_BuildingState = State;}
	
public:
	void SetBuildingInfo(const FBuildingInfo& Info);
	virtual void SetBuildingStateType(const EBuildingStateType& StateType);
	void SetBuildingMesh(UStaticMesh* Mesh);

public: // BuildingInstall 관련
	ABuilding* RaycastToFloor();
	float GetBuildingInstallYawToFloor(float LookAtYaw);
	void GetNewIdxToLayer(int32& OutputIdxX, int32& OutputIdxY, int32 AddLayer);
	bool IsFloorExist();

public:
	virtual void OnInteraction(APlayerCharacter* Player, UPrimitiveComponent* HitComponent) override;
	
	
public: // Building Common
	virtual bool IsBuildPossible(); // 부모 함수에서는 충돌 검사 빌딩 자체에서 MxN 등 자체 확인 함수
	virtual bool IsBuildPossible(class APlayerCharacter* BuildOwnerActor); // Boat에서 호출 함수 
	
	virtual void BuildCompleted() { m_BuildingState = EBuildingState::Build; }
	virtual void BreakCompleted() { m_BuildingState = EBuildingState::Break; }

protected: // Effect
	virtual void SetBurnEffectEnable(bool Enable, float EffectSpeed = 1.f);
	virtual void ExecuteBurnEffect(float DeltaTime);
	virtual void EffectFinished();
};




