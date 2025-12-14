// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SCR_AIBrainSubsystem.h"
#include "Team8_GP3/Actors/Components/SCR_ActivatableComponent.h"
#include "Team8_GP3/Character/Player/SCR_PlayerCharacter.h"
#include "Team8_GP3/Interfaces/Attack.h"
#include "SCR_AI_Base.generated.h"

class AAIManager;
class ASCR_AI_Base_Controller;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIReloading);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIAggro);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAISpawn);

UCLASS()
class TEAM8_GP3_API ASCR_AI_Base : public ASCR_PlayerCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASCR_AI_Base();

	UFUNCTION(BlueprintCallable)
	void DrawLineExample(UWorld* WorldContext, const FVector& Start, const FVector& End);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AssignController(ASCR_AI_Base_Controller* controllerToBeAssigned);

	UFUNCTION(BlueprintCallable)
	void RepossessAI();

	UFUNCTION(BlueprintCallable)
	void ReturnAllTokens();

	UFUNCTION(BlueprintCallable)
	void Attack();

	void AttackFinished();

	void SubscribeToEvents();

	UFUNCTION(BlueprintCallable)
	void Died();

	UFUNCTION(BlueprintCallable)
	void AggroEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool ShouldNotGetAddedToManager = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	ECharacterType CharacterType = ECharacterType::Ranged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AAIManager* AIManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsAILastManStanding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	bool DrawDebugPatrolLines = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrolling")
	int currentPatrolPointIndex = 0;

	UPROPERTY(BlueprintAssignable, Category = "AI|Events")
	FOnAIAiming OnAIAiming;

	UPROPERTY(BlueprintAssignable, Category = "AI|Events")
	FOnAIReloading OnAIReloading;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAIAttack OnAiAttack;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAIAggro OnAIAggro;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAISpawn OnAISpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Events", meta = (ToolTip = "Shows if the AI sees the player or not"))
	bool CanSeePlayer = false;

	UFUNCTION(BlueprintCallable)
	void SetSpawn();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Tags")
	FName AiManagerTag;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Tokens")
	int StoredTokens = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool Attacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USCR_ActivatableComponent* ActivatableComponent;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AIBody;

	UPROPERTY(Transient)
	ASCR_AI_Base_Controller* AIController = nullptr;

	UPROPERTY(Transient)
	TScriptInterface<IAttack> attackComponent = nullptr;	
};
