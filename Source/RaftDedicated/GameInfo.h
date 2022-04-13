#pragma once


#include "EngineMinimal.h"
#include "Engine.h"
#include "engine/AssetManager.h"


//DrawDebug
#include "DrawDebugHelpers.h"
#include "LauncherServices/Public/ILauncherProfile.h"

//MoveComponentTo
#include "Kismet/KismetSystemLibrary.h"

//SetScalarParameterValue
#include "Kismet/KismetMaterialLibrary.h"

//Water
#include "WaterModule.h"

#include "GameInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(RaftDedicated, Log, All);	///해당 카테고리를 이용해서 로그를 출력하게끔 하는것
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))		//__FUNTION__ : 현재 이 로그를 출력하는 함수 이름을 의미한다
#define LOG(Format, ...) UE_LOG(RaftDedicated, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

void PrintViewport(float Time, const FColor& Color, const FString& Text);

#define RAFT_SCALE FVector(100.f, 100.f, 100.f)

template <typename UENUMTYPE>
FString UEnumToString(UENUMTYPE enumValue)
{
	FString text = typeid(UENUMTYPE).name();
	text.ReplaceInline(TEXT("enum "), TEXT(""));

	LOG(TEXT("%s"), *text);
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *text, true);
	if (!enumPtr)
		return FString("Invalid");

	return enumPtr->GetNameStringByIndex(static_cast<int32>(enumValue));
}

UENUM(BlueprintType)
enum class EBuildingStateType : uint8
{
	Build,
	Green,
	Red,
	Alpha,
};

UENUM(BlueprintType)
enum class EBlockFoundationDirType : uint8
{
	Up,
	Down,
	Right,
	Left,

	End,
};

UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Bottom,
	Floor,
	Install,
};

UENUM(BlueprintType)
enum class ENotebookCraftingCategoryType : uint8
{
	FoodWater,
	Other,
	Utilities,
	Equipment,
	Materials,
	Navigation,
	Decoration,

	None,
};


USTRUCT(BlueprintType)
struct FBlockFoundationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_Idx_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_Idx_Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBuildingStateType	m_StateType;
	
	FBlockFoundationInfo()
	{
		m_Idx_X = 0;
		m_Idx_Y = 0;
		m_Layer = 0;
		m_StateType = EBuildingStateType::Build;
	}

public:
	bool IsIdxSame(int32 _Idx_X, int32 _Idx_Y) const
	{
		return m_Idx_X == _Idx_X && m_Idx_Y == _Idx_Y;
	}

	bool IsIdxSame(const FBlockFoundationInfo& _Info) const
	{
		return m_Idx_X == _Info.m_Idx_X && m_Idx_Y == _Info.m_Idx_Y;
	}
};

// new
USTRUCT(BlueprintType)
struct FBuildingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	m_Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_IdxX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_IdxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32	m_Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBuildingStateType	m_StateType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EBuildType		m_BuildType;

	FBuildingInfo()
	{
		m_Name.Empty();
		m_IdxX = 0;
		m_IdxY = 0;
		m_Layer = 0;
		m_StateType = EBuildingStateType::Build;
		m_BuildType = EBuildType::Floor;
	}

	bool IsIdxSame(int32 IdxX, int32 IdxY) const
	{
		return m_IdxX == IdxX && m_IdxY == IdxY;
	}

	bool IsIdxSame(const FBuildingInfo& Info) const
	{
		return m_IdxX == Info.m_IdxX && m_IdxY == Info.m_IdxY;
	}

	void operator = (const FBuildingInfo& Info)
	{
		if(!Info.m_Name.IsEmpty())
		{
			m_Name = Info.m_Name;
		}
		m_IdxX = Info.m_IdxX;
		m_IdxY = Info.m_IdxY;
		m_Layer = Info.m_Layer;
		m_StateType = Info.m_StateType;
		m_BuildType = Info.m_BuildType;
	}
};

USTRUCT(BlueprintType)
struct FCraftingItemParts
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString		Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32		Count;
};

USTRUCT(BlueprintType)
struct FItemDropInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString		Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float		DropPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32		DropMinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32		DropMaxCount;
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Part,
	Equipment,
	Build_Install,
};

UENUM(BlueprintType)
enum class EFoodTypeInfo : uint8
{
	Fruit,
	Raw,
	Cooked,
};

UENUM(BlueprintType)
enum class EWaterType : uint8
{
	Empty,
	SaltWater,
	FreshWater,
};

UENUM(BlueprintType)
enum class EFeedbackIconType : uint8
{
	None,
	Axe,
	Bucket,
	Hook,
	Machete,
	Shear,
	Shovel,
};

UENUM(BlueprintType)
enum class EUISoundType : uint8
{
	Hovered,
	MenuClicked,
	ButtonClicked,
	ButtonFailClicked,
	Drop,
	PickUp,
};
