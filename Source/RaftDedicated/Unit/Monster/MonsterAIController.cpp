// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
	m_AIBalckboard = nullptr;
	m_AITree = nullptr;
}


void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//�����带 ����
	if (UseBlackboard(m_AIBalckboard, Blackboard))
	{
		//�ൿƮ���� ����
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Monster BehaviorTree Error!!"));
		}
		else
		{
			LOG(TEXT("Monster BehaviorTree Success!!"));
		}
	}
}
void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();

}