// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "../../RaftDedicatedGameInstance.h"
#include "WaterBodyOceanActor.h"

#include "RaftDedicated/Item/Equipment/Equipment.h"
#include "RaftDedicated/Item/Equipment/FishingRod.h"
#include "RaftDedicated/Item/Equipment/ToolHammer.h"

#include "../../UI/MainHUD.h"
#include "../../UI/NotebookCrafting.h"

#include "../../Item/Building/Boat.h"
#include "../../Item/Building/Building.h"
#include "../../Effect/NormalSound.h"



APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_Arm->bUsePawnControlRotation = true;
	m_Arm->SetupAttachment(GetRootComponent());

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm);
	
	m_Head = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	m_Head->SetupAttachment(GetMesh());
	
	m_PlayerInfoName = "";
	m_PlayerInfo = FPlayerInfo{};
	m_PlayerInfo.bBreathAble = true;
	
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	m_EquipmentInfo = {};
	m_Equipment = nullptr;

	// MainHUD
	static ConstructorHelpers::FClassFinder<UMainHUD> MainHUDAsset(TEXT("WidgetBlueprint'/Game/UI/MainHUD/UI_MainHUD.UI_MainHUD_C'"));

	if (MainHUDAsset.Succeeded())
	{
		m_MainHUDClass = MainHUDAsset.Class;
	}
	m_MainHUDWidget = nullptr;
	
	// Item Interaction
	m_InteractionItem = nullptr;
	m_ItemHitComponent = nullptr;
	m_RaycastDist = 300.f;
	m_RaycastOffset = FVector(0.f ,0.f, 110.f);
	m_InteractionActor = nullptr;

	// Build
	m_BuildEnable = false;
	m_NewBuildingInfo = nullptr;
	
	m_NoseBoneName = NAME_None;

	m_bBreathLow = false;
	m_BreathLowPercent = 0.3f;
	m_bHungry = false;
	m_HungryPercent = 0.3f;
	m_bThirsty = false;
	m_ThirstyPercent = 0.3f;
	m_bBreathRecovery = false;

	//PostEffect
	m_bBMRScreen = false;

	//Respawn
	m_RespawnTime = 5.f;
	m_RespawnAccTime = 0.f;

}


void APlayerCharacter::SetBloodScreen(float Ratio)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(),m_PostEffectCollection, TEXT("Blood"), Ratio);
}

void APlayerCharacter::SetBMRScreen(float Ratio)
{
	m_bBMRScreen = !m_bBMRScreen; 
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(),m_PostEffectCollection, TEXT("BMR"), Ratio);
}

