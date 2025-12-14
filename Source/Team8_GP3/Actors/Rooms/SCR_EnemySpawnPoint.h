#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Components/SCR_ActivatableComponent.h"
#include "Team8_GP3/Actors/SCR_ObjectPool.h"
#include "Components/BoxComponent.h"
#include "../Components/SCR_ActivatableComponent.h"
#include "SCR_EnemySpawnPoint.generated.h"
 
UENUM(BlueprintType)
enum class EnemyType : uint8 {
	knight = 0 UMETA(DisplayName = "knight"),
	archer = 1 UMETA(DisplayName = "archer"),
	mage = 2 UMETA(DisplayName = "mage")
}; 

UCLASS()
class TEAM8_GP3_API ASCR_EnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASCR_EnemySpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spawning")
	ASCR_ObjectPool* Pool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Spawning")
	int TotalAmountToSpawn = 1;
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category= "Spawning")
	int AmountSpawned = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EnemyType EnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> KnightBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ArcherBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MageBlueprint;

	UFUNCTION(BlueprintCallable)
	AActor* SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	AActor* ActivateEnemy();

	UFUNCTION()
	void Reset();
	
	UPROPERTY(VisibleAnywhere)
	USCR_ActivatableComponent* ActivatableComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;
};
