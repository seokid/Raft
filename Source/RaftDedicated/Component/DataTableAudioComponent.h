// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/AudioComponent.h"
#include "DataTableAudioComponent.generated.h"


USTRUCT(BlueprintType)
struct FSoundTableInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString					Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32					Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*>		Sound;
};

UCLASS(ClassGroup=(Audio, Common), hidecategories=(Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories=Trigger, meta=(BlueprintSpawnableComponent))
class RAFTDEDICATED_API UDataTableAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UDataTableAudioComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UDataTable*			m_SoundInfoTable;

public:
	void PlayDataTableSound(const FString& Name, int32 Idx = -1);
	
};