void APlayerCharacter::EatFood(int32 Hungry, int32 Thirsty)
{
	m_PlayerInfo.ThirstyAccTime = 0.f;
	m_PlayerInfo.HungryAccTime = 0.f;

	m_PlayerInfo.Hungry -= Hungry;
	if (0 > m_PlayerInfo.Hungry)
		m_PlayerInfo.Hungry = 0;

	if (m_PlayerInfo.HungryMax < m_PlayerInfo.Hungry)
		m_PlayerInfo.Hungry = m_PlayerInfo.HungryMax;

	m_PlayerInfo.Thirsty -= Thirsty;

	if (0 > m_PlayerInfo.Thirsty)
		m_PlayerInfo.Thirsty = 0;

	if (m_PlayerInfo.ThirstyMax < m_PlayerInfo.Thirsty)
		m_PlayerInfo.Thirsty = m_PlayerInfo.ThirstyMax;

	if (m_HungryPercent < 1.f - m_PlayerInfo.Hungry / m_PlayerInfo.HungryMax)
		HungryRecovery();

	if (m_ThirstyPercent < 1.f - m_PlayerInfo.Thirsty / m_PlayerInfo.ThirstyMax)
		ThirstyRecovery();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
	m_AnimInstance = Cast<UPlayerCharacterAnim>(GetMesh()->GetAnimInstance());
	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FPlayerTableInfo* Info = GameInst->FindPlayerInfo(m_PlayerInfoName);
		if (Info)
		{
			m_PlayerInfo.Name = Info->Name;
			m_PlayerInfo.Attack = Info->Attack;
			m_PlayerInfo.Armor = Info->Armor;
			m_PlayerInfo.HP = Info->HP;
			m_PlayerInfo.HPMax = Info->HPMax;
			m_PlayerInfo.Hungry = Info->Hungry;
			m_PlayerInfo.HungryMax = Info->HungryMax;
			m_PlayerInfo.HungrySpeed = Info->HungrySpeed;
			m_PlayerInfo.HungryAccTime = 0.f;
			m_PlayerInfo.Thirsty = Info->Thirsty;
			m_PlayerInfo.ThirstyMax = Info->ThirstyMax;
			m_PlayerInfo.ThirstySpeed = Info->ThirstySpeed;
			m_PlayerInfo.ThirstyAccTime = 0.f;
			m_PlayerInfo.Breath = Info->Breath;
			m_PlayerInfo.BreathMax = Info->BreathMax;
			m_PlayerInfo.BreathSpeed = Info->BreathSpeed;
			m_PlayerInfo.BreathAccTime = 0.f;
			m_PlayerInfo.HookDistance = Info->HookDistance;
			m_PlayerInfo.HookSpeed = Info->HookSpeed;
			m_PlayerInfo.HookAngle = Info->HookAngle;
			m_PlayerInfo.AttackDistance = Info->AttackDistance;
			m_PlayerInfo.AttackSpeed = Info->AttackSpeed;
			m_PlayerInfo.AttackAngle = Info->AttackAngle;
			m_PlayerInfo.WalkSpeed = Info->WalkSpeed;
			m_PlayerInfo.RunSpeed = Info->RunSpeed;
			m_PlayerInfo.CrouchSpeed = Info->CrouchSpeed;
			m_PlayerInfo.SwimSpeed = Info->SwimSpeed;
			m_PlayerInfo.FastSwimSpeed = Info->FastSwimSpeed;

			GetCharacterMovement()->MaxWalkSpeed = m_PlayerInfo.WalkSpeed;
			GetCharacterMovement()->MaxSwimSpeed = m_PlayerInfo.SwimSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = m_PlayerInfo.CrouchSpeed;
		}
	}

	// MainHUD
	if (IsValid(m_MainHUDClass))
	{
		m_MainHUDWidget = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));

		if (m_MainHUDWidget)
		{
			m_MainHUDWidget->AddToViewport();
			m_MainHUDWidget->SetPlayer(this);

			// Item
			int count1 = 60;
			m_MainHUDWidget->AddItem(TEXT("Plank"), count1);

			int count2 = 60;
			m_MainHUDWidget->AddItem(TEXT("Thatch"), count2);
			
			int count3 = 60;
			m_MainHUDWidget->AddItem(TEXT("Plastic"), count3);

			int count4 = 1;
			m_MainHUDWidget->AddItem(TEXT("Hammer"), count4);

			// int count5 = 2;
			// m_MainHUDWidget->AddItem(TEXT("Tilapia"), count5);
			
			// int count6 = 1;
			// m_MainHUDWidget->AddItem(TEXT("PlasticCup"), count6);

			int count7 = 1;
			m_MainHUDWidget->AddItem(TEXT("Hook"), count7);

			// int count8 = 20;
			// m_MainHUDWidget->AddItem(TEXT("Rope"), count8);

			// int count9 = 20;
			// m_MainHUDWidget->AddItem(TEXT("Rock"), count9);

			int count10 = 40;
			m_MainHUDWidget->AddItem(TEXT("Scrap"), count10);

			// int count11 = 20;
			// m_MainHUDWidget->AddItem(TEXT("Nail"), count11);

			// int count12 = 1;
			// m_MainHUDWidget->AddItem(TEXT("Axe"), count12);

	

			// Crafting Item
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::FoodWater, TEXT("PlasticCup"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::FoodWater, TEXT("Purifier"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::FoodWater, TEXT("Grill"));

			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Other, TEXT("Storage_Small"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Other, TEXT("Storage"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Other, TEXT("Storage_Bigger"));

			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Utilities, TEXT("Hammer"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Utilities, TEXT("Hook"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Utilities, TEXT("FishingRod"));

			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Equipment, TEXT("Spear"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Equipment, TEXT("Bow"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Equipment, TEXT("Arrow"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Equipment, TEXT("Axe"));

			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Materials, TEXT("Rope"));
			m_MainHUDWidget->GetNotebookCraftingWidget()->AddCraftingItem(ENotebookCraftingCategoryType::Materials, TEXT("Nail"));
		}
	}

	if (m_Head)
	{
		m_Head->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, m_NoseBoneName);
		m_Head->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::HeadBeginOverlap);
		m_Head->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::HeadEndOverlap);	
	}

	// Build
	SetBuildInfo(TEXT("Block_Foundation"));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
	{
		m_RespawnAccTime += DeltaTime;
        
        if (m_RespawnTime <= m_RespawnAccTime)
        {
        	Respawn();
        }
	}

	BMR(DeltaTime);

	const float Speed = GetCharacterMovement()->Velocity.Size();
	const EMovementMode CurMovementMode = GetCharacterMovement()->MovementMode;
	if (EMovementMode::MOVE_Swimming == CurMovementMode && Speed <= 200.f)
	{
		if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		{
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator(-80.f, GetActorRotation().Yaw, 0.f), DeltaTime, 0.5f));
			// GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 150.f);
		}
		else
		{
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator(0.f, GetActorRotation().Yaw, 0.f), DeltaTime, 2.f));	
		}
		
	}

	

	if (m_Equipment && m_Equipment->IsChargeEquip())
		m_MainHUDWidget->SetChargingPercent(m_Equipment->GetChargingRatio());

	float BloodRatio = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(),m_PostEffectCollection, TEXT("Blood"));
	if (0.f < BloodRatio)
	{
		SetBloodScreen(BloodRatio -= DeltaTime);
	}
	
	// ItemInteraction
	InteractionRaycast();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	const EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;

	if (EMovementMode::MOVE_Swimming != NewMovementMode)
	{
		SetActorRotation(FRotator(0.f, 0.f, GetActorRotation().Yaw));	
	}

	// Water Falling
	if (EMovementMode::MOVE_Falling == PrevMovementMode && EMovementMode::MOVE_Swimming == NewMovementMode)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, param);
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/watersplash_big02.watersplash_big02'"));
		Effect->Play();	
	}
	
	
	if (EMovementMode::MOVE_Swimming == NewMovementMode)
	{
		GetCharacterMovement()->RotationRate = FRotator(360.f, 360.f, 360.f);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
		SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}


