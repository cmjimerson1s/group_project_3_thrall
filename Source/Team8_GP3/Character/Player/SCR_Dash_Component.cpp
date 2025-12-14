// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_Dash_Component.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SCR_HealthComponent.h"
#include "SCR_Possession_Component.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
USCR_Dash_Component::USCR_Dash_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USCR_Dash_Component::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem && DashInputMappingContext)
		{
			Subsystem->AddMappingContext(DashInputMappingContext, 1);
		}
	}
	// ...
	
}


// Called every frame
void USCR_Dash_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USCR_Dash_Component::BindInput(UEnhancedInputComponent* InputComponent)
{
	InputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &USCR_Dash_Component::Dash);

}

void USCR_Dash_Component::Dash()
{
	if (!bCanDash || !OwnerCharacter || !bDashEnabled) return;
	USCR_HealthComponent* HealthComp = OwnerCharacter->FindComponentByClass<USCR_HealthComponent>();
	if (HealthComp && bInvulnerableWhileDashing == true)
	{
		HealthComp->bInvulnerable = true;
	}
	USCR_Possession_Component* Possession_Component = OwnerCharacter->FindComponentByClass<USCR_Possession_Component>();
	if (Possession_Component)
	{
		bTimeSlowed = Possession_Component->bIsTimeSlowed;
		if (bTimeSlowed)
		{
			TimeDialationAmount = Possession_Component->TimeDilationIncrease;
		} else
		{
			TimeDialationAmount = 1.0;
		}
	}
	UCharacterMovementComponent* Movement = OwnerCharacter->FindComponentByClass<UCharacterMovementComponent>();
	PreDashVelocity = OwnerCharacter->GetVelocity();
	DefaultBrakingFriction = Movement->BrakingFriction;
	DefaultGroundFriction = Movement->GroundFriction;
	DefaultBrakingDecelerationWalking = Movement->BrakingDecelerationWalking;
	Movement->BrakingFriction = 1.0f; 
	Movement->GroundFriction = 8.0;        
	Movement->BrakingDecelerationWalking = 2048.0f;
	DashDirection = GetDashDirection();
	DashDirection.Z = 0.0f;
	DashDirection.Normalize();
	bCanDash = false;
	DashElapsedTime = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(
			DashTickTimerHandle, this,
			&USCR_Dash_Component::PerformDashTick,
			(DashTimeInterval / TimeDialationAmount), true
		);
}
void USCR_Dash_Component::PerformDashTick()
{
	if (!OwnerCharacter) return;

	float StrengthToUse = DashStrength;

	FVector ForwardVector = OwnerCharacter->GetActorForwardVector().GetSafeNormal();
	float Dot = FVector::DotProduct(ForwardVector, DashDirection);

	if (Dot > 0.8f) // 0.8 is an adjustable threshold for "forward enough"
	{
		StrengthToUse = ForwardDashStrength;
	}

	OwnerCharacter->LaunchCharacter(DashDirection * (StrengthToUse * 1000), true, true);
	DashElapsedTime += DashTimeInterval;
	if (DashElapsedTime >= DashDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashTickTimerHandle);
		EndDash();
	}
}

void USCR_Dash_Component::EndDash()
{
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &USCR_Dash_Component::ResetDash, DashCooldown);
	USCR_HealthComponent* HealthComp = OwnerCharacter->FindComponentByClass<USCR_HealthComponent>();
	if (HealthComp && bInvulnerableWhileDashing == true)
	{
		HealthComp->bInvulnerable = false;
	}
	UCharacterMovementComponent* Movement = OwnerCharacter->FindComponentByClass<UCharacterMovementComponent>();
	Movement->BrakingFriction = DefaultBrakingFriction; 
	Movement->GroundFriction = DefaultGroundFriction;        
	Movement->BrakingDecelerationWalking = DefaultBrakingDecelerationWalking;
	Movement->Velocity = PreDashVelocity;
}


void USCR_Dash_Component::ResetDash()
{
	bCanDash = true;
}

FVector USCR_Dash_Component::GetDashDirection() const
{
	if (!OwnerCharacter || !OwnerCharacter->Controller) return FVector::ZeroVector;

	FVector InputVector = OwnerCharacter->GetLastMovementInputVector();
	if (!InputVector.IsNearlyZero())
		return InputVector.GetSafeNormal();

	FRotator ControlRot = OwnerCharacter->Controller->GetControlRotation();
	return FRotationMatrix(ControlRot).GetScaledAxis(EAxis::X);

}

