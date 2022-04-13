// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "../Unit/PlayerCharacter/PlayerCharacter.h"
#include "ChaosInterfaceWrapperCore.h"
#include "RaftDedicated/UI/MainHUD.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Item"));
	
	SetRootComponent(m_RootComponent);

	m_RootComponent->ResetRelativeTransform();*/

	//m_RootComponent->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));

	//m_RootComponent->SetRelativeScale3D_Direct(FVector(100.f, 100.f, 100.f));
	
	// SetActorRelativeScale3D(FVector(100.f, 100.f, 100.f));
	//
	// SetActorScale3D(FVector(100.f, 100.f, 100.f));
	//
	// FVector scale = GetActorRelativeScale3D();
	// PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("A : %f %f %f"), scale.X, scale.Y, scale.Z));

	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// FVector scale = GetActorRelativeScale3D();
	// PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), scale.X, scale.Y, scale.Z));
	// SetActorRelativeScale3D(FVector(100.f, 100.f, 100.f));
	//
	// scale = GetActorRelativeScale3D();
	// PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), scale.X, scale.Y, scale.Z));
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AItem::OnInteraction(APlayerCharacter* Player, UPrimitiveComponent* HitComponent)
{
	// 여러번 상호작용 하더라도 번갈아 가면서 들어온다.
	if(false == m_Interaction) // 상호작용 시작
	{
		OnInteractionEnter(Player, HitComponent);
	}
	else // 상호작용 종료
	{
		OnInteractionExit(Player, HitComponent);
	}
}

void AItem::AddItemToPlayer(APlayerCharacter* Player, const TArray<FItemDropInfo>& ArrayItemDropInfo)
{
	if(nullptr == Player) { return; }

	for(int32 i = 0; i < ArrayItemDropInfo.Num(); ++i)
	{
		if(FMath::RandRange(0.f, 100.f) <= ArrayItemDropInfo[i].DropPercentage)
		{
			int32 ItemCount = FMath::RandRange(ArrayItemDropInfo[i].DropMinCount, ArrayItemDropInfo[i].DropMaxCount);
			Player->GetMainHUD()->AddItem(ArrayItemDropInfo[i].Name, ItemCount);
		}
	}
}



