// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterCup.h"
#include "Food.h"
#include "../../Unit/PlayerCharacter/PlayerCharacter.h"
#include "WaterBodyOceanActor.h"
#include "../../UI/MainHUD.h"
#include "../../UI/QuickItemSlotList.h"

AWaterCup::AWaterCup()
{
	PrimaryActorTick.bCanEverTick = true;
	m_bInteractionWater = false;

	// Water Static Mesh Component 
	m_WaterStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
	m_WaterStaticMeshComponent->SetupAttachment(m_RootComponent);
	
	// Water Static Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshAsset(TEXT("StaticMesh'/Game/Item/Equipment/Food/Mesh/SM_Plastic_FreshWater.SM_Plastic_FreshWater'"));

	if(WaterMeshAsset.Succeeded())
	{
		m_WaterStaticMeshComponent->SetStaticMesh(WaterMeshAsset.Object);
	}

	// Empty Water Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> EmptyWaterMaterialAsset(TEXT("Material'/Game/Item/Building/Common/Building_Alpha.Building_Alpha'"));
	if(EmptyWaterMaterialAsset.Succeeded())
	{
		m_EmptyWaterMaterial = EmptyWaterMaterialAsset.Object;
	}

	// Salt Water Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SaltWaterMaterialAsset(TEXT("Material'/Game/Item/Equipment/Food/Material/M_CupFluid2.M_CupFluid2'"));
	if(SaltWaterMaterialAsset.Succeeded())
	{
		m_SaltWaterMaterial = SaltWaterMaterialAsset.Object;
	}

	// Fresh Water Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> m_FreshWaterMaterialAsset(TEXT("Material'/Game/Item/Equipment/Food/Material/M_CupFluid.M_CupFluid'"));
	if(m_FreshWaterMaterialAsset.Succeeded())
	{
		m_FreshWaterMaterial = m_FreshWaterMaterialAsset.Object;
	}

	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	if (m_AudioComponent)
	{
		m_AudioComponent->SetupAttachment(GetRootComponent());
		m_AudioComponent->bOverrideAttenuation = true;
		m_AudioComponent->bAutoActivate = false;
	}

	m_WaterCupSound = nullptr;
	m_DrinkSound = nullptr;
}

void AWaterCup::BeginPlay()
{
	Super::BeginPlay();

	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());
	if (GameInst)
	{
		
		const FFoodTableInfo* Info = GameInst->FindFoodInfo(TEXT("FreshWater"));
		if (Info)
		{
			m_FreshWaterInfo.Name = Info->Name;
			m_FreshWaterInfo.Hungry = Info->Hungry;
			m_FreshWaterInfo.Thirsty = Info->Thirsty;
			m_FreshWaterInfo.Type = Info->Type;
		}

		const FFoodTableInfo* Info2 = GameInst->FindFoodInfo(TEXT("SaltWater"));
		if (Info2)
		{
			m_SaltWaterInfo.Name = Info2->Name;
			m_SaltWaterInfo.Hungry = Info2->Hungry;
			m_SaltWaterInfo.Thirsty = Info2->Thirsty;
			m_SaltWaterInfo.Type = Info2->Type;
		}
	}
	
}

void AWaterCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_Owner)
		return;

	if(IsEmptyWater())
	{
		m_Owner->SetItemInteractionUIText(IsRaycastToOcean(), TEXT("L"), TEXT("Fill SaltWater"));
	}
	
	// if (!m_Food)
	// {
	// 	
	// 	
	// 	// const bool bPreWater = m_bInteractionWater;
	// 	// m_bInteractionWater = Cast<AWaterBodyOcean>(m_Owner->GetInteractionActor());
	// 	//
	// 	// if (bPreWater != m_bInteractionWater)
 //  //       {
 //  //           m_Owner->SetItemInteractionUIText(m_bInteractionWater, TEXT("E"), TEXT("Fill SaltWater"));
 //  //       }
	// }
}

bool AWaterCup::OnEquip(APlayerCharacter* EquipOwner)
{
	bool result = Super::OnEquip(EquipOwner);

	if(nullptr != m_Owner)
	{
		const FString& ItemName = m_Owner->GetMainHUD()->GetQuickItemSlotListWidget()->GetSelectItemName();
		if(ItemName == TEXT("PlasticCup"))
		{
			ChangeWater(EWaterType::Empty, false);
		}
		else if(ItemName == TEXT("PlasticCup_SaltWater"))
		{
			ChangeWater(EWaterType::SaltWater, false);
		}
		else if(ItemName == TEXT("PlasticCup_FreshWater"))
		{
			ChangeWater(EWaterType::FreshWater, false);
		}
	}
	
	return result;
}

