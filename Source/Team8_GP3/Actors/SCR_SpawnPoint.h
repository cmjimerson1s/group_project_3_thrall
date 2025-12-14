// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"
#include "SCR_SpawnPoint.generated.h"

class AAIManager;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_SpawnPoint : public UStaticMeshComponent
{
protected:
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

public:

	USCR_SpawnPoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "This dictates how many Agents will spawn from this spot"))
	int SpawnSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning", meta = (ToolTip = "number of enemies to spawn at spawnpoint when it should start spawning"))
	int EnemiesToSpawnAtTheSameTime = 1;
	
	void SpawnUnit();

	void RequestUnitsToBeReady();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Set what type of enemy agent that should spawn here"))
	ECharacterType SpawnPointType = ECharacterType::Melee;

	bool shouldDestroy = false;

private:
	UPROPERTY(Transient)
	AAIManager* AIManager;
};
