// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableAudioComponent.h"

UDataTableAudioComponent::UDataTableAudioComponent()
{
	m_SoundInfoTable = nullptr;
}

void UDataTableAudioComponent::PlayDataTableSound(const FString& Name, int32 Idx)
{
	if (m_SoundInfoTable)
	{
		if (FSoundTableInfo* Info = m_SoundInfoTable->FindRow<FSoundTableInfo>(*Name, ""))
		{
			
			// LOG(TEXT("PlaySound : %s - %s"), *GetOwner()->GetName(), *Name);
			
			if (0 > Idx || Idx >= Info->Sound.Num())
			{
				Idx = Info->Index;
				Info->Index = (Info->Index + 1) % Info->Sound.Num();
			}
			Super::SetSound(Info->Sound[Idx]);
			Super::Play();
			
		}	
	}
}
