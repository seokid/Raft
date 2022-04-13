// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"
#include "../Unit/Monster/Monster.h"
#include "../Unit/Monster/MonsterAIController.h"
#include "../Unit/PlayerCharacter/PlayerCharacter.h"

UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	NodeName = TEXT("TargetTrace");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 공격거리 안에 들어왔는지 여기서도 체크하여 들어갔다면 Trace를 종료시킨다
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

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	Monster->GetCharacterMovement()->MaxWalkSpeed = MonsterInfo.RunSpeed;
	Monster->GetCharacterMovement()->MaxSwimSpeed = MonsterInfo.SwimRunSpeed;
	
	// const float Distance = FVector::Distance(Monster->GetActorLocation(), Target->GetActorLocation());
	// if (Distance <= MonsterInfo.AttackDistance)
	// {
	// 	Controller->StopMovement();
	// 	return EBTNodeResult::Succeeded;
	// }
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	const FVector MonsterLoc = Monster->GetActorLocation();
	const FVector TargetLoc = Target->GetActorLocation();
	
	FVector Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();
	Monster->AddMovementInput(Dir, 1.f);
	// FRotator Rot = Dir.Rotation();
	// Rot.Pitch = Monster->GetActorRotation().Pitch;
	Monster->SetActorRotation(Dir.Rotation());

// #if ENABLE_DRAW_DEBUG
// 	FColor DrawColor = FColor::Green;
// 	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), MonsterInfo.AttackDistance, 20, DrawColor, false, 0.3f);
// #endif

	const float Distance = FVector::Distance(MonsterLoc, TargetLoc);
    
    if (Distance <= MonsterInfo.AttackDistance)
    {
    	Controller->StopMovement();
    	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
