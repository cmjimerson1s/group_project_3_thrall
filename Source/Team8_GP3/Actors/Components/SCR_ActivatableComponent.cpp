#include "SCR_ActivatableComponent.h"
#include "GameFramework/Actor.h"

USCR_ActivatableComponent::USCR_ActivatableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USCR_ActivatableComponent::SetEnabled(bool enabled)
{
    if (!Owner)
    {
        Owner = GetOwner();
        if (!Owner)
        {
            UE_LOG(LogTemp,Warning,TEXT("Was not able to find Owner"));
            return false;
        }
    }
    
    bIsEnabled = enabled;
    Owner->SetActorHiddenInGame(!enabled);
    Owner->SetActorEnableCollision(enabled);
    Owner->SetActorTickEnabled(enabled);
    if (enabled) OnEnable.Broadcast();
    else OnDisable.Broadcast();
    return true;
}

void USCR_ActivatableComponent::Enable()
{
    SetEnabled(true);
}

void USCR_ActivatableComponent::Disable()
{
    SetEnabled(false);
}

void USCR_ActivatableComponent::Toggle()
{
    SetEnabled(!bIsEnabled);
}
