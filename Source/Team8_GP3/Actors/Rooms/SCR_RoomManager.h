#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h" 
#include "GameFramework/Actor.h"
#include "SCR_EnemySpawnPoint.h"
#include "../SCR_ObjectPool.h"
#include "Components/BoxComponent.h"
#include "Team8_GP3/Actors/SCR_MemoryShard.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "SCR_RoomManager.generated.h"

UENUM(BlueprintType)
enum class SpawningAlgorithm : uint8 {
	cycling = 0 UMETA(DisplayName = "cycling"),
	random = 1 UMETA(DisplayName = "random")
}; 
 
UCLASS()
class TEAM8_GP3_API ASCR_RoomManager : public AActor
{
	GENERATED_BODY()

public:
	ASCR_RoomManager();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	SpawningAlgorithm SpawningType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float TimeToRegister = .5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MemoryShards")
	int TotalEnemies = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float Delay = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float RandomDelay = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemoryShards")
	TArray<TSubclassOf<ASCR_MemoryShard>> MemoryShardsToDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemoryShards")
	float ShardSpawningRadius = 100;
	
	UFUNCTION(blueprintCallable, CallInEditor, Category = "Spawning")
	void StartSpawning();

	UFUNCTION(blueprintCallable, CallInEditor, Category = "Spawning")
	void ResetSpawners();
	
	UFUNCTION(blueprintCallable)
	void CycleSpawners();

	UFUNCTION(blueprintCallable)
	void RandomizeSpawners();

	UFUNCTION(blueprintCallable)
	void UnregisterEnemy(ASCR_AI_Base* enemy);

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnEnemyDeathWithLocation(FVector DeathLocation);

	UFUNCTION()
	void RegisterActors();
	
	bool CalculateOdds() const;

	void DropMemoryShard(const FVector& SpawnLocation);
	
	TArray<ASCR_EnemySpawnPoint*> EnemySpawnPoints;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY() int DeadEnemiesAmount = 0;

	//CYCLE SPAWNING
	UFUNCTION() void Cycling(float DeltaTime);
	UFUNCTION() void Randomizing(float DeltaTime);
	
	UPROPERTY() int currentCycle = 0;
	UPROPERTY() float DelayTimer = 0;
	
	UPROPERTY() bool isCycling = false;

	UPROPERTY() bool isRandomizing = false;
};
