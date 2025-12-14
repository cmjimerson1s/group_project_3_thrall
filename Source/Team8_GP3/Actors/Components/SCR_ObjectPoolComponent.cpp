// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_ObjectPoolComponent.h"

#include "SCR_ActivatableComponent.h"
#include "Team8_GP3/Actors/AIManager.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

// Sets default values for this component's properties
USCR_ObjectPoolComponent::USCR_ObjectPoolComponent(): Owner(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USCR_ObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AAIManager>(GetOwner());

	if (Owner)
		Owner->ObjectPool = this;
	
	if (PoolSize <= 0) PoolSize = 10;
}

void USCR_ObjectPoolComponent::TopUpPool(ECharacterType type)
{
	switch (type)
	{
	case ECharacterType::Melee:		
		for (int32 i = 0; i < PoolSize; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			ASCR_AI_Base* SpawnedAgent = GetWorld()->SpawnActor<ASCR_AI_Base>(
				MeleeAgentsClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (!SpawnedAgent) continue;

			SpawnedAgent->ActivatableComponent->Disable();;
		
			MeleePool.Add(SpawnedAgent);
			Owner->AddAgent(SpawnedAgent);
		}

		break;
	case ECharacterType::Ranged:
		for (int32 i = 0; i < PoolSize; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			ASCR_AI_Base* SpawnedAgent = GetWorld()->SpawnActor<ASCR_AI_Base>(
				RangedAgentsClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (!SpawnedAgent) continue;

			SpawnedAgent->ActivatableComponent->Disable();;
		
			RangedPool.Add(SpawnedAgent);
			Owner->AddAgent(SpawnedAgent);
		}
		
		break;
	case ECharacterType::Caster:
			UE_LOG(LogTemp, Error, TEXT("Getting a Caster from the pool is not allowed (SCR_ObjectPoolComponent)"));
	default: ;
	}
}

ASCR_AI_Base* USCR_ObjectPoolComponent::GetPooledActor(ECharacterType type)
{
	switch (type)
	{
	case ECharacterType::Melee:
		for (auto* Agent : MeleePool)
			if (Agent && Agent->IsHidden()) return Agent;
		break;
	case ECharacterType::Ranged:
		for (auto* Agent : RangedPool)
			if (Agent && Agent->IsHidden()) return Agent;
		break;
	case ECharacterType::Caster:
		UE_LOG(LogTemp, Error, TEXT("Getting a Caster from the pool is not allowed (SCR_ObjectPoolComponent)"));
		return nullptr;
	default: ;
	}
	
	TopUpPool(type);
	return GetPooledActor(type);
}

void USCR_ObjectPoolComponent::SpawnUnits(ECharacterType unitType, int ammount)
{
	switch (unitType)
	{
	case ECharacterType::Melee:		
		for (int32 i = 0; i < ammount; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			ASCR_AI_Base* SpawnedAgent = GetWorld()->SpawnActor<ASCR_AI_Base>(
				MeleeAgentsClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (!SpawnedAgent) continue;
			
			SpawnedAgent->ActivatableComponent->Disable();
			SpawnedAgent->SetActorTickEnabled(false);
		
			MeleePool.Add(SpawnedAgent);
			Owner->AddAgent(SpawnedAgent);
		}

		break;
	case ECharacterType::Ranged:
		for (int32 i = 0; i < ammount; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			ASCR_AI_Base* SpawnedAgent = GetWorld()->SpawnActor<ASCR_AI_Base>(
				RangedAgentsClass,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (!SpawnedAgent) continue;

			SpawnedAgent->ActivatableComponent->Disable();
			SpawnedAgent->SetActorTickEnabled(false);
		
			RangedPool.Add(SpawnedAgent);
			Owner->AddAgent(SpawnedAgent);
		}
		
		break;
	case ECharacterType::Caster:
		UE_LOG(LogTemp, Error, TEXT("Getting a Caster from the pool is not allowed (SCR_ObjectPoolComponent)"));
	default: ;
	}
}

void USCR_ObjectPoolComponent::ClearPool()
{
	CleanupPool();
}

void USCR_ObjectPoolComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CleanupPool();
}

void USCR_ObjectPoolComponent::CleanupPool()
{
	for (auto* Agent : RangedPool)
	{
		if (!Agent) continue;

		if (auto* ActorOwner = Agent)
			ActorOwner->Destroy();
	}
	
	for (auto* Comp : MeleePool)
	{
		if (!Comp) continue;

		if (auto* ActorOwner = Comp)
			ActorOwner->Destroy();
	}
    
	RangedPool.Empty();
	MeleePool.Empty();
}
