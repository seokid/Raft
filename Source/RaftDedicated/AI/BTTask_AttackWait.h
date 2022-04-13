// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackWait.generated.h"

/**
 * 
 */
UCLASS()
class RAFTDEDICATED_API UBTTask_AttackWait : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AttackWait();

protected:
	float	m_WaitTime;
	
	float	m_AccTime;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
