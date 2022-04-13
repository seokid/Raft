// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"


#include "Building.h"
#include "BlockFoundation.h"
#include "../../RaftDedicatedGameInstance.h"
#include "../../RaftDedicatedCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
//#include "../../RaftDedicatedGameMode.h"

// Sets default values
ABoat::ABoat()
{
	// RootComponent ����
	m_BoatRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Boat"));
	SetRootComponent(m_BoatRootComponent);
	
	// Editor Ȯ�ο� StaticMeshComponent ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockAsset(TEXT("StaticMesh'/Game/Item/Building/Block_Foundation/Block_Foundation.Block_Foundation'"));
	if(BlockAsset.Succeeded())
	{
		UStaticMesh* BlockMesh = BlockAsset.Object;
		UStaticMeshComponent* BlockMeshComponent = nullptr;
	
		for(int32 i = -1; i <= 1; ++i)
		{
			for(int32 j = -1; j <= 1; ++j)
			{
				BlockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("BlockFoundation_(%2d %2d)"),i, j));
				BlockMeshComponent->SetStaticMesh(BlockMesh);
				BlockMeshComponent->SetRelativeTransform(FTransform(FVector(150.f * i, 150.f * j, 0.f)));
				BlockMeshComponent->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
				BlockMeshComponent->SetupAttachment(m_BoatRootComponent);
			}
		}
	}

	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	m_Movement->SetUpdatedComponent(m_BoatRootComponent);
	m_Movement->ProjectileGravityScale = 0.f;
	m_Movement->InitialSpeed = 0.f;
	
	const FVector WaveDir(1.f, 0.f, 0.f);
	m_Movement->Velocity = WaveDir;

	m_BuildEnable = false;
	
	m_MultiBuildingSize = 7;
	m_MultiBuildingRootIdx = m_MultiBuildingSize / 2;

	m_BuildingInstall = nullptr;

	m_ArrayMultiBuilding.Empty();
	m_ArrayMultiBuilding.Init(FMultiBuilding(m_MultiBuildingSize), 1);
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();
	
	// Editor Ȯ�ο� StaticMeshComponent ����
	TArray<USceneComponent*> ArrayChildComponent = RootComponent->GetAttachChildren();
	for(auto ChildComponent : ArrayChildComponent)
	{
		ChildComponent->DestroyComponent();
	}
	
	m_MultiBuildingRootIdx = m_MultiBuildingSize / 2;
	
	// ���� Blockfoundation Actor ����
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABlockFoundation* ChildBlockFoundationActor = nullptr;
	for(int32 i = -1; i <= 1; ++i)
	{
		for(int32 j = -1; j <= 1; ++j)
		{
			
			FBuildingInfo ChildBuildingInfo;
			ChildBuildingInfo.m_IdxX = i;
			ChildBuildingInfo.m_IdxY = j;
			ChildBuildingInfo.m_StateType = EBuildingStateType::Build;
			ChildBuildingInfo.m_BuildType = EBuildType::Bottom;
			
			ChildBlockFoundationActor = GetWorld()->SpawnActor<ABlockFoundation>(ABlockFoundation::StaticClass(), GetActorLocation(), GetActorRotation(), param);
			ChildBlockFoundationActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			ChildBlockFoundationActor->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(150.f * i, 150.f * j, 0.f), FVector::OneVector));
			//ChildBlockFoundationActor->SetBuildingInfo(ChildBuildingInfo);
			ChildBlockFoundationActor->SetIdx(i, j);
			ChildBlockFoundationActor->SetBuildingStateType(EBuildingStateType::Build);
			ChildBlockFoundationActor->SetBuildType(EBuildType::Bottom);
			ChildBlockFoundationActor->SetBuildingState(EBuildingState::Build);
			
			m_ArrayBuilding.Add(ChildBlockFoundationActor);
			AddBuilding(ChildBlockFoundationActor, ChildBuildingInfo.m_IdxX + m_MultiBuildingRootIdx, ChildBuildingInfo.m_IdxY + m_MultiBuildingRootIdx);
		}
	}
	
	ABuilding* ChildBuildingActor = nullptr;
	for(auto CurBuilding : m_ArrayBuilding)
	{
		const FBuildingInfo& CurInfo = CurBuilding->GetBuildingInfo();
		FBuildingInfo NewBuildingInfo;
		NewBuildingInfo.m_StateType = EBuildingStateType::Alpha;
		NewBuildingInfo.m_BuildType = EBuildType::Bottom;
		
		TArray<int32> ArrayIdxX = {0, 0, 1, -1};
		TArray<int32> ArrayIdxY = {1, -1, 0, 0};
	
		for(int32 i = 0; i < 4; ++i)
		{
			NewBuildingInfo.m_IdxX = CurInfo.m_IdxX + ArrayIdxX[i];
			NewBuildingInfo.m_IdxY = CurInfo.m_IdxY + ArrayIdxY[i];
	
			bool IsCreate = false;
			
			for(auto NextBuilding : m_ArrayBuilding)
			{
				if(NextBuilding->GetBuildingInfo().IsIdxSame(NewBuildingInfo))
				{
					IsCreate = true;
					break;
				}
			}
			
			if(!IsCreate)
			{
				ChildBuildingActor = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), GetActorLocation(), GetActorRotation(), param);
				ChildBuildingActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				ChildBuildingActor->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(150.f * NewBuildingInfo.m_IdxX, 150.f * NewBuildingInfo.m_IdxY, 0.f), FVector::OneVector ));
				ChildBuildingActor->SetBuildingInfo(NewBuildingInfo);
				m_ArrayBuildingAlpha.Add(ChildBuildingActor);
				
				AddBuilding(ChildBuildingActor, NewBuildingInfo.m_IdxX + m_MultiBuildingRootIdx, NewBuildingInfo.m_IdxY + m_MultiBuildingRootIdx);
			}
		}
	}
}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ShowBuildingGreen();

	const FVector WaveDir(1.f, 0.f, 0.f);
	
}

