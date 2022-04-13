// Fill out your copyright notice in the Description page of Project Settings.


#include "Shark.h"
#include "SharkAIController.h"
#include "../../Effect/Effect.h"


AShark::AShark()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelAsset(TEXT("SkeletalMesh'/Game/Unit/Monster/Shark/Mesh/SK_Shark.SK_Shark'"));
	
	if (SkelAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkelAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Unit/Monster/Shark/ABP_Shark.ABP_Shark_C'"));
	
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	
	m_Mouth = CreateDefaultSubobject<USphereComponent>(TEXT("Mouth"));
	m_Mouth->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Mouth->SetupAttachment(GetRootComponent());

	GetMesh()->SetMassScale(NAME_None, 700.f);
		
	AIControllerClass = ASharkAIController::StaticClass();
	m_MonsterInfoName = TEXT("Shark");
}

void AShark::BeginPlay()
{
	Super::BeginPlay();
	m_Mouth->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

float AShark::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	m_Mouth->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_AudioComponent->PlayDataTableSound(TEXT("Damage"));
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AShark::NormalAttack()
{
	Super::NormalAttack();
	
	FVector SharkLoc = GetActorLocation();

	FCollisionQueryParams params(NAME_None, false, this);

	FHitResult result;
	
	bool Sweep = GetWorld()->SweepSingleByChannel(result
		, SharkLoc
		, SharkLoc + GetActorForwardVector() * m_MonsterInfo.AttackRange
		, FQuat::Identity
		, ECollisionChannel::ECC_GameTraceChannel7
		, FCollisionShape::MakeSphere(m_MonsterInfo.AttackRange), params);

	FHitResult CollisionArray = {};

	if (Sweep)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEffect* Effect = GetWorld()->SpawnActor<AEffect>(AEffect::StaticClass(), result.ImpactPoint, result.ImpactNormal.Rotation(), param);
		
		Effect->LoadParticle(TEXT("ParticleSystem'/Game/Effect/FXVillesBloodVFXPack/Particles/Cascade/Real/P_Blood_Spurt_02.P_Blood_Spurt_02'"));
		Effect->SetActorScale3D(FVector(3.f, 3.f, 3.f));
		
		FDamageEvent DmgEvent = {};
		result.GetActor()->TakeDamage(m_MonsterInfo.Attack, DmgEvent, GetController(), this);
	}

	m_AudioComponent->PlayDataTableSound(TEXT("NormalAttack"));
}

void AShark::OpenMouth()
{
	m_Mouth->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AShark::Dash()
{
	m_Mouth->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 50.f * GetMesh()->GetMass());
}

void AShark::Swim()
{
	m_AudioComponent->PlayDataTableSound(TEXT("Swim"));
}

