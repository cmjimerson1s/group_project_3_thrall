#include "SCR_Interactable.h"
#include "GameFramework/Actor.h"

USCR_Interactable::USCR_Interactable()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USCR_Interactable::BeginPlay()
{
	Super::BeginPlay();
}

void USCR_Interactable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USCR_Interactable::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacted with: %s"), *GetOwner()->GetName());
        OnInteract.Broadcast();
}
