// SCR_StatusEffectBase.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SCR_StatusEffectBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class TEAM8_GP3_API USCR_StatusEffectBase : public UObject
{
	GENERATED_BODY()

public:
	// Configurable properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float Duration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	bool bIsInfinite = false;

	// Main effect functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Effect")
	void ApplyEffect(AActor* Target, AActor* Instigator, const FHitResult& Hit);
	virtual void ApplyEffect_Implementation(AActor* Target, AActor* Instigator, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Effect")
	void RemoveEffect(AActor* Target);
	virtual void RemoveEffect_Implementation(AActor* Target);

protected:
	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void StartDurationTimer(AActor* Target);

private:
	FTimerHandle TimerHandle_EffectDuration;
};