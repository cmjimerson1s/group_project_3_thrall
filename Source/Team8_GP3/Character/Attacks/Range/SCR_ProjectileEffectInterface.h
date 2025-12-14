#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SCR_ProjectileEffectInterface.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class USCR_ProjectileEffectInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM8_GP3_API ISCR_ProjectileEffectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile Effect")
	void ApplyEffect(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner);
};