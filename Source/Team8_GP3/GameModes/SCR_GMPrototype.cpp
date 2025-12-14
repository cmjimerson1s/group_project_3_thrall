#include "SCR_GMPrototype.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_GP3/PD_GameInstance.h"
#include "Engine/LevelStreaming.h"

ASCR_GMPrototype::ASCR_GMPrototype()
{
    CurrentSaveGame = nullptr;
}

void ASCR_GMPrototype::StartPlay()
{
    Super::StartPlay();

    // if (LoadSaveOnStart)
    // {
    //     LoadGame();
    // }
}

void ASCR_GMPrototype::LoadPlayerAndActors()
{
    if (CurrentSaveGame)
    {
        // Restore world state after level loads
        ResetLevel();
        CurrentSaveGame->RestoreWorldState(GetWorld());
        RestorePlayerState();
    }
}

void ASCR_GMPrototype::RestoreWorld() const
{
    CurrentSaveGame->RestoreWorldState(GetWorld());
    RestorePlayerState();
}

void ASCR_GMPrototype::RestorePlayerState() const
{
    if (!CurrentSaveGame) return;
    
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Pawn->SetActorTransform(CurrentSaveGame->GetSavedPlayerTransform());
        }
    }
}

void ASCR_GMPrototype::SaveGame(const FTransform& SavedTransform)
 {
    UE_LOG(LogTemp, Display, TEXT("SaveGame::SaveGame"));
    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
        UE_LOG(LogTemp, Display, TEXT("CurrentSaveGame loaded from slot: %s"), *SaveName);
    }
    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::CreateSaveGameObject(USCR_SaveGame::StaticClass()));
        UE_LOG(LogTemp, Display, TEXT("CurrentSaveGame created: %s"), *SaveName);
    }
    if (CurrentSaveGame)
    {
        CurrentSaveGame->CaptureWorldState(GetWorld(), SavedTransform);
        UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveName, 0);
    }
}

void ASCR_GMPrototype::LoadGame()
{
    // CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
    if (CurrentSaveGame && CurrentSaveGame->ShouldLoadSavedLevel())
    {
        CurrentSaveGame->LoadSavedLevel(this);
    }
    if (CurrentSaveGame)
    {
        LoadPlayerAndActors();
    }
    else
    {
        ResetLevel();
    }
}

void ASCR_GMPrototype::PlayerDeath()
{
    if (!GetWorld()) return;
    
    // UPD_GameInstance* GameInstance = Cast<UPD_GameInstance>(GetGameInstance());
    // if (!GameInstance) return;
    //
    // // Use a safer level reset approach
    // GetWorldTimerManager().ClearAllTimersForObject(this);
    //
    // // Delay the reset to ensure clean state
    // FTimerHandle SafeResetHandle;
    // GetWorldTimerManager().SetTimer(SafeResetHandle, [this]()
    // {
    //     if (!this || !GetWorld()) return;
    //     ResetLevel();
    // }, 0.1f, false);

    ResetLevel();
    return;
    // if (CurrentSaveGame)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("CurrentSaveGame already loaded, no need to load again"));
    //     LoadGame();
    //     return;
    // }
    // CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
    // if (CurrentSaveGame)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("CurrentSaveGame had to be loaded from slot"));
    //     LoadGame();
    //     return;
    // }
    // CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::CreateSaveGameObject(USCR_SaveGame::StaticClass()));
    // if (CurrentSaveGame)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("CurrentSaveGame had to be created"));
    //     UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveName, 0);
    //     LoadGame();
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Failed to create or load CurrentSaveGame"));
    //     ResetLevel();
    // }

    // CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
    //
    // if (!CurrentSaveGame)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("No CurrentSaveGame"));
    //
    //     // Default reset if no save exists
    //     GetWorldTimerManager().SetTimerForNextTick([this]()
    //     {
    //         if (this && GetWorld()) LoadPlayerAndActors();
    //         return;
    //     });
    // }
    // else if (CurrentSaveGame)
    // {
    //     LoadPlayerAndActors();
    // }
    
}

void ASCR_GMPrototype::ResetLevel()
{
    /*
    if (!GetWorld()) return;
    
    FString MapName = GetWorld()->GetMapName();
    MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    UGameplayStatics::OpenLevel(this, FName(*MapName), true);
    */

    if (!GetWorld()) return;
    
    FString MapName = GetWorld()->GetMapName();
    MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    
    // Use seamless travel for more stable transitions
    UGameplayStatics::OpenLevel(this, FName(*MapName), true);
}


bool operator==(const FTransform& Lhs, const FTransform& RHS);

void ASCR_GMPrototype::PostLogin(APlayerController* NewPlayer)
{

    // Super::PostLogin(NewPlayer);
    //
    // UPD_GameInstance* GameInstance = Cast<UPD_GameInstance>(GetGameInstance());
    // if (!GameInstance) return;
    //
    // if (APawn* Pawn = NewPlayer->GetPawn())
    // {
    //     if (GameInstance->PlayerSpawnPointPosition != FVector::ZeroVector)
    //     {
    //         Pawn->SetActorLocationAndRotation(
    //         GameInstance->PlayerSpawnPointPosition,
    //         GameInstance->PlayerSpawnPointRotation
    //     );
    //     }
    // }
    
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("Player logged in"));
    UPD_GameInstance* GameInstance = Cast<UPD_GameInstance>(GetGameInstance());
    if (!GameInstance) return;
    UE_LOG(LogTemp, Warning, TEXT("GameInstance is %s"), *GameInstance->GetName());
    if (APawn* Pawn = NewPlayer->GetPawn())
    {
        if (!CurrentSaveGame)
            CurrentSaveGame = Cast<USCR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
        if (CurrentSaveGame && LoadSaveOnStart)
        {
            // Use saved player position if available
            UE_LOG(LogTemp, Log, TEXT("Restoring player position from save game"));
            if (CurrentSaveGame->GetSavedPlayerTransform().Equals(FTransform::Identity))
            {
                UE_LOG(LogTemp, Warning, TEXT("No saved player transform found, using default"));
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Saved player transform found"));
                Pawn->SetActorTransform(CurrentSaveGame->GetSavedPlayerTransform());
            }
            CurrentSaveGame->RestoreWorldState(GetWorld());
        }
        else if (GameInstance->PlayerSpawnPointPosition != FVector::ZeroVector)
        {
            UE_LOG(LogTemp, Log, TEXT("Restoring player position from game instance checkpoint"));
            // Fall back to checkpoint position
            Pawn->SetActorLocationAndRotation(
                GameInstance->PlayerSpawnPointPosition,
                GameInstance->PlayerSpawnPointRotation
            );
        }
    }
    if (!LoadSaveOnStart)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LoadSaveOnStart is false, skipping load"));
        LoadSaveOnStart = true;
    }
    
    
}