void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (0.f == Damage)
		return 0.f;
	
	Damage = Damage - m_PlayerInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;
	m_PlayerInfo.HP -= Damage;

	
	if (0 >= m_PlayerInfo.HP)
	{
		if (!m_PlayerInfo.bBreathAble)
			Drowning();
		Death();
		
	}

	SetBloodScreen(5.f);
	m_AudioComponent->PlayDataTableSound(TEXT("Damage"));

	
	return Damage;
}

void APlayerCharacter::BMR(float DeltaTime)
{
	m_PlayerInfo.ThirstyAccTime += DeltaTime;
	m_PlayerInfo.HungryAccTime += DeltaTime;
	
	if (m_PlayerInfo.ThirstyMax >= m_PlayerInfo.Thirsty && m_PlayerInfo.ThirstyAccTime >= m_PlayerInfo.ThirstySpeed)
	{
		m_PlayerInfo.Thirsty += 1.f;
		m_PlayerInfo.ThirstyAccTime -= m_PlayerInfo.ThirstySpeed;

		if (!m_bThirsty && m_ThirstyPercent > 1.f - m_PlayerInfo.Thirsty / m_PlayerInfo.ThirstyMax)
		{
			Thirsty();
		}
	}

	if (m_PlayerInfo.HungryMax >= m_PlayerInfo.Hungry && m_PlayerInfo.HungryAccTime >= m_PlayerInfo.HungrySpeed)
	{
		m_PlayerInfo.Hungry += 1.f;
		m_PlayerInfo.HungryAccTime -= m_PlayerInfo.HungrySpeed;

		if (!m_bHungry && m_HungryPercent > 1.f - m_PlayerInfo.Hungry / m_PlayerInfo.HungryMax)
		{
			Hungry();
		}
	}

	//숨쉬지 못할때
	if (!m_PlayerInfo.bBreathAble)
	{
		if (m_PlayerInfo.BreathMax > m_PlayerInfo.Breath)
        {
			m_PlayerInfo.Breath += DeltaTime;

			if (!m_bBreathLow && m_BreathLowPercent > 1.f - m_PlayerInfo.Breath / m_PlayerInfo.BreathMax)
			{
				BreathLow();
			}
        }
	}
	else
	{
		if (0.f < m_PlayerInfo.Breath)
		{
			const float BreathRecoverySpeed = 15.f;
			m_PlayerInfo.Breath -= BreathRecoverySpeed * DeltaTime;

			if (!m_bBreathRecovery)
			{
				BreathRecoveryStart();
			}
			
			if (0.f >= m_PlayerInfo.Breath)
			{
				BreathRecoveryEnd();
			}
		}
	}
	
	if (m_bBMRScreen != (m_bHungry || m_bThirsty))
    {
		SetBMRScreen(!m_bBMRScreen ? 1.f : 0.f);	
    }
	
	if (m_MainHUDWidget)
	{
		m_MainHUDWidget->SetStatePercent(1.f - m_PlayerInfo.Thirsty / m_PlayerInfo.ThirstyMax, 1.f - m_PlayerInfo.Hungry / m_PlayerInfo.HungryMax, m_PlayerInfo.HP / static_cast<float>(m_PlayerInfo.HPMax));
		m_MainHUDWidget->SetBreathBarPercent(m_PlayerInfo.Breath / m_PlayerInfo.BreathMax);
	}
}


