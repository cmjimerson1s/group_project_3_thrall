#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Team8_GP3/SaveSystem/SCR_SaveGame.h"
#include "SCR_GMPrototype.generated.h"


class ASCR_ObjectPool;

UCLASS()
class TEAM8_GP3_API ASCR_GMPrototype : public AGameModeBase
{
	GENERATED_BODY()
    
public:
	ASCR_GMPrototype();
    
protected:
	UPROPERTY(EditAnywhere)
	float currentHealth = 100.f;
    
	UPROPERTY(EditAnywhere)
	float maxHealth = 100.f;

	UPROPERTY()
	USCR_SaveGame* CurrentSaveGame;
    
public:
	int Kills = 0;

	void SetCurrentHealth(float NewHealth);
	void SetMaxHealth(float NewMaxHealth);
	void SetCurrentAndMaxHealth(float NewCurrentHealth, float NewMaxHealth);
	void AddKill();

	UFUNCTION(BlueprintCallable)
	void PlayerDeath();

	UPROPERTY(EditAnywhere)
	TSet<ASCR_ObjectPool*> ObjectPools;

	virtual void StartPlay() override;
	virtual void ResetLevel() override;
    
	UFUNCTION(BlueprintCallable)
	void SaveGame(const FTransform& SavedTransform);
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UPROPERTY(EditAnywhere)
	bool LoadSaveOnStart = true;
	

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void LoadPlayerAndActors();
	void RestoreWorld() const;
	void RestorePlayerState() const;

private:
	FString SaveName = "CheckpointSave";
};