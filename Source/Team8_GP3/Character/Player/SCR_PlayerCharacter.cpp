// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Team8_GP3/SCR_SliceTest.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SCR_Dash_Component.h"
#include "SCR_Possession_Component.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Team8_GP3/Character/Attacks/Range/SCR_ProjectileLauncherComponent.h"


// Sets default values
ASCR_PlayerCharacter::ASCR_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent); // RootComponent is CapsuleComponent in ACharacter
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = false;
	bIsPossessionCameraLerp = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;  
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false; // Important: false
	
}

// Called when the game starts or when spawned
void ASCR_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem && BaseStatePlayerInputMap)
		{
			Subsystem->AddMappingContext(BaseStatePlayerInputMap, 0);
		}
	}
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->BrakingFrictionFactor = SpiritBrakingFriction; // Lower = more sliding
	Movement->GroundFriction = SpiritGroundFriction;        // Lower = more slick
	Movement->BrakingDecelerationWalking = SpiritBrakingDecelerationWalking; // Lower = slower stopping
}

// Called every frame
void ASCR_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Head bobbing effect
	if (bShouldHeadBob)
	{
		FVector Velocity = GetVelocity();
		bool bIsMoving = !Velocity.IsNearlyZero();
		if (bIsMoving)
		{
			HeadBobTime += DeltaTime;
			FVector HeadBobOffset;
			HeadBobOffset.Z = FMath::Abs(FMath::Sin(HeadBobTime * HeadBobFrequency)) * HeadBobAmplitude;
			SmoothedHeadBobOffset = FMath::VInterpTo(SmoothedHeadBobOffset, HeadBobOffset, DeltaTime, HeadBobInterpSpeed);
			SpringArm->SetRelativeLocation(SmoothedHeadBobOffset);

		} else
		{
			HeadBobTime = 0.f;
			SmoothedHeadBobOffset = FMath::VInterpTo(SmoothedHeadBobOffset, FVector::ZeroVector, DeltaTime, 6.f);
			SpringArm->SetRelativeLocation(SmoothedHeadBobOffset);

		}
	}
	if (!bShouldHeadBob)
	{
		// Subtle float effect (e.g., up and down)
		FloatingCameraTime += DeltaTime;
		float FloatOffsetZ = FMath::Sin(FloatingCameraTime * 2 * PI * FloatFrequency) * FloatAmplitude;
		FVector NewRelativeLocation = FVector(0.f, 0.f, FloatOffsetZ);
		SpringArm->SetRelativeLocation(NewRelativeLocation);
	}

}

// Called to bind functionality to input
void ASCR_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASCR_PlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASCR_PlayerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ASCR_PlayerCharacter::LookUp);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASCR_PlayerCharacter::Turn);
		if (USCR_Dash_Component* DashComp = FindComponentByClass<USCR_Dash_Component>()) {DashComp->BindInput(EnhancedInputComponent);}
		if (USCR_Possession_Component* PossessionComponent = FindComponentByClass<USCR_Possession_Component>()) {PossessionComponent->BindInput(EnhancedInputComponent);}
		if (USCR_ProjectileLauncherComponent* ProjectileComponent = FindComponentByClass<USCR_ProjectileLauncherComponent>()) {ProjectileComponent->BindInput(EnhancedInputComponent);}

	}
}

void ASCR_PlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>(); 
	if (Controller && InputValue != 0.0f)
	{
		FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
	}
}

void ASCR_PlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>(); 
	if (Controller && InputValue != 0.0f)
	{
		FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);
	}
}

void ASCR_PlayerCharacter::LookUp(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>(); 
	if (Camera && InputValue != 0.0f && !bIsPossessionCameraLerp)
	{
		FRotator Rotation = Camera->GetRelativeRotation();
		Rotation.Pitch = FMath::Clamp(Rotation.Pitch + InputValue * LookSensitivity, -89.9f, 89.9f);
		Camera->SetRelativeRotation(Rotation);
	}
}

void ASCR_PlayerCharacter::Turn(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>(); 
	if (Controller && InputValue != 0.0f && !bIsPossessionCameraLerp)
	{
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Yaw += InputValue * LookSensitivity;
		Controller->SetControlRotation(Rotation);
	}
}


