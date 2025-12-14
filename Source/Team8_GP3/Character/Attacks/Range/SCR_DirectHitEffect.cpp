// SCR_DirectHitEffect.cpp
#include "SCR_DirectHitEffect.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Engine/Engine.h"
#include "Team8_GP3/Character/Player/SCR_Possession_Component.h"

void USCR_DirectHitEffect::ApplyEffect_Implementation(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner)
{
	Super::ApplyEffect_Implementation(HitActor, Hit, ProjectileOwner);
	if (!HitActor) return;

	// Apply damage
	if (USCR_HealthComponent* HealthComp = HitActor->FindComponentByClass<USCR_HealthComponent>())
	{
		bool IsEnvironment = false;
		bool IsPlayer = false;
		if (HealthComp->PossessionComponent.IsValid())
		{
			IsPlayer = HealthComp->PossessionComponent->bIsPlayer;
		}
		else
		{
			IsEnvironment = true;
		}
		
		if ((!IsEnvironment && ((IsPlayer && DamagePlayer) || (!IsPlayer && DamageEnemy))) || IsEnvironment)
		{
			HealthComp->DamageHealth(DamageAmount);
		}
		else
		{
			return; // Skip damage application if type mismatch
		}
	}

	if (StatusEffectToApply)
	{
		USCR_StatusEffectBase* Effect = NewObject<USCR_StatusEffectBase>(this, StatusEffectToApply);
		if (Effect)
		{
			Effect->ApplyEffect(HitActor, ProjectileOwner, Hit);
		}
	}
}
