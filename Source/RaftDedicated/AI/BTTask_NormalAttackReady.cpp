// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NormalAttackReady.h"
#include "../Unit/Monster/Monster.h"
#include "../Unit/Monster/MonsterAIController.h"
#include "../Unit/PlayerCharacter/PlayerCharacter.h"

UBTTask_NormalAttackReady::UBTTask_NormalAttackReady()
{
	NodeName = TEXT("NormalAttackReady");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_NormalAttackReady::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	Monster->GetAnim()->SetAnimType(EMonsterAnimType::Attack);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_NormalAttackReady::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_NormalAttackReady::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (EMonsterAnimType::Damaged == Monster->GetAnim()->GetAnimType()
		|| EMonsterAnimType::Dead == Monster->GetAnim()->GetAnimType())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	const FVector TargetLoc = Target->GetActorLocation();
	const FVector MonsterLoc = Monster->GetMesh()->GetSocketLocation(TEXT("SharkHead"));
	FVector Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();
	
	Monster->AddMovementInput(Dir, 1.f);
	const FRotator Rot = Dir.Rotation();
	Monster->SetActorRotation(Rot);

	const float Dist = FVector::Distance(MonsterLoc, TargetLoc);

	
	
	if (MonsterInfo.AttackRange >= Dist)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
