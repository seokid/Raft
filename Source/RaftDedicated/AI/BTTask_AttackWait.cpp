// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackWait.h"
#include "../Unit/Monster/Monster.h"
#include "../Unit/Monster/MonsterAIController.h"
#include "../Unit/PlayerCharacter/PlayerCharacter.h"

UBTTask_AttackWait::UBTTask_AttackWait()
{
	NodeName = TEXT("AttackWait");
	bNotifyTick = true;

	m_WaitTime = 0.f;
	m_AccTime = 0.f;
}

EBTNodeResult::Type UBTTask_AttackWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target || !Target->GetCharacterMovement()->IsSwimming())
	{
		Controller->StopMovement();
		return EBTNodeResult::Failed;
	}
	
	
	m_AccTime = 0.f;
	m_WaitTime = Monster->GetMonsterInfo().AttackSpeed;
	Monster->GetAnim()->SetAnimType(EMonsterAnimType::Move);
	
	return EBTNodeResult::InProgress;
	
}

EBTNodeResult::Type UBTTask_AttackWait::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_AttackWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
    
    if (!Controller)
    {
    	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    	return;
    }

    AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

    if (!Monster)
    {
    	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    	return;
    }

    APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target || !Target->GetCharacterMovement()->IsSwimming())
    {
    	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    	return;
    }

	m_AccTime += DeltaSeconds;
	
	FVector Dir = Monster->GetCharacterMovement()->Velocity;
	Dir.Normalize();
	Monster->AddMovementInput(Dir, 1.f);
	Monster->SetActorRotation(Dir.Rotation());

	if (m_WaitTime <= m_AccTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
