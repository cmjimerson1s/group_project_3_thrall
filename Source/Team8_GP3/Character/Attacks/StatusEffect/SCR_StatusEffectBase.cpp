// SCR_StatusEffectBase.cpp
#include "SCR_StatusEffectBase.h"

void USCR_StatusEffectBase::ApplyEffect_Implementation(AActor* Target, AActor* Instigator, const FHitResult& Hit)
{
	if (!Target) return;
    
	if (!bIsInfinite)
	{
		StartDurationTimer(Target);
	}
}

void USCR_StatusEffectBase::RemoveEffect_Implementation(AActor* Target)
{
	if (!Target) return;
    
	Target->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EffectDuration);
}

void USCR_StatusEffectBase::StartDurationTimer(AActor* Target)
{
	if (!Target) return;
    
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &USCR_StatusEffectBase::RemoveEffect, Target);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectDuration, Delegate, Duration, false);
}