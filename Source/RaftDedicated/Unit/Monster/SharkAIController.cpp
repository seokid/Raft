// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAIController.h"

ASharkAIController::ASharkAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> AITreeAsset(TEXT("BehaviorTree'/Game/Unit/Monster/Shark/BT_Shark.BT_Shark'"));
	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;
		

	static ConstructorHelpers::FObjectFinder<UBlackboardData> AIBlackboardAsset(TEXT("BlackboardData'/Game/Unit/Monster/BB_Monster.BB_Monster'"));
	if (AIBlackboardAsset.Succeeded())
		m_AIBalckboard = AIBlackboardAsset.Object;
}

void ASharkAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASharkAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
