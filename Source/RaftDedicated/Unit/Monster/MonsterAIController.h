// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree*	m_AITree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData*	m_AIBalckboard;

protected:
	virtual void OnPossess(APawn* InPawn) override;	//ºùÀÇÇÒ¶§
	virtual void OnUnPossess() override;			//ºùÀÇÅ»Ãâ
	
};
