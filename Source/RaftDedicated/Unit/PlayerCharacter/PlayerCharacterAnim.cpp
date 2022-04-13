// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnim.h"
#include "PlayerCharacter.h"

UPlayerCharacterAnim::UPlayerCharacterAnim()
{
	m_Direction = 0.f;
	m_Speed = 0.f;
	m_AnimType = EPlayerAnimType::Ground;
	m_bOnEquip = false;
}

void UPlayerCharacterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UPlayerCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (!Player)
		return;
	
	UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
	if (!Movement)
		return;

	if (EPlayerAnimType::Death == m_AnimType)
		return;
	

	m_Speed = Movement->Velocity.Size();
	const FVector Forward = Player->GetActorForwardVector();
	FVector Dir = Movement->Velocity;
	Dir.Normalize();

	const float Dot = FVector::DotProduct(Dir, Forward);
	m_Direction = FMath::RadiansToDegrees(FMath::Acos(Dot));
	
	const float Det = FVector::DotProduct(Player->GetActorUpVector(), FVector::CrossProduct(Dir, Forward));
	if (Det > 0.f)
		m_Direction *= -1.f;
		
	if (Movement->IsFalling())
	{
		if (0.f < Player->GetVelocity().Z)
		{
			m_AnimType = EPlayerAnimType::Jump;	
		}
		else
		{
			m_AnimType = EPlayerAnimType::Fall;	
		}
	}
	else if (Movement->IsSwimming())
	{
		m_AnimType = EPlayerAnimType::Swimming;
	}
	else if (Movement->IsCrouching())
	{
		m_AnimType = EPlayerAnimType::Crouch;
	}
	else if (Movement->IsWalking())
	{
		m_AnimType = EPlayerAnimType::Ground;
	}
	
}



void UPlayerCharacterAnim::AnimNotify_Use()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->UseEquipment();
	}
}

void UPlayerCharacterAnim::AnimNotify_UseEnd()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->UseEndEquipment();
	}
}

void UPlayerCharacterAnim::AnimNotify_GrabEnd()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->GrabEnd();
	}
}

void UPlayerCharacterAnim::AnimNotify_Swim()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->Swim();
	}
}

void UPlayerCharacterAnim::AnimNotify_BeginCharging()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->BeginChargingEquipment();	
	}
}

void UPlayerCharacterAnim::AnimNotify_EndCharging()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
	{
		Player->EndChargingEquipment();	
	}
}
