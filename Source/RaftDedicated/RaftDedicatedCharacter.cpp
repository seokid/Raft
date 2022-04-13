// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaftDedicatedCharacter.h"

#include "GameInfo.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ToolBuilderUtil.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "RaftDedicatedGameInstance.h"
#include "Item/Building/Boat.h"
#include "Item/Item.h"
#include "Item/Building/BlockFoundation.h"
#include "UI/Inventory.h"
#include "UI/MainHUD.h"
#include "UI/ItemInteraction.h"
#include "UI/ItemSlot.h"

//////////////////////////////////////////////////////////////////////////
// ARaftDedicatedCharacter

ARaftDedicatedCharacter::ARaftDedicatedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	m_BuildEnable = false;
	m_BuildingName = TEXT("Block_Foundation");
	m_BuldingInfoIdx = 0;
	m_InteractionItem = nullptr;
	m_ItemHitComponent = nullptr;

	m_RaycastDist = 300.f;

	static ConstructorHelpers::FClassFinder<UMainHUD> MainHUDAsset(TEXT("WidgetBlueprint'/Game/UI/MainHUD/UI_MainHUD.UI_MainHUD_C'"));

	if (MainHUDAsset.Succeeded())
	{
		m_MainHUDClass = MainHUDAsset.Class;
	}

	m_MainHUDWidget = nullptr;
}

void ARaftDedicatedCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(m_MainHUDClass))
	{
		m_MainHUDWidget = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));

		if (m_MainHUDWidget)
		{
			m_MainHUDWidget->AddToViewport();

			// int count1 = 30;
			// m_MainHUDWidget->AddItem(TEXT("Plank"), count1);

			// int count2 = 30;
			// m_MainHUDWidget->AddItem(TEXT("Thatch"), count2);
			//
			// int count3 = 30;
			// m_MainHUDWidget->AddItem(TEXT("Plastic"), count3);
		}
	}
}

void ARaftDedicatedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionRaycast();
}

//////////////////////////////////////////////////////////////////////////
// Input

bool ARaftDedicatedCharacter::RaycastFromCamera(struct FHitResult& OutHit)
{
	FCollisionQueryParams params(NAME_None, false, this);
	return GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation() + FVector(0.f, 0.f, 30.f),
		GetActorLocation() + FVector(0.f, 0.f, 30.f) + GetFollowCamera()->GetForwardVector() * 300.f, ECollisionChannel::ECC_GameTraceChannel2, params);
}

void ARaftDedicatedCharacter::SetItemInteractionUIText(bool SlateVisibilty,
	const FString& InteractionButtonText, const FString& ItemDescText)
{
	if(nullptr == m_MainHUDWidget) { return; }

	m_MainHUDWidget->SetItemInteractionUIText(SlateVisibilty,InteractionButtonText, ItemDescText);
}

void ARaftDedicatedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARaftDedicatedCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARaftDedicatedCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARaftDedicatedCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARaftDedicatedCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARaftDedicatedCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARaftDedicatedCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARaftDedicatedCharacter::OnResetVR);

	// Build
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &ARaftDedicatedCharacter::Build);
	PlayerInputComponent->BindAction("Break", IE_Pressed, this, &ARaftDedicatedCharacter::BreakBlock);
	PlayerInputComponent->BindAction("BuildEnable", IE_Pressed, this, &ARaftDedicatedCharacter::BuildEnable);
	PlayerInputComponent->BindAction("ChangeBuildInfo", IE_Pressed, this, &ARaftDedicatedCharacter::ChangeBuildInfo);
	
	PlayerInputComponent->BindAction("BuildingRoatation", IE_Pressed, this, &ARaftDedicatedCharacter::BuildingRotation);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ARaftDedicatedCharacter::Interaction);

	// Item Slot
	PlayerInputComponent->BindAction("ItemSlotLeft", IE_Pressed, this, &ARaftDedicatedCharacter::ItemSlotLeft);
	PlayerInputComponent->BindAction("ItemSlotRight", IE_Pressed, this, &ARaftDedicatedCharacter::ItemSlotRight);

	// Inventory, NotebookCrafting
	PlayerInputComponent->BindAction("Tap", IE_Pressed, this, &ARaftDedicatedCharacter::ShowInventory);
}