void APlayerCharacter::MoveForward(float Value)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (GetCharacterMovement()->IsSwimming())
	{
		AddMovementInput(m_Camera->GetForwardVector(), Value);
	}
	else
	{
		AddMovementInput(GetActorForwardVector(), Value);	
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::SwimmingUp(float Value)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (EPlayerAnimType::Swimming == m_AnimInstance->GetAnimType())
	{
		if (1.f <= abs(Value))
		{
			SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
			GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 300.f * Value);
		}
	}
}

void APlayerCharacter::Run_Implementation(bool bRun)
{
	if (bRun)
		GetCharacterMovement()->MaxWalkSpeed = m_PlayerInfo.RunSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = m_PlayerInfo.WalkSpeed;
}

void APlayerCharacter::CameraZoom(float Value)
{
	m_Arm->TargetArmLength += Value * 10.f;

	if (500.f < m_Arm->TargetArmLength)
	{
		m_Arm->TargetArmLength = 500.f;
	}
	else if (10.f > m_Arm->TargetArmLength)
	{
		m_Arm->TargetArmLength = 10.f;
	}
}

void APlayerCharacter::Jump()
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (GetCharacterMovement()->IsMovingOnGround())
    {
		Super::Jump();
    }		
    else if (GetCharacterMovement()->IsSwimming())
    {
    	FVector Vel = GetCharacterMovement()->Velocity;
    	Vel.Normalize();
    	FVector WallNormal(0.f);
    	if (GetCharacterMovement()->CheckWaterJump(Vel, WallNormal))
    	{
    		LaunchCharacter(FVector(0.f,0.f,1.f) * 600.f, true, true);
    	}
    }
}

void APlayerCharacter::Crouch(bool bCrouch)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (bCrouch)
		Super::Crouch();
	else
		Super::UnCrouch();
}

void APlayerCharacter::LButtonAction_Implementation(bool bPress)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (!m_Equipment || !m_EquipmentInfo.Montage)
	{
		if(m_BuildEnable)
		{
			if(Building_Build())
			{
				FActorSpawnParameters param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
				ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, param);
				Effect->LoadSound(TEXT("SoundWave'/Game/Sound/craft_item02.craft_item02'"));
				Effect->Play();
			}
		}
	}
	else
	{
		m_Equipment->LButtonAction(bPress);
	}
}

void APlayerCharacter::RButtonAction(bool bPress)
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (!m_Equipment || !m_EquipmentInfo.Montage)
		return;
	
	m_Equipment->RButtonAction(bPress);
}

void APlayerCharacter::Key_R_Action()
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (!m_Equipment)
		return;
	
	m_Equipment->Key_R_Action();
}

void APlayerCharacter::Key_T_Action()
{
	if (m_AnimInstance->GetAnimType() == EPlayerAnimType::Death)
		return;
	
	if (!m_Equipment)
		return;
	
	m_Equipment->Key_T_Action();
}

void APlayerCharacter::UseEquipment()
{
	if (!m_Equipment)
		return;
	
	m_Equipment->Use();
}