bool ABoat::SelectBuildFloor(ABuilding* SelectBuilding, APlayerCharacter* OwnerActor)
{
	// ��ġ ���� ���� Ȯ�� -> green ���� �������
	if(m_ArrayBuildingAlpha.Num() <= 0) {return false;}
	
	int32 Idx = m_ArrayBuildingAlpha.Find(SelectBuilding);
	if(INDEX_NONE == Idx) {return false;}
	
	const FBuildingInfo& NewInfo = SelectBuilding->GetBuildingInfo();
	if(NewInfo.m_BuildType != OwnerActor->GetNewBuildingClassInfo().BuildType) {return false;}

	TArray<AActor*> ArrayResult;
	SelectBuilding->GetBuildingMeshComponent()->GetOverlappingActors(ArrayResult);
	for(auto result : ArrayResult)
	{
		LOG(TEXT("%s"), *result->GetName());
	}
	
	// ���� ���� ������ ������ ���� ������ Ȯ���ؾ� �Ѵ�.
    
	// ���⼭ ���� �ν��Ͻ����� Ű������ ��������
	// URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	// if(nullptr == GameInst) {return;}
	// const FBuildingTableInfo* NewBuildingClassInfo = GameInst->FindBuildingInfo(BuildingName);
	// if(nullptr == NewBuildingClassInfo) {return;}

	// ��ġ ���� �ϴٸ� ��ġ���ɺ��� �迭���� ����
	m_ArrayBuildingAlpha.RemoveAt(Idx);

	// ���ο� ���� ����
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABuilding* NewBuilding = GetWorld()->SpawnActor<ABuilding>(OwnerActor->GetNewBuildingClassInfo().BuildingClass, SelectBuilding->GetRootComponent()->GetRelativeLocation(), SelectBuilding->GetRootComponent()->GetRelativeRotation(), param);
	NewBuilding->SetBuildingInfo(NewInfo);
	NewBuilding->SetBuildingName(OwnerActor->GetNewBuildingClassInfo().Name);
	NewBuilding->SetBuildingStateType(EBuildingStateType::Build);
	NewBuilding->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	// FRotator NewRotator = FRotator::ZeroRotator;
	// NewRotator.Yaw = 90.f * OwnerActor->GetBuildingRotationIdx();
	//NewBuilding->SetActorRelativeTransform(FTransform(NewRotator, FVector(150.f * NewInfo.m_IdxX, 150.f * NewInfo.m_IdxY, 0.f), FVector::OneVector));
	AddBuilding(NewBuilding, NewInfo.m_IdxX  + m_MultiBuildingRootIdx, NewInfo.m_IdxY + m_MultiBuildingRootIdx);
	m_ArrayBuilding.Add(NewBuilding);

	NewBuilding->BuildCompleted();

	if(NewBuilding->GetBuildingInfo().m_BuildType == EBuildType::Bottom)
	{
		// ���� ��ġ�� ���� �ֺ� ��ġ ���� ���� Ȯ�� �� ����
		TArray<int32> ArrayIdxX = {-1, 0, 1, 0};
		TArray<int32> ArrayIdxY = {0, -1, 0, 1};
		for(int32 i = 0; i < 4; ++i)
		{
			FBuildingInfo NextInfo;
			NextInfo.m_Layer = NewInfo.m_Layer;
			NextInfo.m_IdxX = NewInfo.m_IdxX + ArrayIdxX[i];
			NextInfo.m_IdxY = NewInfo.m_IdxY + ArrayIdxY[i];
			NextInfo.m_StateType = EBuildingStateType::Alpha;
			NextInfo.m_BuildType = NewInfo.m_BuildType;


			if(CheckBuildingExist(SelectBuilding, NextInfo.m_IdxX + m_MultiBuildingRootIdx, NextInfo.m_IdxY + m_MultiBuildingRootIdx))
			{
				ABuilding* ChildBuildingActor = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), GetActorLocation(), GetActorRotation(), param);
				ChildBuildingActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				ChildBuildingActor->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(150.f * NextInfo.m_IdxX, 150.f * NextInfo.m_IdxY, SelectBuilding->GetRootComponent()->GetRelativeLocation().Z),FVector::OneVector));
				ChildBuildingActor->SetBuildingInfo(NextInfo);
				AddBuilding(ChildBuildingActor, NextInfo.m_IdxX + m_MultiBuildingRootIdx, NextInfo.m_IdxY + m_MultiBuildingRootIdx);
				m_ArrayBuildingAlpha.Add(ChildBuildingActor);
			}
		}
	}
	
	// ���� ��ġ ���� ���� ����
	SelectBuilding->Destroy();

	return true;
}

