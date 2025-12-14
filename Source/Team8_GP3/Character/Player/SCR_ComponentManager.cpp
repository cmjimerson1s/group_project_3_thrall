// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_ComponentManager.h"

#include "SCR_Possession_Component.h"


// Sets default values for this component's properties
USCR_ComponentManager::USCR_ComponentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	OwnerCharacter = GetOwner();
	HealthComp = CreateDefaultSubobject<USCR_HealthComponent>(TEXT("HealthComp"));
	DashComp = CreateDefaultSubobject<USCR_Dash_Component>(TEXT("DashComp"));
	PossessionComp = CreateDefaultSubobject<USCR_Possession_Component>(TEXT("PossessionComp"));
	CharacterVFX = CreateDefaultSubobject<USCR_CharacterVFX>("CharacterVFX");

	// ...
}


// Called when the game starts
void USCR_ComponentManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USCR_ComponentManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

