// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabFoliage.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"

AGrabFoliage::AGrabFoliage()
{
	PrimaryActorTick.bCanEverTick = true;
	
	m_IconType = EFeedbackIconType::None;
}

void AGrabFoliage::BeginPlay()
{
	Super::BeginPlay();
}

void AGrabFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrabFoliage::OnInteractionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionEnter(Player, HitComponent);
	
	UAnimMontage* Montage = Player->GetGrabMontage();
	if (Montage)
	{
		Player->GetAnim()->Montage_SetPosition(Player->GetGrabMontage(), 0.f);
		Player->GetAnim()->Montage_Play(Player->GetGrabMontage());
	}
	Gather(Player);
}

void AGrabFoliage::OnInteractionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionExit(Player, HitComponent);
}

void AGrabFoliage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);

	m_Mesh->SetRenderCustomDepth(true);
	m_Mesh->SetCustomDepthStencilValue(255);
	Player->SetItemInteractionUIText(true, TEXT("E"), TEXT("Pick Up Item"));
}

void AGrabFoliage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);
	m_Mesh->SetRenderCustomDepth(false);
	m_Mesh->SetCustomDepthStencilValue(255);
	Player->SetItemInteractionUIText(false);
}
