#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SCR_SaveGame.generated.h"

// USTRUCT()
// struct FActorSnapshotData
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	FTransform Transform;
//     
// 	UPROPERTY()
// 	bool bIsDestroyed;
//
// 	// Add any other persistent state variables here
// };

UCLASS()
class TEAM8_GP3_API USCR_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool ShouldLoadSavedLevel() const { return !LevelName.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadSavedLevel(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void CaptureWorldState(UWorld* World, const FTransform& SavedTransform);

	// Restores the world to saved state including player position
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RestoreWorldState(UWorld* World);

	// Manually register an actor to be tracked
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void RegisterActor(AActor* Actor);

	// Manually unregister an actor
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void UnregisterActor(AActor* Actor);

	// Gets the saved player transform
	UFUNCTION(BlueprintCallable, Category = "Save System")
	FTransform GetSavedPlayerTransform() const { return PlayerTransform; }

	// Gets the saved level name
	UFUNCTION(BlueprintCallable, Category = "Save System")
	FString GetSavedLevelName() const { return LevelName; }


	


private:
	// Finds all relevant actors in the world
	void CacheRelevantActors(UWorld* World);

	// Validates currently tracked actors
	bool ValidateTrackedActors(UWorld* World) const;

	// Weak references to actors we're tracking
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> TrackedActors;
    
	// // Saved data for each actor (keyed by actor name)
	// UPROPERTY()
	// TMap<FString, FActorSnapshotData> SavedActorData;

	// Saved player transform
	UPROPERTY()
	FTransform PlayerTransform;

	// Name/path of the saved level
	UPROPERTY()
	FString LevelName;
};