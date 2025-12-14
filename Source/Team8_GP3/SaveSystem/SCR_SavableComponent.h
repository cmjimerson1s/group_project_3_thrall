// SavableComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_SavableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_SavableComponent : public UActorComponent
{
	GENERATED_BODY()

public:    
	USCR_SavableComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Save State")
	bool bIsDestroyed = false;

public:
	UFUNCTION(BlueprintCallable, Category="Save State")
	bool IsDestroyed() const { return bIsDestroyed; }

	UFUNCTION(BlueprintCallable, Category="Save State")
	void SetDestroyed(bool bNewDestroyed) { bIsDestroyed = bNewDestroyed; }

	// Add more save-relevant properties as needed
	// UPROPERTY(EditAnywhere, Category="Save State")
	// int32 SomeCustomState;
};