void APlayerCharacter::UseEndEquipment()
{
	if (!m_Equipment)
		return;
	
	m_Equipment->UseEnd();
}

void APlayerCharacter::GrabEnd()
{
	if (!m_Equipment)
		return;
	
	m_AnimInstance->Montage_SetPosition(m_EquipmentInfo.Montage, 0.f);
	m_AnimInstance->Montage_Play(m_EquipmentInfo.Montage);
	
}


void APlayerCharacter::BeginChargingEquipment()
{
	if (!m_Equipment)
		return;
	
	m_Equipment->BeginCharging();
	
}

void APlayerCharacter::EndChargingEquipment()
{
	if (!m_Equipment)
		return;
	
	m_Equipment->EndCharging();
}

void APlayerCharacter::OnEquipmentItem_Implementation(const FString& Name)
{
	if (Name.IsEmpty())
		return;
	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInst)
		return;

	const FEquipmentTableInfo* Info = GameInst->FindEquipmentInfo(Name);
	if (!Info || !Info->Montage)
		return;

	m_AnimInstance->SetOnEquip(true);
	
	m_EquipmentInfo.Name = Info->Name;
	m_EquipmentInfo.Montage = Info->Montage;
	m_EquipmentInfo.Equipment = Info->Equipment;
	
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_Equipment = GetWorld()->SpawnActor<AEquipment>(m_EquipmentInfo.Equipment, FVector::ZeroVector, FRotator::ZeroRotator, param);
	
	if (m_Equipment)
	{
		m_Equipment->OnEquip(this);
	}
	m_MainHUDWidget->SetChargingPercent(0.f);
}

void APlayerCharacter::UnEquipmentItem()
{
	if (m_Equipment && m_EquipmentInfo.Montage && m_AnimInstance->Montage_IsPlaying(m_EquipmentInfo.Montage))
	{
		m_AnimInstance->SetOnEquip(false);
		m_AnimInstance->Montage_Stop(0.f, m_EquipmentInfo.Montage);
		m_EquipmentInfo = {};
		m_Equipment->UnEquip();
		m_Equipment = nullptr;
		m_MainHUDWidget->SetChargingPercent(0.f);
	}
}

void APlayerCharacter::InteractionRaycast()
{
	m_InteractionActor = nullptr;
	
	TArray<FHitResult>	results;
	if(m_Camera)
	{
		// Raycast
		FCollisionQueryParams params(NAME_None, false, this);
		bool Hit = GetWorld()->LineTraceMultiByChannel(results, GetActorLocation() + m_RaycastOffset,
                                                       			GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist,
                                                       			ECollisionChannel::ECC_GameTraceChannel4, params);
		AItem* SelectItem = nullptr;
		UPrimitiveComponent* SelectItemCompoent = nullptr;
		
		for (FHitResult& result : results)
		{
			m_InteractionActor = result.GetActor();
				
			SelectItem = Cast<AItem>(result.GetActor());
			if (SelectItem)
			{
				SelectItemCompoent = result.GetComponent();
				break;
			}
		}

		if(nullptr == SelectItem || nullptr == SelectItemCompoent)
		{
			Hit = GetWorld()->LineTraceMultiByChannel(results, GetActorLocation() + m_RaycastOffset,
																	   GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist,
																	   ECollisionChannel::ECC_GameTraceChannel2, params);

			for (FHitResult& result : results)
			{
				//m_InteractionActor = result.GetActor();
				
				SelectItem = Cast<AItem>(result.GetActor());
				if (SelectItem)
				{
					SelectItemCompoent = result.GetComponent();
					break;
				}
			}
		}
		
		// 충돌 한게 있다면
		if(results.Num())
		{
			// 충돌 한게 Item 이라면
			if(SelectItem)
			{
				m_ItemHitComponent = SelectItemCompoent;
				// 처음 Item 충돌 이라면
				if(nullptr == m_InteractionItem)
				{
					m_InteractionItem = SelectItem;
					m_InteractionItem->OnCollisionEnter(this, SelectItemCompoent);
				}
				// 이전 충돌 Item과 현재 충돌 Item이 다른 것 이라면
				else if(m_InteractionItem != SelectItem)
				{
					m_InteractionItem->OnCollisionExit(this, SelectItemCompoent);
					m_InteractionItem = SelectItem;
					m_InteractionItem->OnCollisionEnter(this, SelectItemCompoent);
				}
				// 이전과 현재 충돌 Item이 같다면
				else if(m_InteractionItem == SelectItem)
				{
			
					m_InteractionItem->OnCollision(this, SelectItemCompoent);
				}
			}
			// 충돌 한게 Item이 아니라면
			else
			{
				// 이전에 충돌한 Item이 있다면
				if(nullptr != m_InteractionItem)
				{
					m_InteractionItem->OnCollisionExit(this, m_ItemHitComponent);
					m_InteractionItem = nullptr;
					m_ItemHitComponent = nullptr;
				}
			}
		}
		// 충돌 하지 않았더라면
		else
		{
			//m_InteractionActor = nullptr;
			// 이전에 충돌한 Item이 있다면
			if(nullptr != m_InteractionItem)
			{
				m_InteractionItem->OnCollisionExit(this, m_ItemHitComponent);
				m_InteractionItem = nullptr;
				m_ItemHitComponent = nullptr;
			}
		}
	}
}

