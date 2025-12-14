#include "SCR_SaveGame.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SCR_SavableComponent.h"



void USCR_SaveGame::LoadSavedLevel(UObject* WorldContextObject)
{
    if (LevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No level name saved to load"));
        return;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World) return;

    // Get current level name (without streaming prefix)
    FString CurrentLevel = World->GetOuter()->GetPathName();
    // CurrentLevel.RemoveFromStart(World->StreamingLevelsPrefix);

    // Only load if we're not already in the correct level
    if (CurrentLevel != LevelName)
    {
        UE_LOG(LogTemp, Log, TEXT("Loading saved level: %s (Current: %s)"), *LevelName, *CurrentLevel);
        UGameplayStatics::OpenLevel(WorldContextObject, FName(*LevelName));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Already in correct level: %s"), *LevelName);
    }
}


void USCR_SaveGame::CaptureWorldState(UWorld* World, const FTransform& SavedTransform)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot capture world state - no world provided"));
        return;
    }

    // // Update our tracked actors list
    // if (!ValidateTrackedActors(World))
    // {
    //     CacheRelevantActors(World);
    // }

    // Clear previous data
    // SavedActorData.Empty();

    // // Save all relevant actors' states
    // for (const TWeakObjectPtr<AActor>& ActorPtr : TrackedActors)
    // {
    //     if (AActor* Actor = ActorPtr.Get())
    //     {
    //         FActorSnapshotData Data;
    //         Data.Transform = Actor->GetTransform();
    //         UE_LOG(LogTemp, Log, TEXT("Saving actor: %s at %s"), *Actor->GetName(), *Data.Transform.ToString());
    //         
    //         if (USCR_SavableComponent* Interactable = Cast<USCR_SavableComponent>(Actor))
    //         {
    //             Data.bIsDestroyed = Interactable->IsDestroyed();
    //         }
    //         
    //         SavedActorData.Add(Actor->GetName(), Data);
    //     }
    // }

    PlayerTransform = SavedTransform;

    // Save level name
    LevelName = World->GetOuter()->GetPathName();

    // UE_LOG(LogTemp, Log, TEXT("World state captured with %d actors"), SavedActorData.Num());
}

void USCR_SaveGame::RestoreWorldState(UWorld* World)
{
    if (!World || !ValidateTrackedActors(World))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot restore world state - invalid world or no tracked actors"));
        return;
    }

    // Restore all relevant actors' states
    // for (const TWeakObjectPtr<AActor>& ActorPtr : TrackedActors)
    // {
    //     if (AActor* Actor = ActorPtr.Get())
    //     {
    //         if (const FActorSnapshotData* Data = SavedActorData.Find(Actor->GetName()))
    //         {
    //             Actor->SetActorTransform(Data->Transform);
    //             
    //             if (USCR_SavableComponent* Interactable = Cast<USCR_SavableComponent>(Actor))
    //             {
    //                 Interactable->SetDestroyed(Data->bIsDestroyed);
    //             }
    //         }
    //     }
    // }

    // Restore player position (handled separately by game mode)
    UE_LOG(LogTemp, Log, TEXT("World state restored for %d actors"), TrackedActors.Num());
}

void USCR_SaveGame::RegisterActor(AActor* Actor)
{
    if (Actor && Actor->Implements<USCR_SavableComponent>())
    {
        TrackedActors.AddUnique(TWeakObjectPtr<AActor>(Actor));
    }
}

void USCR_SaveGame::UnregisterActor(AActor* Actor)
{
    TrackedActors.Remove(TWeakObjectPtr<AActor>(Actor));
}

void USCR_SaveGame::CacheRelevantActors(UWorld* World)
{
    TrackedActors.Empty();
    //
    // if (World)
    // {
    //     for (TActorIterator<AActor> It(World); It; ++It)
    //     {
    //         if (It->FindComponentByClass<USCR_SavableComponent>())
    //         {
    //             TrackedActors.Add(TWeakObjectPtr<AActor>(*It));
    //         }
    //     }
    // }
}

bool USCR_SaveGame::ValidateTrackedActors(UWorld* World) const
{
    if (TrackedActors.Num() == 0)
        return false;

    for (const TWeakObjectPtr<AActor>& ActorPtr : TrackedActors)
    {
        AActor* Actor = ActorPtr.Get();
        if (!Actor || Actor->GetWorld() != World)
        {
            return false;
        }
    }

    return true;
}