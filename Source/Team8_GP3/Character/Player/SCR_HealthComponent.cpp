// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_HealthComponent.h"

#include "SCR_PlayerCharacter.h"
#include "SCR_Possession_Component.h"

// Sets default values for this component's properties
USCR_HealthComponent::USCR_HealthComponent() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features 
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	// ...
}

void USCR_HealthComponent::TestKill()
{
	DamageHealth(10000);
}

void USCR_HealthComponent::DamageHealth(float Damage)
{
	if (!bInvulnerable)
	{
		CurrentHealth -= Damage;
		if (PossessionComponent.IsValid() && PossessionComponent->bIsPlayer)
		{
			OnDamageTaken.Broadcast(CurrentHealth, MaxHealth);
		} else if (PossessionComponent.IsValid() && !PossessionComponent->bIsPlayer)
		{
			OnAIDamageTaken.Broadcast();
		} else
		{
			OnEmergentDamageTaken.Broadcast(CurrentHealth, MaxHealth);
		}
		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0;
			if (PossessionComponent.IsValid() && PossessionComponent->bIsPlayer)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Death"));
				OnDeath.Broadcast();
			}
			else if (PossessionComponent.IsValid() && !PossessionComponent->bIsPlayer) 
			{
				OnDeathWithLocation.Broadcast(PlayerCharacterOwner.Get(), PlayerCharacterOwner->GetActorLocation());
			}
		}
	}
}


void USCR_HealthComponent::Heal(float Healing)
{
	CurrentHealth += Healing;
}

void USCR_HealthComponent::ResetInvulnerability()
{
	bInvulnerable = false;
}


// Called when the game starts
void USCR_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	AActor* Owner = GetOwner();
	PlayerCharacterOwner = Cast<ASCR_PlayerCharacter>(Owner);
	if (Owner)
	{
		PossessionComponent = Owner->FindComponentByClass<USCR_Possession_Component>();
	}
}


// Called every frame
void USCR_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




	

