#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SCR_ProjectileEffectBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class TEAM8_GP3_API USCR_ProjectileEffectBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Effect")
	bool DamagePlayer = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Effect")
	bool DamageEnemy = true;
	
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile Effect")
	void ApplyEffect(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner);
	virtual void ApplyEffect_Implementation(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner) {}
};