// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterAnim.h"

APlayerCharacterController::APlayerCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	m_Speed = 100.f;
	m_ArrKeyPress.SetNum(static_cast<int32>(EKeyInputType::End));
	
	m_EquipName = TEXT("Bow");
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
}


void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAxis(TEXT("SwimmingUp"), this, &APlayerCharacterController::SwimmingUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacterController::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacterController::LookUp);
	//InputComponent->BindAxis(TEXT("CameraZoom"), this, &APlayerCharacterController::CameraZoom);

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacterController::Jump);
	
	InputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &APlayerCharacterController::Run);
	InputComponent->BindAction(TEXT("Run"), IE_Released, this, &APlayerCharacterController::Run);

	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacterController::Crouch);
	InputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacterController::Crouch);
	
	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Pressed, this, &APlayerCharacterController::LButtonPress);
	InputComponent->BindAction(TEXT("LButton"), EInputEvent::IE_Released, this, &APlayerCharacterController::LButtonRelease);

	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Pressed, this, &APlayerCharacterController::RButton);
	InputComponent->BindAction(TEXT("RButton"), EInputEvent::IE_Released, this, &APlayerCharacterController::RButton);

	InputComponent->BindAction(TEXT("UseItem1"), EInputEvent::IE_Pressed, this, &APlayerCharacterController::UseItem1);
	InputComponent->BindAction(TEXT("UseItem2"), EInputEvent::IE_Pressed, this, &APlayerCharacterController::UseItem2);

	// E Item Interaction
	InputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayerCharacterController::Interaction);
	// R Building Rotation
	InputComponent->BindAction(TEXT("BuildingRoatation"), IE_Pressed, this, &APlayerCharacterController::Key_R);
	// T Building Change
	InputComponent->BindAction(TEXT("BuildingChange"), IE_Pressed, this, &APlayerCharacterController::Key_T);

	// Quick Item Slot List - Mouse Wheel
	InputComponent->BindAction(TEXT("ItemSlotLeft"), IE_Pressed, this, &APlayerCharacterController::ItemSlotLeft);
	InputComponent->BindAction(TEXT("ItemSlotRight"), IE_Pressed, this, &APlayerCharacterController::ItemSlotRight);

	// MainHUD Enable
	InputComponent->BindAction("Tap", IE_Pressed, this, &APlayerCharacterController::ShowMainHUD);
}

void APlayerCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}


void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// for(int32 i = 0; i < m_ArrKeyPress.Num(); ++i)
	// {
	// 	if (m_ArrKeyPress[i])
	// 	{
	// 		KeyDown(static_cast<EKeyInputType>(i));
	// 	}
	// }
}

void APlayerCharacterController::KeyDown(EKeyInputType KeyType)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		if (EKeyInputType::LButton == KeyType)
		{
			
		}
	}
	
}

void APlayerCharacterController::Interaction()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Interaction();
	}
}



void APlayerCharacterController::Key_R()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Key_R_Action();
	}
}

void APlayerCharacterController::Key_T()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Key_T_Action();
	}
}

void APlayerCharacterController::ItemSlotLeft()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->ItemSlotLeft();
	}
}

void APlayerCharacterController::ItemSlotRight()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->ItemSlotRight();
	}
}

void APlayerCharacterController::ShowMainHUD()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->ShowMainHUD();
	}
}

void APlayerCharacterController::Build()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Building_Build();
	}
}

void APlayerCharacterController::LButtonPress_Implementation()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->LButtonAction(true);
	}
	
}

void APlayerCharacterController::LButtonRelease_Implementation()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->LButtonAction(false);
	}
	
}

void APlayerCharacterController::RButton()
{
	static constexpr int32 Idx = static_cast<int32>(EKeyInputType::RButton);
	m_ArrKeyPress[Idx] = !m_ArrKeyPress[Idx]; 
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->RButtonAction(m_ArrKeyPress[Idx]);
	}
}


void APlayerCharacterController::CameraZoom(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter)
    {
    	PlayerCharacter->CameraZoom(Value);
    }
}

void APlayerCharacterController::MoveForward(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->MoveForward(Value);
	}
}

void APlayerCharacterController::MoveRight(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->MoveRight(Value);
	}
}

void APlayerCharacterController::SwimmingUp(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->SwimmingUp(Value);
	}
}

void APlayerCharacterController::Turn(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->AddControllerYawInput(Value);	
	}
}

void APlayerCharacterController::LookUp(float Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->AddControllerPitchInput(Value);	
	}
}

void APlayerCharacterController::Jump()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Jump();
	}
}

void APlayerCharacterController::Run_Implementation()
{
	static constexpr int32 Idx = static_cast<int32>(EKeyInputType::Run);
	m_ArrKeyPress[Idx] = !m_ArrKeyPress[Idx];  

	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Run(m_ArrKeyPress[Idx]);
	}
}

void APlayerCharacterController::Crouch()
{
	static constexpr int32 Idx = static_cast<int32>(EKeyInputType::Crouch);
	m_ArrKeyPress[Idx] = !m_ArrKeyPress[Idx];
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->Crouch(m_ArrKeyPress[Idx]);
	}
}

void APlayerCharacterController::UseItem1_Implementation()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnEquipmentItem(m_EquipName);
	}
}

void APlayerCharacterController::UseItem2()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->UnEquipmentItem();
	}
}



void APlayerCharacterController::PlayerControllerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                              const FHitResult& SweepResult)
{
}

void APlayerCharacterController::PlayerControllerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}