// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "../Unit/Monster/Monster.h"
#include "../Unit/Monster/MonsterAIController.h"
#include "../Unit/PlayerCharacter/PlayerCharacter.h"


UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
	
	m_CurPatrolLength = 0.f;
	m_Dir = 1.f;
	m_PatrolIdx = 0;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterInfo().WalkSpeed;
	Monster->GetCharacterMovement()->MaxSwimSpeed = Monster->GetMonsterInfo().SwimSpeed;

	Monster->GetAnim()->SetAnimType(EMonsterAnimType::Move);

	m_PatrolOrder.Empty();
	for (int32 i = 0; i < Monster->GetPatrolSplineSize(); ++i)
	{
		m_PatrolOrder.Push(i);
	}
	
	ShuffleOrder();
	
	return EBTNodeResult::InProgress;
	
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (1 > m_PatrolOrder.Num())
		return;

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
	
	if (Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	const FVector PatrolPoint = Monster->GetNextPatrolPoint(m_PatrolOrder[m_PatrolIdx], m_CurPatrolLength);
	const FVector MonsterLoc = Monster->GetActorLocation();
	
	FVector Dir = PatrolPoint - MonsterLoc;
	Dir.Normalize();
	Monster->AddMovementInput(Dir, 1.f);
	FRotator Rot = Dir.Rotation();
	Rot.Pitch = Monster->GetActorRotation().Pitch;
	Monster->SetActorRotation(Rot);
	
	m_CurPatrolLength += Monster->GetCharacterMovement()->GetMaxSpeed() * DeltaSeconds;
	
	if (m_CurPatrolLength >= Monster->GetPatrolLength(m_PatrolOrder[m_PatrolIdx]))
	{
		
		m_CurPatrolLength = 0;
		NextPatrolIdx();
	}
	
}

void UBTTask_Patrol::ShuffleOrder()
{
	if (2 > m_PatrolOrder.Num())
		return;
	
	int32 Swap;
	const int32 Size = m_PatrolOrder.Num() - 1;
	for (int32 i = 0; i <= Size; ++i)
	{
		 Swap = FMath::RandRange(0, Size);
		if (i == Swap)
			continue;

		const int32 Tmp = m_PatrolOrder[i];
		m_PatrolOrder[i] = m_PatrolOrder[Swap];
		m_PatrolOrder[Swap] = Tmp;
	}
}

void UBTTask_Patrol::NextPatrolIdx()
{
	m_PatrolIdx = ++m_PatrolIdx % m_PatrolOrder.Num();
}