void ARaftDedicatedCharacter::Build()
{
//	m_MainHUDWidget->GetInventoryWidget()->AddItem(TEXT("Plank"), 2);

	
	
// 	if(m_BuildEnable)
// 	{
// 		PrintViewport(2.f, FColor::Red, TEXT("Build!"));
//
// 		ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
// 	
// 		if(BoatActor)
// 		{
// 			if(m_BuildType == EBuildType::Bottom || m_BuildType == EBuildType::Floor)
// 			{
// 				FHitResult	result;
// 				if(FollowCamera)
// 				{
// 					FCollisionQueryParams params(NAME_None, false, this);
// 					bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation() + FVector(0.f, 0.f, 30.f),
// 						GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * 300.f, ECollisionChannel::ECC_GameTraceChannel2, params);
// 					if(Hit)
// 					{
// 						PrintViewport(2.f, FColor::Red, result.GetActor()->GetName());
// 						ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
// 						if(SelectBuilding)
// 						{
// 							BoatActor->SelectBuildFloor(SelectBuilding, this);
// 						}
// 					}
//
// #if ENABLE_DRAW_DEBUG
// 					FColor DrawColor = Hit ? FColor::Red : FColor::Green;
// 					DrawDebugLine(GetWorld(),
// 						GetActorLocation() + FVector(0.f, 0.f, 30.f),
// 						GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * 300.f,
// 						DrawColor, false, 1.f);
// #endif
// 				}
// 			}
// 			else
// 			{
// 				BoatActor->SelectBuildInstall(this);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		
// #if ENABLE_DRAW_DEBUG
// 		FColor DrawColor = false ? FColor::Red : FColor::Green;
// 		DrawDebugLine(GetWorld(),
// 			GetActorLocation() + FVector(0.f, 0.f, 30.f),
// 			GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * 300.f,
// 			DrawColor, false, 1.f);
// #endif
// 		
// 		
// 	}
}

void ARaftDedicatedCharacter::BreakBlock()
{
	
// 	
// 	if(m_BuildEnable)
// 	{
// 		PrintViewport(2.f, FColor::Red, TEXT("Break!"));
// 		ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
// 	
// 		if(BoatActor)
// 		{
// 			FHitResult	result;
// 			if(FollowCamera)
// 			{
// 				FCollisionQueryParams params(NAME_None, false, this);
// 				bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation() + FVector(0.f, 0.f, 30.f),
// 					GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * 300.f, ECollisionChannel::ECC_GameTraceChannel2, params);
//
// 				if(Hit)
// 				{
// 					PrintViewport(2.f, FColor::Red, result.GetActor()->GetName());
// 					ABuilding* SelectBuilding = Cast<ABuilding>(result.GetActor());
// 					if(SelectBuilding)
// 					{
// 						if(SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Bottom || SelectBuilding->GetBuildingInfo().m_BuildType == EBuildType::Floor)
// 						{
// 							BoatActor->SelectBreakFloor(SelectBuilding, this);
// 						}
// 						else
// 						{
// 							BoatActor->SelectBreakInstall(SelectBuilding, this);
// 						}
// 					}
// 				}
//
// #if ENABLE_DRAW_DEBUG
// 				FColor DrawColor = Hit ? FColor::Red : FColor::Green;
// 				DrawDebugLine(GetWorld(),
// 					GetActorLocation() + FVector(0.f, 0.f, 30.f),
// 					GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * 300.f,
// 					DrawColor, false, 1.f);
// #endif
// 			}
// 		}		
// 	}
}

void ARaftDedicatedCharacter::BuildEnable()
{
	// m_BuildEnable = !m_BuildEnable;
	// ABoat* BoatActor = Cast<ABoat>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoat::StaticClass()));
	//
	// if(BoatActor)
	// {
	// 	BoatActor->BuildEnable(m_BuildEnable, this);
	// 	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	// 	if(nullptr == GameInst) { return; }
	// 	const FBuildingTableInfo* NewBuildingClassInfo = GameInst->FindBuildingInfo(m_BuildingName);
	// 	if(nullptr == NewBuildingClassInfo) {return;}
	// 	//BoatActor->SetBuildSourceMesh(NewBuildingClassInfo->SourceMesh);
	// 	//BoatActor->SetBuildingClassInfo(NewBuildingClassInfo);
	//
	// 	m_NewbuildingClassInfo = NewBuildingClassInfo;
	// }
	//
	// if(false == m_BuildEnable)
	// {
	// 	n_BuildingRotationIdx = 0;
	// }
	//
	// if(m_BuildEnable)
	// {
	// 	int32 count = 15;
	// 	m_MainHUDWidget->AddItem(TEXT("Plank"), count);
	// }
	// else
	// {
	// 	int32 count = 17;
	// 	m_MainHUDWidget->AddItem(TEXT("Thatch"), count);
	// }
}

