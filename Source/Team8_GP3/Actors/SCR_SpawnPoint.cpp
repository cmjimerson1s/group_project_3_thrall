// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_SpawnPoint.h"

#include "AIManager.h"
#include "Components/SCR_ObjectPoolComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

void USCR_SpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	AIManager = Cast<AAIManager>(GetOwner());
	
	if (AIManager)
	{
		AIManager->AddSpawnPoint(this);
		AIManager->TotalEnemies += SpawnSize;
	}

	RequestUnitsToBeReady();
}

USCR_SpawnPoint::USCR_SpawnPoint(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void USCR_SpawnPoint::SpawnUnit()
{
	for (int i =0; i < EnemiesToSpawnAtTheSameTime; i++)
	{
		if (SpawnSize <= 0)
			return;
	
	auto* Agent = AIManager->GetComponentByClass<USCR_ObjectPoolComponent>()->GetPooledActor(SpawnPointType);
	
	Agent->SetActorLocation(GetComponentLocation());
	Agent->SetActorRotation(GetComponentRotation());

	
	Agent->GetComponentByClass<USCR_ActivatableComponent>()->Enable();
	Agent->SetSpawn();
	Agent->AIManager = AIManager;
	SpawnSize--;
	}	
}

void USCR_SpawnPoint::RequestUnitsToBeReady()
{
	AIManager->GetComponentByClass<USCR_ObjectPoolComponent>()->SpawnUnits(SpawnPointType, SpawnSize);
}
