// SCR_AreaEffect.cpp
#include "SCR_AreaEffect.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "FrameTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_GP3/Character/Player/SCR_Possession_Component.h"

void USCR_AreaEffect::ApplyEffect_Implementation(AActor* HitActor, const FHitResult& Hit, AActor* ProjectileOwner)
{
	Super::ApplyEffect_Implementation(HitActor, Hit, ProjectileOwner);

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(ProjectileOwner);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoredActors);

	FVector HitLocation = Hit.Location;

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		HitLocation,
		HitLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);
	

	for (const FHitResult& AreaHit : OutHits)
	{
		AActor* AffectedActor = AreaHit.GetActor();
		if (!AffectedActor) continue;

		float DamageAmount = 0.0f;

		if (AffectedActor == HitActor)
		{
			DamageAmount = DirectHitDamage;
		}
		else
		{
			DamageAmount = CalculateDamage(AffectedActor, HitLocation);
		}

		
		// Apply damage
		if (USCR_HealthComponent* HealthComp = AffectedActor->FindComponentByClass<USCR_HealthComponent>())
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

		// Apply status effect if specified
		if (StatusEffectToApply)
		{
			USCR_StatusEffectBase* Effect = NewObject<USCR_StatusEffectBase>(this, StatusEffectToApply);
			if (Effect)
			{
				Effect->ApplyEffect(AffectedActor, ProjectileOwner, AreaHit);
			}
		}
	}

	// Debug visualization
	DrawDebugSphere(GetWorld(), Hit.Location, Radius, 16, FColor::Red, false, 2.0f);
}

float USCR_AreaEffect::CalculateDamage(AActor* HitActor, FVector AreaCenterLocation)
{
	if (!HitActor) return 0.0f;

	FVector HitLocation = HitActor->GetActorLocation();

	float Distance = FVector::Distance(HitLocation, AreaCenterLocation);

	if (Distance > Radius)
		return DamageAmountAtEdge;

	switch (DamageFallOff)
	{
	case DropOff::None:
		return DamageAmountAtCenter;
	case DropOff::Linear:
		return CalculateLinearDamage(Distance);
	case DropOff::Exponential:
		return CalculateExponentialDamage(Distance);
	case DropOff::Logarithmic:
		return CalculateLogarithmicDamage(Distance);
	default:
		return 0.0f;
	}
}


float USCR_AreaEffect::CalculateLinearDamage(float Distance)
{
	float DistanceAsRatio = Distance / Radius;
	float Damage = (DistanceAsRatio * (DamageAmountAtCenter - DamageAmountAtEdge)) + DamageAmountAtEdge;

	return Damage;
}

float USCR_AreaEffect::CalculateExponentialDamage(float Distance)
{
	float DistanceAsRatio = Distance / Radius;
	float DamageFallOffScaler = FMath::Pow(DistanceAsRatio, FallOffExponent);
	float Damage = (DamageFallOffScaler * (DamageAmountAtCenter - DamageAmountAtEdge)) + DamageAmountAtEdge;

	return Damage;
}

float USCR_AreaEffect::CalculateLogarithmicDamage(float Distance)
{
	//TODO: Implement logarithmic damage calculation
	return 0.0f; // Placeholder for future implementation
}
