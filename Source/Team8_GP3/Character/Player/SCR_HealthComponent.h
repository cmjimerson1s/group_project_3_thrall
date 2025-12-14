// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_HealthComponent.generated.h"

class USCR_Possession_Component;
class ASCR_PlayerCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIDamageTakenDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmergentDamageTakenDelegate, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathWithLocationDelegate, ASCR_PlayerCharacter*, character,  FVector, DeathLocation); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathUnitDelegate, TWeakObjectPtr<ASCR_PlayerCharacter>, self); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTakenDelegate, float, CurrentHealth, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(CallInEditor, Category="Debugging")
	void TestKill();
	// Sets default values for this component's properties
	USCR_HealthComponent();
	UFUNCTION(BlueprintCallable)
	void DamageHealth(float Damage);
	UFUNCTION(BlueprintCallable)
	void Heal(float Healing);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvulnerable;
	UFUNCTION(BlueprintCallable)
	void ResetInvulnerability();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth;
	UPROPERTY(BlueprintAssignable, Category = "Death")
	FOnInteractDelegate OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FOnDeathWithLocationDelegate OnDeathWithLocation;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FOnDeathUnitDelegate OnDeathUnitDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FOnDamageTakenDelegate OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FOnAIDamageTakenDelegate OnAIDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FOnEmergentDamageTakenDelegate OnEmergentDamageTaken;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	TWeakObjectPtr<ASCR_PlayerCharacter> PlayerCharacterOwner;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	TWeakObjectPtr<USCR_Possession_Component> PossessionComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