bool ABoat::SelectBreakFloor(ABuilding* SelectBuilding, APlayerCharacter* OwnerActor)
{
	if(m_ArrayBuilding.Num() <= 0) {return false;}

	int32 Idx = m_ArrayBuilding.Find(SelectBuilding);
	if(INDEX_NONE == Idx) {return false;}
	const FBuildingInfo& NewInfo = SelectBuilding->GetBuildingInfo();
	
	// ���� ���� �ڸ��� ��ġ ���� ���� �����ϱ�
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABuilding* ChildNewBuilding = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), SelectBuilding->GetRootComponent()->GetRelativeTransform(), param);
	ChildNewBuilding->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	//ChildNewBuilding->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(150.f * NewInfo.m_IdxX, 150.f * NewInfo.m_IdxY, 0.f),FVector::OneVector));
	ChildNewBuilding->SetBuildingInfo(NewInfo);
	ChildNewBuilding->SetBuildingName(TEXT("Building"));
	ChildNewBuilding->SetBuildingMesh(SelectBuilding->GetBuildingMesh());
	ChildNewBuilding->SetBuildingStateType(EBuildingStateType::Alpha);
	AddBuilding(ChildNewBuilding, NewInfo.m_IdxX + m_MultiBuildingRootIdx, NewInfo.m_IdxY + m_MultiBuildingRootIdx);

	m_ArrayBuilding.RemoveAt(Idx);
	m_ArrayBuildingAlpha.Add(ChildNewBuilding);
	//m_ArrayBuildingGreen.Add(ChildNewBuilding);

	if(ChildNewBuilding->GetBuildingInfo().m_BuildType == EBuildType::Bottom)
	{
		// ���� ���� -> ��ġ ���� ���� �Ǿ����Ƿ� �ֺ� ��ġ ���� ������ �˻�
		TArray<int32> ArrayIdxX = {-1, 0, 1, 0};
		TArray<int32> ArrayIdxY = {0, -1, 0, 1};
		for(int32 i = 0; i < 4; ++i)
		{
			// ���� �ѽ� ���� �̿� �� -> �ε��� �˻� �ʿ� ���� -> �Ǽ����־��ٸ� ������ ���� Green Block ���� ������
			//ABuilding* NextBuilding = m_MultiBuilding[NewInfo.m_IdxY + m_MultiBuildingRootIdx + ArrayIdxY[i]].ArrayBuilding[NewInfo.m_IdxX + m_MultiBuildingRootIdx + ArrayIdxX[i]];
			ABuilding* NextBuilding = m_ArrayMultiBuilding[NewInfo.m_Layer].GetBuilding(NewInfo.m_IdxX + m_MultiBuildingRootIdx + ArrayIdxX[i], NewInfo.m_IdxY + m_MultiBuildingRootIdx + ArrayIdxY[i]);
			if(CheckBuildingBreak(NextBuilding, NewInfo.m_IdxX + ArrayIdxX[i] + m_MultiBuildingRootIdx, NewInfo.m_IdxY + ArrayIdxY[i] + m_MultiBuildingRootIdx))
			{
				m_ArrayMultiBuilding[NewInfo.m_Layer].PushBuilding(nullptr, NewInfo.m_IdxX + m_MultiBuildingRootIdx + ArrayIdxX[i], NewInfo.m_IdxY + m_MultiBuildingRootIdx + ArrayIdxY[i]);
				m_ArrayBuildingAlpha.Remove(NextBuilding);
				NextBuilding->Destroy();
			}	
		}
	}

	SelectBuilding->BreakCompleted();

	return true;
	//SelectBuilding->Destroy();
}

