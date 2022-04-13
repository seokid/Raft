// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "Male.generated.h"

class UAnimMontage;

UCLASS()
class RAFTDEDICATED_API AMale : public APlayerCharacter
{
	GENERATED_BODY()
public:
	AMale();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_AnimMontage;
};
