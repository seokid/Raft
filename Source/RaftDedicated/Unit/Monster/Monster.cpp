// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAIController.h"
#include "PatrolSpline.h"
#include "../../RaftDedicatedGameInstance.h"


AMonster::AMonster()
{
	m_AnimInstance = nullptr;

}

FVector AMonster::GetNextPatrolPoint(int32 Idx, float Length) const
{
	return m_PatrolSpline[Idx]->GetPatrolPoint(Length);
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	m_AnimInstance = Cast<UMonsterAnim>(GetMesh()->GetAnimInstance());

	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FMonsterTableInfo* Info = GameInst->FindMonsterInfo(m_MonsterInfoName);
		if (Info)
		{
			m_MonsterInfo.Name = Info->Name;
			m_MonsterInfo.Attack = Info->Attack;
			m_MonsterInfo.Armor = Info->Armor;
			m_MonsterInfo.HP = Info->HP;
			m_MonsterInfo.HPMax = Info->HPMax;
			
			m_MonsterInfo.WalkSpeed = Info->WalkSpeed;
			m_MonsterInfo.RunSpeed = Info->RunSpeed;
			m_MonsterInfo.SwimSpeed = Info->SwimSpeed;
			m_MonsterInfo.SwimRunSpeed = Info->SwimRunSpeed;

			m_MonsterInfo.AttackSpeed = Info->AttackSpeed;
			m_MonsterInfo.TraceDistance = Info->TraceDistance;
			m_MonsterInfo.AttackDistance = Info->AttackDistance;
			m_MonsterInfo.AttackRange = Info->AttackRange;
			
			
		}
	}
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (0.f == Damage)
		return 0.f;

	

	Damage = Damage - m_MonsterInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	LOG(TEXT("TakeDamge : %f"), Damage);
	
	m_MonsterInfo.HP -= Damage;

	m_AnimInstance->SetAnimType(EMonsterAnimType::Damaged);
	
	if (0 >= m_MonsterInfo.HP)
	{
		m_AnimInstance->SetAnimType(EMonsterAnimType::Dead);
		
		AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());

		if (MonsterController)
		{
			MonsterController->BrainComponent->StopLogic(TEXT("Dead"));
			MonsterController->StopMovement();
			GetCharacterMovement()->Buoyancy = 1.5f;
		}
	}


	return Damage;
}

void AMonster::NormalAttack()
{
	 
}

void AMonster::NormalAttackEnd()
{
	
}

void AMonster::NextPatrolLine()
{
	
}
