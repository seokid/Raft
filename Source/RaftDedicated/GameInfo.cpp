#include "GameInfo.h"

DEFINE_LOG_CATEGORY(RaftDedicated);


void PrintViewport(float Time, const FColor& Color, const FString& Text)
{
	GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);	//-1 : 순서상관없이나온다
}