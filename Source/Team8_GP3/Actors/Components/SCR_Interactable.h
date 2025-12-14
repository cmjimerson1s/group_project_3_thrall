#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_Interactable.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_Interactable : public UActorComponent
{
	GENERATED_BODY()

public:	
	USCR_Interactable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Interaction")
	virtual void Interact();

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractableEvent OnInteract;
};
