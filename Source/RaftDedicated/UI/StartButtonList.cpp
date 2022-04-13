// Fill out your copyright notice in the Description page of Project Settings.


#include "StartButtonList.h"

#include "StartHUD.h"

void UStartButtonList::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartHUDWidget = nullptr;
	m_HoveredButtonType = EStartButtonType::End;
	
	for(int32 i = 0; i < static_cast<int32>(EStartButtonType::End); ++i)
	{
		m_ArrayButton.Add(Cast<UButton>(GetWidgetFromName(*FString::Printf(TEXT("Button_%d"), i))));
		m_ArrayText.Add(Cast<UTextBlock>(GetWidgetFromName(*FString::Printf(TEXT("Text_%d"), i))));
		m_ArrayButton[i]->SetBackgroundColor(FLinearColor(FVector4(0.f, 0.f, 0.f, 0.f)));
		m_ArrayButton[i]->OnClicked.AddDynamic(this, &UStartButtonList::StartButtonClicked);
		m_ArrayButton[i]->OnHovered.AddDynamic(this, &UStartButtonList::StartButtonHovered);
		m_ArrayButton[i]->OnUnhovered.AddDynamic(this, &UStartButtonList::StartButtonUnHovered);
	}
}

void UStartButtonList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartButtonList::StartButtonClicked()
{
	ExecuteStartButtonClicked(m_HoveredButtonType);
}

void UStartButtonList::StartButtonHovered()
{
	m_HoveredButtonType = GetHoveredButtonIdx();
	ExecuteStartButtonHovered(m_HoveredButtonType);
}

void UStartButtonList::StartButtonUnHovered()
{
	ExecuteStartButtonUnHovered(m_HoveredButtonType);
	m_HoveredButtonType = EStartButtonType::End;
}

EStartButtonType UStartButtonList::GetHoveredButtonIdx()
{
	for(int32 i = 0; i < static_cast<int32>(EStartButtonType::End); ++i)
	{
		if(m_ArrayButton[i]->IsHovered())
		{
			return static_cast<EStartButtonType>(i);
		}
	}

	return EStartButtonType::End;
}

void UStartButtonList::ExecuteStartButtonClicked(EStartButtonType Type)
{
	
	switch (Type)
	{
	case EStartButtonType::GameStart:
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Raft_World"));
			break;
		}
	case EStartButtonType::Development:
		{
			if(m_StartHUDWidget)
			{
				m_StartHUDWidget->DevelopmentListEnable();

				UGameplayStatics::PlaySound2D(GetWorld(), m_ClickedSound);
			}
			break;
		}
	case EStartButtonType::GameExit:
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
			break;
		}
	default: ;
	}
}

void UStartButtonList::ExecuteStartButtonHovered(EStartButtonType Type)
{
	UGameplayStatics::PlaySound2D(GetWorld(), m_HoveredSound);
	SetTextFontOutLineColorA(Type, 1.f);
}

void UStartButtonList::ExecuteStartButtonUnHovered(EStartButtonType Type)
{
	SetTextFontOutLineColorA(Type, 0.f);
}

void UStartButtonList::SetTextFontOutLineColorA(EStartButtonType Type, float ColorA)
{
	if(EStartButtonType::End == m_HoveredButtonType) { return; }
	m_ArrayText[static_cast<int32>(Type)]->Font.OutlineSettings.OutlineColor.A = ColorA;
	m_ArrayText[static_cast<int32>(Type)]->SetFont(m_ArrayText[static_cast<int32>(Type)]->Font);
}