// Fill out your copyright notice in the Description page of Project Settings.


#include "Foliage.h"
#include "RaftDedicated/Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "RaftDedicated/UI/IneractionFeedback.h"

AFoliage::AFoliage()
{
	PrimaryActorTick.bCanEverTick = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);

	m_IconType = EFeedbackIconType::None;
	
}

void AFoliage::BeginPlay()
{
	Super::BeginPlay();
}

void AFoliage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFoliage::Gather(class APlayerCharacter* Player)
{
	Player->GetMainHUD()->GetIneractionFeedbackWidget()->SetFeedbackIconType(EFeedbackIconType::None);
	AddItemToPlayer(Player, m_ArrayItemDropInfo);
}

void AFoliage::OnInteractionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionEnter(Player, HitComponent);
}

void AFoliage::OnInteractionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnInteractionExit(Player, HitComponent);
}

void AFoliage::OnCollisionEnter(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionEnter(Player, HitComponent);

	Player->GetMainHUD()->GetIneractionFeedbackWidget()->SetFeedbackIconType(m_IconType);
}

void AFoliage::OnCollisionExit(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	Super::OnCollisionExit(Player, HitComponent);

	Player->GetMainHUD()->GetIneractionFeedbackWidget()->SetFeedbackIconType(EFeedbackIconType::None);
}