void ABoat::BuildEnable(bool Enable, APlayerCharacter* OwnerActor)
{
	m_BuildEnable = Enable;
	m_BuildActor = OwnerActor;

	for(auto Building : m_ArrayBuildingGreen)
	{
		FRotator BuildingRotator = Building->GetActorRotation();
		BuildingRotator.Yaw = 0.f;
		Building->SetActorRelativeRotation(BuildingRotator);
		Building->SetBuildingStateType(EBuildingStateType::Alpha);
	}

	m_ArrayBuildingGreen.Empty();

	if(m_BuildingInstall)
	{
		m_BuildingInstall->Destroy();
		m_BuildingInstall = nullptr;
	}
}

bool ABoat::SelectBuildInstall(APlayerCharacter* OwnerActor)
{
	// ��ġ ���� ���� ���� �Ѵ�. -> Green ����, ��ġ �ǹ� �ڽŸ��� ��ġ ���� ����
	if(nullptr == m_BuildingInstall) {return false;}
	TArray<AActor*> ArrayResult;
	m_BuildingInstall->GetBuildingMeshComponent()->GetOverlappingActors(ArrayResult);
	for(auto result : ArrayResult)
	{
		LOG(TEXT("%s"), *result->GetName());
	}
	
	if(m_BuildingInstall->GetBuildingInfo().m_StateType != EBuildingStateType::Green) { return false;	}
	// ABuilding* FloorBuilding = m_BuildingInstall->RaycastToFloor();
	// if(nullptr == FloorBuilding) {return;}
	
	
	m_ArrayBuildingInstall.Add(m_BuildingInstall);
	m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Build);
	
	// FRotator NewRotator = m_BuildingInstall->GetActorRotation();
	// NewRotator.Yaw -= FloorBuilding->GetActorRotation().Yaw;
	// m_BuildingInstall->SetActorRelativeRotation(NewRotator);
	// m_BuildingInstall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	// m_BuildingInstall->SetActorRelativeLocation(FloorBuilding->GetRootComponent()->GetRelativeLocation());
	
	FString Name = m_BuildingInstall->GetBuildingInfo().m_Name;
	//m_BuildingInstall->SetBuildingInfo(FloorBuilding->GetBuildingInfo());
	m_BuildingInstall->SetBuildType(EBuildType::Install);
	m_BuildingInstall->SetBuildingName(Name);

	// ��ġ ������ �߰����ΰ� �ִ��� Ȯ�� ����̸� 2�� ���� �� �� �ֵ��� ����� �Ѵ�.
	m_BuildingInstall->BuildCompleted();
	m_BuildingInstall = nullptr;

	// ĳ���Ϳ��� ��ġ �Ϸ� �˷���� �Ѵ�. -> ������ â, UI ���� ��ġ�Ѱ� ���� �ϱ� �����̴�.
	OwnerActor->BuildCompleted(Name);

	return true;
}

bool ABoat::SelectBreakInstall(ABuilding* SelectBuilding, APlayerCharacter* OwnerActor)
{
	int32 Idx = m_ArrayBuildingInstall.Find(SelectBuilding);
	if(INDEX_NONE == Idx) {return false;}

	m_ArrayBuildingInstall.RemoveAt(Idx);

	SelectBuilding->BreakCompleted();

	return true;
	//SelectBuilding->Destroy();
}

void ABoat::AddBuildingLayer(int32 NewLayer)
{
	int32 NewLayerCount = NewLayer - m_ArrayMultiBuilding.Num();
	for(int32 i = 0; i <= NewLayerCount; ++i)
	{
		m_ArrayMultiBuilding.Add(FMultiBuilding(m_MultiBuildingSize));
	}
}

