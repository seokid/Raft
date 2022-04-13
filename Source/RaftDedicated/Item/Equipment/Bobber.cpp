// Fill out your copyright notice in the Description page of Project Settings.


#include "Bobber.h"
#include "../../Effect/NormalSound.h"
#include "BuoyancyComponent.h"

ABobber::ABobber()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bobber"));
	StaticMesh->SetCollisionProfileName(TEXT("Equipment"), false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RodAsset(TEXT("StaticMesh'/Game/Item/Equipment/Weapon/Mesh/SM_Bobber.SM_Bobber'"));
	if (RodAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(RodAsset.Object);
	}
	m_Mesh = StaticMesh;
	m_Mesh->SetLinearDamping(0.5f);
	
	SetRootComponent(m_Mesh);
	
	m_Mesh->SetMassOverrideInKg(NAME_None, 30.f);
	
	m_Buoynacy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Bouyancy"));
	m_Buoynacy->AddCustomPontoon(50.f, TEXT("Head"));
}

void ABobber::BeginPlay()
{
	Super::BeginPlay();

	m_Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABobber::BobberBeginOverlap);
	m_Mesh->OnComponentEndOverlap.AddDynamic(this, &ABobber::BobberEndOverlap);
}


void ABobber::AttachToMove(UMeshComponent* Mesh, float OverTime, const FVector& Location, const FRotator& Rotation, FName SocketName)
{
	m_Mesh->SetSimulatePhysics(false);
	AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(m_Mesh, Location, Rotation, true, true, OverTime, true, EMoveComponentAction::Move, Info);
}

void ABobber::Throw(FVector Velocity)
{

	m_Mesh->SetSimulatePhysics(true);
	m_Mesh->AddImpulse(Velocity * m_Mesh->GetBodyInstance()->GetMassOverride(), TEXT("Head"));
}

void ABobber::BobberBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ANormalSound* Effect = GetWorld()->SpawnActor<ANormalSound>(ANormalSound::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, param);
	Effect->LoadSound(TEXT("SoundWave'/Game/Sound/watersplash_small02.watersplash_small02'"));
	Effect->Play();	
	
}

void ABobber::BobberEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}
