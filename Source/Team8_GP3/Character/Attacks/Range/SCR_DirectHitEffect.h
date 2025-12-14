#pragma once

#include "CoreMinimal.h"
#include "SCR_ProjectileEffectBase.h"
#include "Team8_GP3/Character/Attacks/StatusEffect/SCR_StatusEffectBase.h"
#include "UObject/Object.h"
#include "SCR_DirectHitEffect.generated.h"

class USCR_HealthComponent;

UCLASS(Blueprintable, BlueprintType)
class TEAM8_GP3_API USCR_DirectHitEffect : public USCR_ProjectileEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direct Hit")
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direct Hit")
	TSubclassOf<USCR_StatusEffectBase> StatusEffectToApply;

	virtual void ApplyEffect_Implementation(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner) override;
};