// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SCR_MemoryShard.h"
#include "Engine/GameInstance.h"
#include "PD_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API UPD_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Checkpoints")
	FVector PlayerSpawnPointPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Checkpoints")
	FRotator PlayerSpawnPointRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|MemoryShard")
	TArray<ASCR_MemoryShard*> AllMemoryShards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|MemoryShard")
	TArray<ASCR_MemoryShard*> CollectedMemoryShards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Cutscene")
	bool FirstTimePlay = true;
};
