// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect.h"

// Sets default values
AEffect::AEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SetRootComponent(m_ParticleSystem);
	m_bLoopEffect = false;
}

void AEffect::LoadParticle(const FString& Path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), *Path);		//런타임 객체 만들떄사용 주로 에셋만들때 사용한다
	if (Particle)
	{
		m_ParticleSystem->SetTemplate(Particle);
	}
}

void AEffect::LoadSound(const FString& Path)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);

	if (Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
}

// Called when the game starts or when spawned
void AEffect::BeginPlay()
{
	Super::BeginPlay();

	if (!m_bLoopEffect)
		m_ParticleSystem->OnSystemFinished.AddDynamic(this, &AEffect::ParticleFinish);
}

// Called every frame
void AEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffect::ParticleFinish(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

