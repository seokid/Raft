// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "RaftDedicated/UI/MainHUD.h"
#include "RaftDedicated/UI/QuickItemSlotList.h"

AFood::AFood()
{
	PrimaryActorTick.bCanEverTick = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);
	
	if (UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Food")))
	{
		StaticMesh->SetCollisionProfileName(TEXT("Equipment"));
    	m_Mesh = StaticMesh;
    	m_Mesh->SetupAttachment(m_RootComponent);	
	}

	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    if (m_AudioComponent)
    {
    	m_AudioComponent->SetupAttachment(m_RootComponent);
    	m_AudioComponent->bOverrideAttenuation = true;
    	m_AudioComponent->bAutoActivate = false;
    }
	
	m_FoodName = TEXT("");
}

void AFood::BeginPlay()
{
	Super::BeginPlay();

	if (!m_FoodName.IsEmpty())
	{
		URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());
		if (GameInst)
		{
			
			const FFoodTableInfo* Info = GameInst->FindFoodInfo(m_FoodName);
			if (Info)
			{
				m_Info.Name = Info->Name;
				m_Info.Hungry = Info->Hungry;
				m_Info.Thirsty = Info->Thirsty;
				m_Info.Type = Info->Type;
			}
		}
	}
}

void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFood::Use()
{
	Super::Use();
	Eat();
}

void AFood::LButtonAction(bool bPress)
{ 
	Super::LButtonAction(bPress);

	if (bPress)
	{
		if (m_AudioComponent->Sound)
		{
			m_AudioComponent->Play();
		}
		m_OwnerAnim->Montage_JumpToSection(TEXT("Eat"), m_OwnerAnim->GetCurrentActiveMontage());
	}
		
}

void AFood::Eat()
{
	
	
	m_Owner->EatFood(m_Info.Hungry, m_Info.Thirsty);

	int ItemRemoveCount = 1;
	m_Owner->GetMainHUD()->GetQuickItemSlotListWidget()->RemoveSelectItem(m_FoodName, ItemRemoveCount);
}