void ABoat::AddBuildingLayer(int32 NewLayer, int32 IdxX, int32 IdxY)
{
	AddBuildingLayer(NewLayer);
	
	if(nullptr == m_ArrayMultiBuilding[NewLayer].GetBuilding(IdxX + m_MultiBuildingRootIdx, IdxY + m_MultiBuildingRootIdx))
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABuilding* ChildNewBuilding = GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), param);
		ChildNewBuilding->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		ChildNewBuilding->SetActorRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(150.f * IdxX, 150.f * IdxY,120.f * NewLayer ),FVector::OneVector));
		FBuildingInfo NewInfo;
		NewInfo.m_Layer = NewLayer;
		NewInfo.m_IdxX = IdxX;
		NewInfo.m_IdxY = IdxY;
		ChildNewBuilding->SetBuildingInfo(NewInfo);
		ChildNewBuilding->SetBuildingStateType(EBuildingStateType::Alpha);
		m_ArrayBuildingAlpha.Add(ChildNewBuilding);
		AddBuilding(ChildNewBuilding, NewInfo.m_IdxX + m_MultiBuildingRootIdx, NewInfo.m_IdxY + m_MultiBuildingRootIdx);
	}
}

void ABoat::BreakInstallLayer(int32 NewLayer, int32 IdxX, int32 IdxY)
{
	ABuilding* NextBuilding = m_ArrayMultiBuilding[NewLayer].GetBuilding(IdxX + m_MultiBuildingRootIdx, IdxY + m_MultiBuildingRootIdx);
	LOG(TEXT("%d %d"), IdxX, IdxY);
	if(CheckBuildingBreak(NextBuilding, IdxX + m_MultiBuildingRootIdx, IdxY + m_MultiBuildingRootIdx))
	{
		m_ArrayMultiBuilding[NewLayer].PushBuilding(nullptr, IdxX + m_MultiBuildingRootIdx, IdxY + m_MultiBuildingRootIdx);
		m_ArrayBuildingAlpha.Remove(NextBuilding);
		NextBuilding->Destroy();
	}	
}

void ABoat::AddBuilding(ABuilding* Building, int32 IdxX, int32 IdxY)
{
	int32 LayerIdx = Building->GetBuildingInfo().m_Layer;
	if(IdxY >= m_MultiBuildingSize - 1 ||
		IdxX >= m_MultiBuildingSize - 1 ||
		IdxY < 1 || IdxX < 1)
	{
		for(int32 i = 0; i < m_ArrayMultiBuilding.Num(); ++i)
		{
			m_ArrayMultiBuilding[i].AppendToCenter();
		}
	
		m_MultiBuildingSize = m_ArrayMultiBuilding[LayerIdx].MultiBuilding.Num(); 
		IdxX -= m_MultiBuildingRootIdx;
		IdxY -= m_MultiBuildingRootIdx;
		m_MultiBuildingRootIdx = m_MultiBuildingSize / 2;
		IdxX += m_MultiBuildingRootIdx;
		IdxY += m_MultiBuildingRootIdx;
	}

	m_ArrayMultiBuilding[LayerIdx].PushBuilding(Building, IdxX, IdxY);
}

bool ABoat::CheckBuildingExist(ABuilding* Building, int32 IdxX, int32 IdxY)
{
	int LayerIdx = Building->GetBuildingInfo().m_Layer;
	if(nullptr == m_ArrayMultiBuilding[LayerIdx].GetBuilding(IdxX, IdxY))
	{
		return true;
	}
	
	return false;
}

bool ABoat::CheckBuildingBreak(ABuilding* Building, int32 IdxX, int32 IdxY)
{
	const FBuildingInfo& Info =  Building->GetBuildingInfo();

	if(Info.m_StateType == EBuildingStateType::Build) { return false; }

	TArray<int32> ArrayIdxX = {-1, 0, 1, 0};
	TArray<int32> ArrayIdxY = {0, -1, 0, 1};

	for(int32 i = 0; i < 4; ++i)
	{
		// ���� �ѽ� ���� �̿� �� �� �ϳ��� Build �� �ִٸ� �ѽǼ� ����.
		ABuilding* NextBuilding = m_ArrayMultiBuilding[Info.m_Layer].GetBuilding(IdxX + ArrayIdxX[i], IdxY + ArrayIdxY[i]);
		if(NextBuilding && NextBuilding->GetBuildingInfo().m_StateType == EBuildingStateType::Build)
		{
			return false;
		}
	}
	return true;
}


