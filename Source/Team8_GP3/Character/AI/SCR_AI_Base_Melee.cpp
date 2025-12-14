// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_AI_Base_Melee.h"


// Sets default values
ASCR_AI_Base_Melee::ASCR_AI_Base_Melee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASCR_AI_Base_Melee::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCR_AI_Base_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCR_AI_Base_Melee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

