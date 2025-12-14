// Fill out your copyright notice in the Description page of Project Settings.
 
#include "SCR_MemoryShard.h"

#include "Team8_GP3/PD_GameInstance.h"

ASCR_MemoryShard::ASCR_MemoryShard()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<USCR_Interactable>(TEXT("InteractableComponent"));
}

void ASCR_MemoryShard::AddToAllMemories(ASCR_MemoryShard* MemoryShard)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UPD_GameInstance* PDGameInstance = Cast<UPD_GameInstance>(GameInstance);
		if (PDGameInstance)
		{
			PDGameInstance->AllMemoryShards.Add(MemoryShard);
		}
	}
}

void ASCR_MemoryShard::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->OnInteract.AddDynamic(this, &ASCR_MemoryShard::OnPickup);
	
}

void ASCR_MemoryShard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