void ABoat::ShowBuildingGreen()
{
	if(false == m_BuildEnable) {return;}
	if(nullptr == m_BuildActor) {return;}

	// BuildingGreen ��� ���ķ� ����� �ʱ�ȭ
	for(auto Building : m_ArrayBuildingGreen)
	{
		FRotator BuildingRotator = Building->GetActorRotation();
		BuildingRotator.Yaw = 0.f;
		Building->SetActorRelativeRotation(BuildingRotator);
		Building->SetBuildingStateType(EBuildingStateType::Alpha);
	}

	const FBuildingTableInfo& NewBuildingClassInfo = m_BuildActor->GetNewBuildingClassInfo();

	switch (NewBuildingClassInfo.BuildType)
	{
	case EBuildType::Bottom:
	case EBuildType::Floor:
		ShowBuildGreenBottomFloor();
		break;
	case EBuildType::Install:
		ShowBuildGreenInstall();
		break;
	}
	
	// if(NewBuildingClassInfo.BuildType == EBuildType::Bottom || NewBuildingClassInfo.BuildType == EBuildType::Floor)
	// {
	// 	ShowBuildGreenBottomFloor();
	// 	// if(m_BuildingInstall)
	// 	// {
	// 	// 	m_BuildingInstall->Destroy();
	// 	// 	m_BuildingInstall = nullptr;
	// 	// }
	// 	// for(auto Building : m_ArrayBuildingGreen)
	// 	// {
	// 	// 	FRotator BuildingRotator = Building->GetActorRotation();
	// 	// 	BuildingRotator.Yaw = 0.f;
	// 	// 	Building->SetActorRelativeRotation(BuildingRotator);
	// 	// 	Building->SetBuildingStateType(EBuildingStateType::Alpha);
	// 	// }
	// 	//
	// 	// m_ArrayBuildingGreen.Empty();
	// 	//
	// 	// FHitResult	result;
	// 	// FCollisionQueryParams params(NAME_None, false, this);
	// 	// bool Hit = GetWorld()->LineTraceSingleByChannel(result, m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f),
	// 	// 	m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f) + m_BuildActor->GetFollowCamera()->GetForwardVector() * 300.f, ECollisionChannel::ECC_GameTraceChannel2, params);
	// 	//
	// 	// if(Hit)
	// 	// {
	// 	// 	ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
	// 	// 	if(SelectBuilding && SelectBuilding->GetBuildingInfo().m_StateType == EBuildingStateType::Alpha)
	// 	// 	{
	// 	// 		//SelectBuilding->SetBuildingMesh(m_BuildSourceMesh);
	// 	// 		//SelectBuilding->SetBuildingMesh(m_NewBuildingClassInfo.SourceMesh);
	// 	// 		SelectBuilding->SetBuildingMesh(NewBuildingClassInfo.SourceMesh);
	// 	// 		SelectBuilding->SetBuildingStateType(EBuildingStateType::Green);
	// 	// 		FRotator BuildingRotator = SelectBuilding->GetActorRotation();
	// 	// 		//BuildingRotator.Yaw += 90.f * m_BuildActor->GetBuildingRotationIdx();
	// 	// 		BuildingRotator.Yaw = 90.f * m_BuildActor->GetBuildingRotationIdx();
	// 	// 		SelectBuilding->SetActorRelativeRotation(BuildingRotator);
	// 	// 		m_ArrayBuildingGreen.Add(SelectBuilding);
	// 	// 	}
	// 	// }
	// }
	// else
	// {
	// 	for(auto Building : m_ArrayBuildingGreen)
	// 	{
	// 		FRotator BuildingRotator = Building->GetActorRotation();
	// 		BuildingRotator.Yaw = 0.f;
	// 		//Building->SetActorRotation(BuildingRotator);
	// 		Building->SetBuildingStateType(EBuildingStateType::Alpha);
	// 	}
	//
	// 	if(nullptr == m_BuildingInstall)
	// 	{
	// 		FActorSpawnParameters param;
	// 		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 		m_BuildingInstall = GetWorld()->SpawnActor<ABuilding>(m_BuildActor->GetNewBuildingClassInfo().BuildingClass, GetActorLocation(), GetActorRotation(), param);
	// 		m_BuildingInstall->SetActorLocation(m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f) + m_BuildActor->GetFollowCamera()->GetForwardVector() * 300.f);
	// 		m_BuildingInstall->SetBuildingName(m_BuildActor->GetNewBuildingClassInfo().Name);
	// 		m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Red);
	// 		FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(m_BuildingInstall->GetActorLocation(), m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
	// 		LookAtRoataor.Pitch = 0.f;
	// 		LookAtRoataor.Roll = 0.f;
	// 		//LookAtRoataor.Yaw += m_BuildingInstall->GetActorRotation().Yaw;
	// 		m_BuildingInstall->SetActorRotation(LookAtRoataor);
	// 	}
	// 	else
	// 	{
	// 		if(m_BuildingInstall->GetBuildingInfo().m_Name != m_BuildActor->GetNewBuildingClassInfo().Name)
	// 		{
	// 			m_BuildingInstall->Destroy();
	// 			m_BuildingInstall = nullptr;
	// 		}
	// 		else
	// 		{
	// 			m_BuildingInstall->SetActorLocation(m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f) + m_BuildActor->GetFollowCamera()->GetForwardVector() * 300.f);
	// 			FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(m_BuildingInstall->GetActorLocation(), m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
	// 			LookAtRoataor.Pitch = 0.f;
	// 			LookAtRoataor.Roll = 0.f;
	// 			ABuilding* FloorBuilding = m_BuildingInstall->RaycastToFloor();
	// 			
	// 			if(FloorBuilding)
	// 			{
	// 				//float NewYaw = FVector::DotProduct(FloorBuilding->GetActorForwardVector(), m_BuildingInstall->GetActorForwardVector());
	// 				
	// 				//LookAtRoataor.Yaw = FloorBuilding->GetActorRotation().Yaw + (m_BuildActor->GetBuildingRotationIdx() * 90.f);
	// 				//LookAtRoataor.Yaw = NewYaw;
	//
	// 				LookAtRoataor.Yaw = m_BuildingInstall->GetBuildingInstallYawToFloor(LookAtRoataor.Yaw);
	// 				LookAtRoataor.Yaw += FloorBuilding->GetActorRotation().Yaw;// + FloorBuilding->GetRootComponent()->GetRelativeRotation().Yaw;
	// 				LookAtRoataor.Yaw += 90.f * m_BuildActor->GetBuildingRotationIdx();
	// 				
	// 				m_BuildingInstall->SetActorLocation(FloorBuilding->GetActorLocation());
	// 				m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Green);
	// 			}
	// 			else
	// 			{
	// 				//LookAtRoataor.Yaw += m_BuildingInstall->GetActorRotation().Yaw;
	// 				LookAtRoataor.Yaw += 90.f * m_BuildActor->GetBuildingRotationIdx();
	// 				m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Red);
	// 			}
	// 			m_BuildingInstall->SetActorRotation(LookAtRoataor);
	// 		}
	// 	}
	// }
	
}

