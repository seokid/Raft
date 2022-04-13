// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Unit/Monster/MonsterAIController.h"
#include "../Unit/Monster/Monster.h"

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("PlayerDetect");
	Interval = 0.5f;
}

void UBTService_PlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner()); 

	if (!Controller)
		return;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	if (!Monster)
		return;

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	FCollisionQueryParams params(NAME_None, false, Monster);	//�����浹 false
	
	FHitResult HitResult;	//�浹ó�� ����� �����ϴ� ����ü, faceidx �ε��� �ﰢ�� �ε���, ImpacPoint �ε��� ��ġ, Normal �ε��� ����

	//Single�� �Ѹ�, Multi�� ������, Over�� ��ħ��
	bool Sweep = GetWorld()->SweepSingleByChannel(HitResult
		, Monster->GetActorLocation()
		, Monster->GetActorLocation()
		, FQuat::Identity	//ȸ�� �ʿ����
		, ECollisionChannel::ECC_GameTraceChannel7
		, FCollisionShape::MakeSphere(MonsterInfo.TraceDistance)
		, params);

// #if ENABLE_DRAW_DEBUG
// 	FColor DrawColor = Sweep ? FColor::Red : FColor::Green;
// 	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), MonsterInfo.TraceDistance, 20, DrawColor, false, 0.3f);
// #endif

	if (Sweep)
	{
		ACharacter* Player = Cast<ACharacter>(HitResult.GetActor());
		if (Player && Player->GetCharacterMovement()->IsSwimming())
		{
			Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());
			return;
		}
	}
	
	Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
}