void ARaftDedicatedCharacter::ChangeBuildInfo()
{
	if(false == m_BuildEnable) {return;}
	++m_BuldingInfoIdx;
	m_BuldingInfoIdx %= 8;
	switch(m_BuldingInfoIdx)
	{
	case 0:
		m_BuildingName = TEXT("Block_Foundation");
		break;
	case 1:
		m_BuildingName = TEXT("Item_Net");
		break;
	case 2:
		m_BuildingName = TEXT("Block_Stair_Wood");
		break;
	case 3:
		m_BuildingName = TEXT("Block_Floor_Wood");
		break;
	case 4:
		m_BuildingName = TEXT("Block_Floor_Wood_Half");
		break;
	case 5:
		m_BuildingName = TEXT("Block_Pillar_Wood");
		break;
	case 6:
		m_BuildingName = TEXT("Block_Pillar_Wood_Half");
		break;
	case 7:
		m_BuildingName = TEXT("STorage");
		break;
	}

	URaftDedicatedGameInstance* GameInst = Cast<URaftDedicatedGameInstance>(GetGameInstance());
	if(nullptr == GameInst) { return; }
	const FBuildingTableInfo* NewBuildingClassInfo = GameInst->FindBuildingInfo(m_BuildingName);
	if(nullptr == NewBuildingClassInfo) {return;}
	m_BuildType = NewBuildingClassInfo->BuildType;
	m_NewbuildingClassInfo = NewBuildingClassInfo;
}




void ARaftDedicatedCharacter::BuildingRotation()
{
	// 작업 망치에서 할일
	if(m_BuildEnable)
	{
		n_BuildingRotationIdx = (n_BuildingRotationIdx + 1) % 4;
	}
}



void ARaftDedicatedCharacter::Interaction()
{
	// if(m_InteractionItem && m_ItemHitComponent)
	// {
	// 	m_InteractionItem->OnInteraction(this, m_ItemHitComponent);
	// }
}

void ARaftDedicatedCharacter::InteractionRaycast()
{
	// FHitResult	result;
	// if(FollowCamera)
	// {
	// 	// Raycast
	// 	FCollisionQueryParams params(NAME_None, false, this);
	// 	bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation() + FVector(0.f, 0.f, 30.f),
	// 		GetActorLocation() + FVector(0.f, 0.f, 30.f) + FollowCamera->GetForwardVector() * m_RaycastDist,
	// 		ECollisionChannel::ECC_GameTraceChannel4, params);
	//
	// 	// 충돌 한게 있다면
	// 	if(Hit)
	// 	{
	// 		AItem* SelectItem = Cast<AItem>(result.GetActor());
	// 		// 충돌 한게 Item 이라면
	// 		if(SelectItem)
	// 		{
	// 			// 처음 Item 충돌 이라면
	// 			if(nullptr == m_InteractionItem)
	// 			{
	// 				m_InteractionItem = SelectItem;
	// 				m_ItemHitComponent = result.GetComponent();
	// 				m_InteractionItem->OnCollisionEnter(this, result.GetComponent());
	// 			}
	// 			// 이전 충돌 Item과 현재 충돌 Item이 다른 것 이라면
	// 			else if(m_InteractionItem != SelectItem)
	// 			{
	// 				m_InteractionItem->OnCollisionExit(this, result.GetComponent());
	// 				m_InteractionItem = SelectItem;
	// 				m_ItemHitComponent = result.GetComponent();
	// 				m_InteractionItem->OnCollisionEnter(this, result.GetComponent());
	// 			}
	// 			// 이전과 현재 충돌 Item이 같다면
	// 			else if(m_InteractionItem == SelectItem)
	// 			{
	// 				m_InteractionItem->OnCollision(this, result.GetComponent());
	// 			}
	// 		}
	// 		// 충돌 한게 Item이 아니라면
	// 		else
	// 		{
	// 			// 이전에 충돌한 Item이 있다면
	// 			if(nullptr != m_InteractionItem)
	// 			{
	// 				m_InteractionItem->OnCollisionExit(this, m_ItemHitComponent);
	// 				m_InteractionItem = nullptr;
	// 				m_ItemHitComponent = nullptr;
	// 			}
	// 		}
	// 	}
	// 	// 충돌 하지 않았더라면
	// 	else
	// 	{
	// 		// 이전에 충돌한 Item이 있다면
	// 		if(nullptr != m_InteractionItem)
	// 		{
	// 			m_InteractionItem->OnCollisionExit(this, m_ItemHitComponent);
	// 			m_InteractionItem = nullptr;
	// 			m_ItemHitComponent = nullptr;
	// 		}
	// 	}
	// }
}

void ARaftDedicatedCharacter::ItemSlotLeft()
{
	m_MainHUDWidget->MoveQuickItemSlotSelect(true);
	LOG(TEXT("Left"));
}

void ARaftDedicatedCharacter::ItemSlotRight()
{
	m_MainHUDWidget->MoveQuickItemSlotSelect(false);
	LOG(TEXT("Right"));
}

void ARaftDedicatedCharacter::ShowInventory()
{
	m_MainHUDWidget->EnableWidgetFromTapKey();
	LOG(TEXT("Show Inventory"));
}


void ARaftDedicatedCharacter::OnResetVR()
{
	// If RaftDedicated is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in RaftDedicated.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARaftDedicatedCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARaftDedicatedCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARaftDedicatedCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARaftDedicatedCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARaftDedicatedCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARaftDedicatedCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

