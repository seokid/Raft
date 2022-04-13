// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftDedicatedGameInstance.h"
#include "Effect/NormalSound.h"

URaftDedicatedGameInstance::URaftDedicatedGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerInfotableAsset(TEXT("DataTable'/Game/Unit/Player/DT_PlayerInfo.DT_PlayerInfo'"));
	
	if (PlayerInfotableAsset.Succeeded())
	{
		m_PlayerInfoTable = PlayerInfotableAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterInfotableAsset(TEXT("DataTable'/Game/Unit/Monster/DT_MonsterInfo.DT_MonsterInfo'"));
	
	if (MonsterInfotableAsset.Succeeded())
	{
		m_MonsterInfoTable = MonsterInfotableAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentInfotableAsset(TEXT("DataTable'/Game/Unit/Player/Male/DT_Male_EquipInfo.DT_Male_EquipInfo'"));
	
	if (EquipmentInfotableAsset.Succeeded())
	{
		m_EquipmentInfoTable = EquipmentInfotableAsset.Object;
	}
	

	static ConstructorHelpers::FObjectFinder<UDataTable> BuildingTableInfo(TEXT("DataTable'/Game/Item/Building/Common/DT_Building_Info.DT_Building_Info'"));
	
	if (BuildingTableInfo.Succeeded())
	{
		m_BuildingTableInfo = BuildingTableInfo.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemIconTableInfo(TEXT("DataTable'/Game/UI/Common/DataTable/DT_ItemIconTableInfo.DT_ItemIconTableInfo'"));
	if (ItemIconTableInfo.Succeeded())
	{
		m_ItemIconTableInfo = ItemIconTableInfo.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CraftingItemTableInfo(TEXT("DataTable'/Game/UI/Common/DataTable/DT_CraftingItemTableInfo.DT_CraftingItemTableInfo'"));
	if (CraftingItemTableInfo.Succeeded())
	{
		m_CraftingItemTableInfo = CraftingItemTableInfo.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> FoodTableInfo(TEXT("DataTable'/Game/Item/Equipment/Food/DT_FoodInfo.DT_FoodInfo'"));
	if (FoodTableInfo.Succeeded())
	{
		m_FoodInfoTable = FoodTableInfo.Object;
	}
	
	
}

void URaftDedicatedGameInstance::Init()
{
	Super::Init();
}

void URaftDedicatedGameInstance::Shutdown()
{
	Super::Shutdown();
}

const FPlayerTableInfo* URaftDedicatedGameInstance::FindPlayerInfo(const FString& Name)
{
	return m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Name, "");
}

const FMonsterTableInfo* URaftDedicatedGameInstance::FindMonsterInfo(const FString& Name)
{
	return m_MonsterInfoTable->FindRow<FMonsterTableInfo>(*Name, "");
}

const FEquipmentTableInfo* URaftDedicatedGameInstance::FindEquipmentInfo(const FString& Name)
{
	return m_EquipmentInfoTable->FindRow<FEquipmentTableInfo>(*Name, "");
}

const FFoodTableInfo* URaftDedicatedGameInstance::FindFoodInfo(const FString& Name)
{
	return m_FoodInfoTable->FindRow<FFoodTableInfo>(*Name, "");
}

const FBuildingTableInfo* URaftDedicatedGameInstance::FindBuildingInfo(const FString& Name)
{
	return m_BuildingTableInfo->FindRow<FBuildingTableInfo>(*Name, "");
}

const FItemIconTableInfo* URaftDedicatedGameInstance::FindItemIconInfo(const FString& Name)
{
	return m_ItemIconTableInfo->FindRow<FItemIconTableInfo>(*Name, "");
}

const FCraftingItemTableInfo* URaftDedicatedGameInstance::FindCraftingItemInfo(const FString& Name)
{
	return m_CraftingItemTableInfo->FindRow<FCraftingItemTableInfo>(*Name, "");
}

void URaftDedicatedGameInstance::PlayToBGM(const FString& Path)
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	Effect->LoadSound(Path);
	Effect->Play(1.f, true, true);
}
