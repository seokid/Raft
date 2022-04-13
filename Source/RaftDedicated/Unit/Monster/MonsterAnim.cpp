// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnim.h"
#include "Monster.h"

UMonsterAnim::UMonsterAnim()
{
	m_AnimType = EMonsterAnimType::Move;

	m_bAttack = false;
}

void UMonsterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (!Monster)
		return;
	
	UCharacterMovementComponent* Movement = Monster->GetCharacterMovement();
	if (!Movement)
		return;

	m_Speed = Movement->Velocity.Size();
	const FVector Forward = Monster->GetActorForwardVector();
	FVector Dir = Movement->Velocity;
	Dir.Normalize();

	const float Dot = FVector::DotProduct(Dir, Forward);
	m_Direction = FMath::RadiansToDegrees(FMath::Acos(Dot));
	m_Direction = abs(m_Direction);
	const float Det = FVector::DotProduct(Monster->GetActorUpVector(), FVector::CrossProduct(Dir, Forward));
	if (Det > 0.f)
		m_Direction *= -1.f;
}


void UMonsterAnim::AnimNotify_NormalAttack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (!Monster)
		return;
	
	Monster->NormalAttack();
}

void UMonsterAnim::AnimNotify_NormalAttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	if (!Monster)
		return;
	
	m_bAttack = false;
	Monster->NormalAttackEnd();
}
