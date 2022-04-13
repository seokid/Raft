// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../Item.h"
#include "BlockFoundation.h"
#include "Building.h"
#include "../../RaftDedicatedGameInstance.h"
#include "Boat.generated.h"

USTRUCT(BlueprintType)
struct FArrayBuilding
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ABuilding*>	ArrayBuilding;

	FArrayBuilding()
	{
		
	}
	
	FArrayBuilding(int32 _Num)
	{
		ArrayBuilding.Init(nullptr, _Num);
	}

	void AppendFrontBackTArray(int32 _Num)
	{
		TArray<ABuilding*> ArrayFront;
		ArrayFront.Init(nullptr, _Num);

		TArray<ABuilding*> ArrayBack;
		ArrayBack.Init(nullptr, _Num);

		ArrayBuilding.Append(ArrayBack);
		ArrayFront.Append(ArrayBuilding);
		

		ArrayBuilding = ArrayFront;
	}
};

// new
USTRUCT(BlueprintType)
struct FMultiBuilding
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FArrayBuilding>	 MultiBuilding;

	FMultiBuilding()
	{
		
	}
	
	FMultiBuilding(int32 _Num)
	{
		MultiBuilding.Init(FArrayBuilding(_Num), _Num);
	}

	ABuilding* GetBuilding(int32 IdxX, int32 IdxY)
	{
		if(MultiBuilding.Num() <= IdxX || MultiBuilding[IdxX].ArrayBuilding.Num() <= IdxY) {return nullptr;}
		if(IdxX < 0 || IdxY < 0) {return nullptr;}
		return MultiBuilding[IdxX].ArrayBuilding[IdxY];
	}
	
	void AppendToCenter()
	{
		TArray<FArrayBuilding> MultiUp;
		MultiUp.Init(FArrayBuilding(MultiBuilding.Num() * 3), MultiBuilding.Num());

		TArray<FArrayBuilding> MultiDown;
		MultiDown.Init(FArrayBuilding(MultiBuilding.Num() * 3), MultiBuilding.Num());

		for(int32 i = 0; i < MultiBuilding.Num(); ++i)
		{
			MultiBuilding[i].AppendFrontBackTArray(MultiBuilding.Num());
		}

		MultiBuilding.Append(MultiDown);
		MultiUp.Append(MultiBuilding);
		MultiBuilding = MultiUp;
	}

	void PushBuilding(ABuilding* NewBuilding, int32 IdxX, int32 IdxY)
	{
		if(MultiBuilding.Num() <= IdxX || MultiBuilding[IdxX].ArrayBuilding.Num() <= IdxY) {return;}
		if(IdxX < 0 || IdxY < 0) {return;}
		MultiBuilding[IdxX].ArrayBuilding[IdxY] = NewBuilding;
	}
};

UCLASS()
class RAFTDEDICATED_API ABoat : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoat();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*				m_BoatRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*>		m_ArrayFloorSceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool							m_BuildEnable;

	class APlayerCharacter*							m_BuildActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ABuilding*>		m_ArrayBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ABuilding*>		m_ArrayBuildingAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ABuilding*>		m_ArrayBuildingGreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FMultiBuilding>	m_ArrayMultiBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_MultiBuildingSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					m_MultiBuildingRootIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ABuilding*				m_BuildingInstall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ABuilding*>		m_ArrayBuildingInstall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* m_Movement;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	ABuilding* GetBuilding(int32 Layer, int32 IdxX, int32 IdxY)
	{
		if(Layer >= m_ArrayMultiBuilding.Num()) {return nullptr;}
		return m_ArrayMultiBuilding[Layer].GetBuilding(IdxX + m_MultiBuildingRootIdx, IdxY + m_MultiBuildingRootIdx);
	}

	// Player 에서 클릭시 호출 함수
public:
	bool SelectBuildFloor(ABuilding* SelectBuilding, class APlayerCharacter* OwnerActor);
	bool SelectBreakFloor(ABuilding* SelectBuilding, class APlayerCharacter* OwnerActor);
	void BuildEnable(bool Enable, class APlayerCharacter* OwnerActor = nullptr);

	bool SelectBuildInstall(class APlayerCharacter* OwnerActor);
	bool SelectBreakInstall(ABuilding* SelectBuilding,class APlayerCharacter* OwnerActor);

	// 층Layer 관련
public:
	void AddBuildingLayer(int32 NewLayer);
	void AddBuildingLayer(int32 NewLayer, int32 IdxX, int32 IdxY);
	void BreakInstallLayer(int32 NewLayer, int32 IdxX, int32 IdxY);
	
private:
	void AddBuilding(ABuilding* Building, int32 IdxX, int32 IdxY);// 이중 배열에 블럭 추가하기(배열 확장 여부 확인 위해)
	bool CheckBuildingExist(ABuilding* Building, int32 IdxX, int32 IdxY);// 현재 위치에 건설 할수 있는지 여부(배열 범위 초과, nullptr 이면 건설 가능)
	bool CheckBuildingBreak(ABuilding* Building, int32 IdxX, int32 IdxY);// 현재 위치에 블럭(Alpha) 삭제 가능 여부(Alpha Block 이면서 이웃 블럭 중 Build가 하나라도 없다면 가능)

	
private:
	void ShowBuildingGreen();
	void ShowBuildGreenBottomFloor();
	void ShowBuildGreenInstall();
};





