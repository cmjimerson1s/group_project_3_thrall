// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_Possession_Component.generated.h"


class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessionProgress, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessionGetTarget, AActor*, PossessedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessionProgressCancelled, float, ZeroProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessionStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessionSuccessful);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_Possession_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCR_Possession_Component();
	//InputHandling for component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PossessionInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PossessAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PossessCancel;
	//Values for Possession Mechanic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float TransitionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float ChannelDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float InvulnerabilityTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float MovementSpeedMultiplier;
	//Bool states for Possession
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	bool bInPossessionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession") 
	bool bCanPossess;
	UPROPERTY(Transient)
	bool bDisabledVFX;
	UFUNCTION(BlueprintCallable)
	void StartChanneling();
	UFUNCTION(BlueprintCallable)
	void CancelChanneling();
	//UI Event for Delivering Possession amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	float BarFillSpeed;
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FOnPossessionProgress OnPossessionProgress;
	UFUNCTION(BlueprintCallable, Category = "Possession")
	void BroadcastChannelProgress();
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FOnPossessionProgressCancelled OnPossessionProgressCancelled;
	UFUNCTION(BlueprintCallable, Category = "Possession")
	void BroadcastChannelCancel();
	//Event to broadcast currently possessed Actor
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FOnPossessionGetTarget OnPossessionGetTarget;
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FOnPossessionStart OnPossessionStart;
	UPROPERTY(BlueprintAssignable, Category = "Possession")
	FOnPossessionSuccessful OnPossessionSuccessful;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Possession")
	bool bIsPlayer;
	//Slow-Motion Effect variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow")
	bool bIsTimeSlowed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow", meta = (ToolTip = "Default is 1.0, input is a percent representation of the time dilation."))
	float TimeDilation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow")
	float TimeSlowDownLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow")
	float PlayerTimeDilation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow")
	float PlayerDefaultTimeDilation;
	UFUNCTION(BlueprintCallable)
	void StopTimeDilation();
	UPROPERTY(Transient)
	float TimeDilationIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time Slow")
	bool bPossessionInProgress = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spirit") 
	bool bIsSpirit;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void HandlePossessPressed();
	UFUNCTION(BlueprintCallable)
	void HandlePossessReleased();
	UFUNCTION(BlueprintCallable)
	void HandlePossessCancel();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* OwnerCharacter;
	void BindInput(UEnhancedInputComponent* InputComponent);

private:
	FTimerHandle ChannelTimerHandle;
	FTimerHandle ChannelEnemyHealthViewTimer;
	FTimerHandle CooldownTimerHandle;
	FTimerHandle ChannelProgressUpdateTimerHandle;
	FTimerHandle InvulnerabilityTimerHandle;
	FTimerHandle TimeDilationTimerHandle;

	bool bPossessCharged = false;
	bool bPossessHeld = false;
	float OriginalSpeed = 600.f;
	float ChannelStartTime;
	UPROPERTY(Transient)
	AActor* PreviouslyTargetedActor;
	UPROPERTY(Transient)
	AActor* CurrentlyTargetedActor;
	void AttemptPossess();
	void TargetHealthView();
	void StartCameraLerp(AActor* TargetActor);
	void FinishPossession(AActor* TargetActor);
	void ResetPossessionCooldown();
	void ResetDilationTimer();
};