void ABoat::ShowBuildGreenBottomFloor()
{
	const FBuildingTableInfo& NewBuildingClassInfo = m_BuildActor->GetNewBuildingClassInfo();

	// BuldingInstall�� �־��ٸ� ���� �Ѵ�.
	if(m_BuildingInstall)
	{
		m_BuildingInstall->Destroy();
		m_BuildingInstall = nullptr;
	}

	// // BuildingGreen ��� ���ķ� ����� �ʱ�ȭ
	// for(auto Building : m_ArrayBuildingGreen)
	// {
	// 	FRotator BuildingRotator = Building->GetActorRotation();
	// 	BuildingRotator.Yaw = 0.f;
	// 	Building->SetActorRelativeRotation(BuildingRotator);
	// 	Building->SetBuildingStateType(EBuildingStateType::Alpha);
	// }

	m_ArrayBuildingGreen.Empty();

	// �÷��̾� ���� ����ĳ��Ʈ ������ ���õ� �� �����´�.
	FHitResult	result;
	
	bool Hit = m_BuildActor->RaycastFromCamera(result);
	
	// Raycast �浹�� ���ٸ� ����
	if(false == Hit) {return;}


	ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
	if(nullptr == SelectBuilding || SelectBuilding->GetBuildingInfo().m_StateType != EBuildingStateType::Alpha) {return;}

	
	SelectBuilding->IsBuildPossible(m_BuildActor);

	
	
	
	
	
	


	

	
	// FRotator BuildingRotator = SelectBuilding->GetActorRotation();
	// BuildingRotator.Yaw = 90.f * m_BuildActor->GetBuildingRotationIdx();
	// SelectBuilding->SetActorRelativeRotation(BuildingRotator);
	//
	// SelectBuilding->SetBuildingMesh(NewBuildingClassInfo.SourceMesh);
	//
	// if(SelectBuilding->GetBuildingInfo().m_BuildType != NewBuildingClassInfo.BuildType)
	// {
	// 	SelectBuilding->SetBuildingStateType(EBuildingStateType::Red);
	// }
	// else
	// {
	// 	SelectBuilding->SetBuildingStateType(EBuildingStateType::Green);
	// }
		
	
	m_ArrayBuildingGreen.Add(SelectBuilding);

	
}

