// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_Dash_Component.generated.h"


class UInputMappingContext;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_Dash_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCR_Dash_Component();
	UFUNCTION(BlueprintCallable)
	void Dash();
	UFUNCTION(BlueprintCallable)
	void EndDash();
	UFUNCTION(BlueprintCallable)
	void PerformDashTick();
	UFUNCTION(BlueprintCallable)
	void ResetDash();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bDashEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashTimeInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float ForwardDashStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bInvulnerableWhileDashing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	bool bCanDash = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DashInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashElapsedTime;
	FVector DashDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector PreDashVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DefaultBrakingFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DefaultGroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DefaultBrakingDecelerationWalking;
	UPROPERTY(Transient)
	float TimeDialationAmount;
	UPROPERTY(Transient)
	float bTimeSlowed;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FTimerHandle DashCooldownTimerHandle;
	FTimerHandle DashTickTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	class ACharacter* OwnerCharacter;
	FVector GetDashDirection() const;
	void BindInput(UEnhancedInputComponent* InputComponent);

};
