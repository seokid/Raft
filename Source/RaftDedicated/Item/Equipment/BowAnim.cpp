
#include "BowAnim.h"
#include "Bow.h"

UBowAnim::UBowAnim()
{
	m_Owner = nullptr;
}

void UBowAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Owner = Cast<ABow>(GetSkelMeshComponent()->GetOwner());
}

void UBowAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