void APlayerCharacter::Interaction()
{
	if(m_InteractionItem && m_ItemHitComponent)
	{
		m_InteractionItem->OnInteraction(this, m_ItemHitComponent);
	}
	else if (m_Equipment)
	{
		m_Equipment->Key_E_Action();
	}
	
}

void APlayerCharacter::ItemSlotLeft()
{
	m_MainHUDWidget->MoveQuickItemSlotSelect(true);
}

void APlayerCharacter::ItemSlotRight()
{
	m_MainHUDWidget->MoveQuickItemSlotSelect(false);
}

void APlayerCharacter::ShowMainHUD()
{
	m_MainHUDWidget->EnableWidgetFromTapKey();
}

void APlayerCharacter::SetItemInteractionUIText(bool SlateVisibilty, const FString& InteractionButtonText,
	const FString& ItemDescText)
{
	if(nullptr == m_MainHUDWidget) { return; }

	m_MainHUDWidget->SetItemInteractionUIText(SlateVisibilty,InteractionButtonText, ItemDescText);
}


bool APlayerCharacter::Building_Build()
{
	if(false == m_BuildEnable)	{ return false; }
	
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return false; }
	m_NewBuildingInfo = GameInst->FindBuildingInfo(m_BuildingName);
	if(nullptr == m_NewBuildingInfo) { return false; }

	LOG(TEXT("Sound Test"));
	LOG(TEXT("Build : %s"), *m_NewBuildingInfo->Name);
	
	ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
	if(BoatActor)
	{
		if(m_NewBuildingInfo->BuildType == EBuildType::Bottom || m_NewBuildingInfo->BuildType == EBuildType::Floor)
		{
			FHitResult	result;
			if(m_Camera)
			{
				FCollisionQueryParams params(NAME_None, false, this);
				bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation() + m_RaycastOffset,
					GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist, ECollisionChannel::ECC_GameTraceChannel2, params);
				if(Hit)
				{
					ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
					if(SelectBuilding)
					{
						return BoatActor->SelectBuildFloor(SelectBuilding, this);
					}
				}
			}
		}
		else
		{
			return BoatActor->SelectBuildInstall(this);
		}
	}

	return false;
}


bool APlayerCharacter::Building_Break()
{
	if(m_BuildEnable)
	{
		ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
	
		if(BoatActor)
		{
			FHitResult	result;
			if(m_Camera)
			{
				FCollisionQueryParams params(NAME_None, false, this);
				bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation() + m_RaycastOffset,
					GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist, ECollisionChannel::ECC_GameTraceChannel2, params);

				if(Hit)
				{
					ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
					if(SelectBuilding)
					{
						if(SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Bottom || SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Floor)
						{
							return BoatActor->SelectBreakFloor(SelectBuilding, this);
						}
						else
						{
							return BoatActor->SelectBreakInstall(SelectBuilding, this);
						}
					}
				}
			}
		}		
	}

	return false;
}

void APlayerCharacter::SetBuildInfo(const FString& Name)
{
	m_BuildingName = Name;
	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }
	m_NewBuildingInfo = GameInst->FindBuildingInfo(m_BuildingName);
}

