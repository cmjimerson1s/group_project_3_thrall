// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Team8_GP3/Actors/AIManager.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"
#include "SCR_AI_Base_Controller.generated.h"

class USCR_AIBrainSubsystem;
class UBehaviorTree;

UENUM(BlueprintType)
enum class EPlayerDistance : uint8
{
	
	IsInZone		UMETA(DisplayName = "Player in zone"),
	IsClose			UMETA(DisplayName = "Player is close"),
	IsFar			UMETA(DisplayName = "Player is far")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle			UMETA(DisplayName = "Idle state"),
	Attacking		UMETA(DisplayName = "Attack state")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAgentActivate);

UCLASS()
class TEAM8_GP3_API ASCR_AI_Base_Controller : public AAIController
{
public:
	// Sets default values for this actor's properties
	ASCR_AI_Base_Controller(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ActivateAI();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAgentActivate OnAgentActivate;
	
	UFUNCTION(BlueprintCallable)
	AActor* GetTarget();
	
	UFUNCTION(BlueprintCallable)
	void DoAIAttack();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerInAttackZone();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerDistance PlayerDistance = EPlayerDistance::IsInZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Setup")
	EAIState State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackZoneMinDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackZoneMaxDistance = 100.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void DrawGizmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Setup")
	ASCR_AI_Base* AIPawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Setup")
	ECharacterType AiType;

	//Just random gizmo stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	bool ShouldDrawGizmo = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	FVector YAxis = FVector(1, 0, 0); // Use Y to draw in XZ plane
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	FVector ZAxis = FVector(0, 1, 0); // Use Z to draw in XZ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	int32 Segments = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	float lifetime = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|AttackRange|Gizmo")
	FColor Color = FColor::Red;

private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool DrawDebug = false;
};