void AWaterCup::UnEquip()
{
	m_Owner->SetItemInteractionUIText(false);
	
	Super::UnEquip();
}

void AWaterCup::Use()
{
	Super::Use();

	if (m_DrinkSound)
	{
		FFoodInfo Info = m_FreshWaterInfo;
		if (m_WaterType == EWaterType::SaltWater)
		{
			Info = m_SaltWaterInfo;
		}
		m_Owner->EatFood(Info.Hungry, Info.Thirsty);
		m_AudioComponent->SetSound(m_DrinkSound);
		m_AudioComponent->Play();
		
		ChangeWater(EWaterType::Empty);
	}

}

void AWaterCup::LButtonAction(bool bPress)
{
	//Super::LButtonAction(bPress);

	if(bPress && IsEmptyWater())
	{
		InteractionToOcean();
	}
	else if(bPress && false == IsEmptyWater())
	{
		m_OwnerAnim->Montage_JumpToSection(TEXT("Eat"), m_OwnerAnim->GetCurrentActiveMontage());
		
	}
}

void AWaterCup::Key_E_Action()
{
	Super::Key_E_Action();

	InteractionToOcean();
}

void AWaterCup::RButtonAction(bool bPress)
{
	Super::RButtonAction(bPress);

	if(bPress && false == IsEmptyWater())
	{
		m_OwnerAnim->Montage_JumpToSection(TEXT("Plant"), m_OwnerAnim->GetCurrentActiveMontage());
		ChangeWater(EWaterType::Empty);
	}
}


void AWaterCup::ChangeWater(EWaterType Type, bool ChangeIcon)
{
	if(Type == m_WaterType) { return; }

	m_WaterType = Type;
	
	switch (m_WaterType)
	{
	case EWaterType::Empty:
		{
			m_WaterStaticMeshComponent->SetMaterial(0, m_EmptyWaterMaterial);
			if(m_Owner && ChangeIcon)
			{
				int32 ItemCount = 1;
				m_Owner->GetMainHUD()->GetQuickItemSlotListWidget()->ChangeSelectItem(TEXT("PlasticCup"), ItemCount, false);
			}
			break;
		}
	case EWaterType::SaltWater:
		{
			m_WaterStaticMeshComponent->SetMaterial(0, m_SaltWaterMaterial);
			if(m_Owner && ChangeIcon)
			{
				int32 ItemCount = 1;
				m_Owner->GetMainHUD()->GetQuickItemSlotListWidget()->ChangeSelectItem(TEXT("PlasticCup_SaltWater"), ItemCount, false);
			}
			break;
		}
	case EWaterType::FreshWater:
		{
			m_WaterStaticMeshComponent->SetMaterial(0, m_FreshWaterMaterial);
			if(m_Owner && ChangeIcon)
			{
				int32 ItemCount = 1;
				m_Owner->GetMainHUD()->GetQuickItemSlotListWidget()->ChangeSelectItem(TEXT("PlasticCup_FreshWater"), ItemCount, false);
			}
			break;
		}
	}
}


bool AWaterCup::IsRaycastToOcean()
{
	return nullptr != Cast<AWaterBodyOcean>(m_Owner->GetInteractionActor());
}

void AWaterCup::InteractionToOcean()
{
	if(false == IsRaycastToOcean()) { return; }
	if(false == IsEmptyWater()) { return; }

	if (m_WaterCupSound)
	{
		m_AudioComponent->SetSound(m_WaterCupSound);
		m_AudioComponent->Play();	
	}
	

	m_OwnerAnim->Montage_JumpToSection(TEXT("Plant"), m_OwnerAnim->GetCurrentActiveMontage());

	m_Owner->SetItemInteractionUIText(false);

	ChangeWater(EWaterType::SaltWater);
	
	// if (!m_Food  && m_bInteractionWater)
	// {
	// 	m_OwnerAnim->Montage_JumpToSection(TEXT("Plant"), m_OwnerAnim->GetCurrentActiveMontage());
	// 	
	// 	FActorSpawnParameters param;
	// 	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 	AFood* Food = GetWorld()->SpawnActor<AFood>(m_SaltWaterClass, FVector::ZeroVector, FRotator::ZeroRotator, param);
	// 	Super::PutIn(Food);
	//
	// 	m_bInteractionWater = !m_bInteractionWater;
	// 	m_Owner->SetItemInteractionUIText(m_bInteractionWater);
	// 	
	// }
}