bool APlayerCharacter::RaycastFromCamera(FHitResult& OutHit)
{
	FCollisionQueryParams params(NAME_None, false, this);
	return GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation() + m_RaycastOffset,
		GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist, ECollisionChannel::ECC_GameTraceChannel2, params);
}

FVector APlayerCharacter::GetRaycastLocation()
{
	return GetActorLocation() + m_RaycastOffset + m_Camera->GetForwardVector() * m_RaycastDist;
}

int32 APlayerCharacter::GetBuildingRotationIdx()
{
	if (nullptr == m_Equipment) { return 0;}
	
	AToolHammer* Hammer = Cast<AToolHammer>(m_Equipment);
	if(nullptr == Hammer) { return 0; }

	return Hammer->GetBuildingRotationIdx();
}

void APlayerCharacter::SetBuildEnable(bool Enable)
{
	m_BuildEnable = Enable;
	ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
	
	if(BoatActor)
	{
		BoatActor->BuildEnable(m_BuildEnable, this);
	}
}

void APlayerCharacter::BuildCompleted(const FString& ItemName)
{
	m_MainHUDWidget->BuildCompleted(ItemName);
	//SetBuildName("");
}

void APlayerCharacter::HeadBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWaterBodyOcean* Ocean = Cast<AWaterBodyOcean>(OtherActor))
	{
		WaterIn();
	}
}

void APlayerCharacter::HeadEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AWaterBodyOcean* Ocean = Cast<AWaterBodyOcean>(OtherActor))
	{
		WaterOut();
	}
}

void APlayerCharacter::WaterIn()
{
	m_PlayerInfo.bBreathAble = false;
	m_MainHUDWidget->SetBreathBarVisible(ESlateVisibility::Visible);
}

void APlayerCharacter::WaterOut()
{
	m_PlayerInfo.bBreathAble = true;
}

void APlayerCharacter::BreathRecoveryStart()
{
	m_bBreathRecovery = true;
	m_bBreathLow = false;
	m_AudioComponent->PlayDataTableSound(TEXT("BreathRecovery"));
}

void APlayerCharacter::BreathRecoveryEnd()
{
	m_PlayerInfo.Breath = 0.f;
	m_MainHUDWidget->SetBreathBarVisible(ESlateVisibility::Hidden);
}

void APlayerCharacter::Thirsty()
{
	m_bThirsty = true;
	m_AudioComponent->PlayDataTableSound(TEXT("Thirsty"));
}

void APlayerCharacter::ThirstyRecovery()
{

	m_bThirsty = false;
	m_AudioComponent->PlayDataTableSound(TEXT("ThirstyRecovery"));
}

void APlayerCharacter::Hungry()
{
	m_bHungry = true;
	
	// PlaySound(TEXT("Hungry"));
}

void APlayerCharacter::HungryRecovery()
{
	m_bHungry = false;
	
	// PlaySound(TEXT("HungryRecovery"));
}

void APlayerCharacter::Respawn()
{
	GetCharacterMovement()->Buoyancy = 1.f;
	m_RespawnAccTime = 0.f;
	m_AnimInstance->SetAnimType(EPlayerAnimType::Ground);
	m_PlayerInfo.HP = m_PlayerInfo.HPMax;
	m_PlayerInfo.Hungry = 0;
	m_PlayerInfo.Thirsty = 0;
	m_PlayerInfo.Breath = 0.f;
	SetActorLocation(FVector(-5050.f, -2307.f,  275.f));
}

void APlayerCharacter::Death()
{
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 150.f);
	GetCharacterMovement()->Buoyancy = 1.5f;
	m_PlayerInfo.Hungry = m_PlayerInfo.HungryMax;
	m_PlayerInfo.Thirsty = m_PlayerInfo.ThirstyMax;
	m_PlayerInfo.Breath = m_PlayerInfo.BreathMax;
	m_PlayerInfo.HP = 0.f;
	m_AnimInstance->SetAnimType(EPlayerAnimType::Death);
}

void APlayerCharacter::Swim()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, param);
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/swim01.swim01'"));
	Effect->Play();
}

void APlayerCharacter::BreathLow()
{
	m_bBreathLow = true;
	m_AudioComponent->PlayDataTableSound(TEXT("BreathLow"));
}

void APlayerCharacter::Drowning()
{
	m_AudioComponent->PlayDataTableSound(TEXT("Drowning"));
}