void ABoat::ShowBuildGreenInstall()
{
	// // �������� �� �ʱ�ȭ
	// for(auto Building : m_ArrayBuildingGreen)
	// {
	// 	FRotator BuildingRotator = Building->GetActorRotation();
	// 	Building->SetBuildingStateType(EBuildingStateType::Alpha);
	// }

	// Install ������ ���ٸ� ����
	if(nullptr == m_BuildingInstall)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_BuildingInstall = GetWorld()->SpawnActor<ABuilding>(m_BuildActor->GetNewBuildingClassInfo().BuildingClass, GetActorLocation(), GetActorRotation(), param);
		m_BuildingInstall->SetActorLocation(m_BuildActor->GetRaycastLocation());
		m_BuildingInstall->SetBuildingName(m_BuildActor->GetNewBuildingClassInfo().Name);
		m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Red);
		FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(m_BuildingInstall->GetActorLocation(), m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
		m_BuildingInstall->SetActorRotation(LookAtRoataor);
		m_BuildingInstall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		// install ������ ������ ���� �����ؾ� �ϴ� �Ͱ� �ٸ��ٸ� ����
		if(m_BuildingInstall->GetBuildingInfo().m_Name != m_BuildActor->GetNewBuildingClassInfo().Name)
		{
			m_BuildingInstall->Destroy();
			m_BuildingInstall = nullptr;
		}
		else
		{
			//m_BuildingInstall->IsBuildPossible();
			m_BuildingInstall->IsBuildPossible(m_BuildActor);
			
			// m_BuildingInstall->SetActorLocation(m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f) + m_BuildActor->GetFollowCamera()->GetForwardVector() * 300.f);
			// FRotator LookAtRoataor = UKismetMathLibrary::FindLookAtRotation(m_BuildingInstall->GetActorLocation(), m_BuildActor->GetActorLocation() + FVector(0.f, 0.f, 30.f));
			// LookAtRoataor.Pitch = 0.f;
			// LookAtRoataor.Roll = 0.f;
			// ABuilding* FloorBuilding = m_BuildingInstall->RaycastToFloor();
			// 	
			// if(FloorBuilding)
			// {
			// 	//float NewYaw = FVector::DotProduct(FloorBuilding->GetActorForwardVector(), m_BuildingInstall->GetActorForwardVector());
			// 		
			// 	//LookAtRoataor.Yaw = FloorBuilding->GetActorRotation().Yaw + (m_BuildActor->GetBuildingRotationIdx() * 90.f);
			// 	//LookAtRoataor.Yaw = NewYaw;
			// 	LookAtRoataor.Yaw = m_BuildingInstall->GetBuildingInstallYawToFloor(LookAtRoataor.Yaw);
			// 	LookAtRoataor.Yaw += FloorBuilding->GetActorRotation().Yaw;// + FloorBuilding->GetRootComponent()->GetRelativeRotation().Yaw;
			// 	LookAtRoataor.Yaw += 90.f * m_BuildActor->GetBuildingRotationIdx();
			// 	m_BuildingInstall->SetActorLocation(FloorBuilding->GetActorLocation());
			// 	m_BuildingInstall->SetActorRotation(LookAtRoataor);
			// 	m_BuildingInstall->SetIdx(FloorBuilding->GetBuildingInfo().m_IdxX, FloorBuilding->GetBuildingInfo().m_IdxY);
			// 	m_BuildingInstall->SetLayer(FloorBuilding->GetBuildingInfo().m_Layer);
			// 	
			// 	if(m_BuildingInstall->IsBuildPossible())
			// 	{
			// 		m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Green);
			// 	}
			// 	else
			// 	{
			// 		m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Red);
			// 	}
			// }
			// else
			// {
			// 	//LookAtRoataor.Yaw += m_BuildingInstall->GetActorRotation().Yaw;
			// 	LookAtRoataor.Yaw += 90.f * m_BuildActor->GetBuildingRotationIdx();
			// 	m_BuildingInstall->SetBuildingStateType(EBuildingStateType::Red);
			// 	m_BuildingInstall->SetActorRotation(LookAtRoataor);
			// }
		}
	}
}
