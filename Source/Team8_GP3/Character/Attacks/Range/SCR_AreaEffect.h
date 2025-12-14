#pragma once

#include "CoreMinimal.h"
#include "SCR_ProjectileEffectBase.h"
#include "Team8_GP3/Character/Attacks/StatusEffect/SCR_StatusEffectBase.h"
#include "UObject/Object.h"
#include "SCR_AreaEffect.generated.h"

class USCR_HealthComponent;

UENUM(BlueprintType)
enum class DropOff:uint8
{
	None,
	Linear,
	Exponential,
	Logarithmic
};

UCLASS(Blueprintable, BlueprintType)
class TEAM8_GP3_API USCR_AreaEffect : public USCR_ProjectileEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	float Radius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	float DamageAmountAtCenter = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	float DamageAmountAtEdge = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	float DirectHitDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	DropOff DamageFallOff = DropOff::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	float FallOffExponent = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Effect")
	TSubclassOf<USCR_StatusEffectBase> StatusEffectToApply;
	
	virtual void ApplyEffect_Implementation(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner) override;

	float CalculateDamage(AActor* HitActor, FVector AreaCenterLocation);
	float CalculateLinearDamage(float Distance);
	float CalculateExponentialDamage(float Distance);
	float CalculateLogarithmicDamage(float Distance);
};