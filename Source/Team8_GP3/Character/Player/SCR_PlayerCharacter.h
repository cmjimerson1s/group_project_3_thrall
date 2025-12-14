// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "SCR_PlayerCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class TEAM8_GP3_API ASCR_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCR_PlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession Controls")
	bool bIsPossessionCameraLerp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	//Character Camera Movement and Sensitivity
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraSmoothSpeed = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float LookSensitivity = 1.0f;
	//Camera Float Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Camera")
	float FloatingCameraTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Camera")
	float FloatAmplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Camera")
	float FloatFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Camera")
	float RotationAmplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Camera")
	float RotationFrequency;
	//Floaty Movement Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floaty Movement", meta=(ToolTip = "Default: 1.0. Lower = more sliding"))
	float SpiritBrakingFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floaty Movement", meta=(ToolTip = "Default: 8.0. Lower = more slickness"))
	float SpiritGroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floaty Movement", meta=(ToolTip = "Default: 2048.0. Lower = slower stopping"))
	float SpiritBrakingDecelerationWalking;
	//Head-bob for walking Effect
	UPROPERTY(Transient)
	FVector SmoothedHeadBobOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head bobbing")
	bool bShouldHeadBob;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head bobbing")
	float HeadBobTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head bobbing")
	float HeadBobFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head bobbing")
	float HeadBobAmplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head bobbing")
	float HeadBobInterpSpeed;
	//PlayerCharacter Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* BaseStatePlayerInputMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveForwardAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnAction;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasKey = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FinalTransport = false;
private:
	UPROPERTY(Blueprintable)
	FRotator TargetControlRotation;
	UFUNCTION(BlueprintCallable)
	void MoveForward(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void LookUp(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Turn(const FInputActionValue& Value);
};
