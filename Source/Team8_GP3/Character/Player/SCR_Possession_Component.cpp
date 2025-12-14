// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_Possession_Component.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SCR_HealthComponent.h"
#include "SCR_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SCR_CharacterVFX.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Team8_GP3/Actors/SCR_MemoryShard.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Team8_GP3/Character/Attacks/Range/SCR_ProjectileLauncherComponent.h"


class UEnhancedInputLocalPlayerSubsystem;

// Sets default values for this component's properties
USCR_Possession_Component::USCR_Possession_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; 
	TransitionTime = 1.0f;
	MaxRange = 1000.0f;
	ChannelDuration = 2.0f;
	InvulnerabilityTimer = 2.0f;
	CoolDown = 5.0f;
	MovementSpeedMultiplier = 0.5f;
	BarFillSpeed = 0.1f;
	bInPossessionState = false;
	PreviouslyTargetedActor = nullptr;
	bCanPossess = true;
	bDisabledVFX = false;
	PlayerDefaultTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	// ...
}




// Called when the game starts
void USCR_Possession_Component::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem && PossessionInputMappingContext)
		{
			Subsystem->AddMappingContext(PossessionInputMappingContext, 2);
		}
	}
	// ...
	
}

// Called every frame
void USCR_Possession_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPlayer || !bDisabledVFX)
	{
		USCR_CharacterVFX* Character = OwnerCharacter->FindComponentByClass<USCR_CharacterVFX>();
		if (Character)
		{
			Character->RemovePossessionEffect();
			Character->DisableOverlayMaterial();
			bDisabledVFX = true;
		}
	}
	// ...
}

void USCR_Possession_Component::BindInput(UEnhancedInputComponent* InputComponent)
{
	InputComponent->BindAction(PossessAction, ETriggerEvent::Started, this, &USCR_Possession_Component::HandlePossessPressed);
	InputComponent->BindAction(PossessAction, ETriggerEvent::Completed, this, &USCR_Possession_Component::HandlePossessReleased);
	InputComponent->BindAction(PossessCancel, ETriggerEvent::Triggered, this, &USCR_Possession_Component::HandlePossessCancel);

}

void USCR_Possession_Component::HandlePossessPressed()
{
	if (!bCanPossess || bInPossessionState) return;

	bPossessHeld = true;
	bPossessCharged = false;
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
	bIsTimeSlowed = true;
	TimeDilationIncrease = 1.0 / TimeDilation;
	PlayerTimeDilation = (PlayerTimeDilation * TimeDilationIncrease);
	OwnerCharacter->CustomTimeDilation = PlayerTimeDilation;
	
	StartChanneling();
	
	// Start timer to mark as charged
	GetWorld()->GetTimerManager().SetTimer(ChannelTimerHandle, [this](){bPossessCharged = true;}, (ChannelDuration / TimeDilationIncrease), false);
	GetWorld()->GetTimerManager().SetTimer(ChannelEnemyHealthViewTimer, this, &USCR_Possession_Component::TargetHealthView, (0.1 / TimeDilationIncrease), true);

}

void USCR_Possession_Component::HandlePossessReleased()
{
	if (!bInPossessionState) return;

	if (bPossessCharged)
	{
		StopTimeDilation();
		AttemptPossess(); // Do NOT cancel here; trigger the attempt
	}
	else
	{
		CancelChanneling(); // Released early
	}

	bPossessHeld = false;
	bPossessCharged = false;
}

void USCR_Possession_Component::HandlePossessCancel()
{
	if (!bPossessHeld || !bInPossessionState) return;

	CancelChanneling();

	bPossessHeld = false;
	bPossessCharged = false;

	GetWorld()->GetTimerManager().ClearTimer(ChannelTimerHandle);

}

void USCR_Possession_Component::StartChanneling()
{
	if (!bCanPossess || bInPossessionState) return;
	
	bInPossessionState = true;
	ChannelStartTime = GetWorld()->GetTimeSeconds();

	// Slow movement during channeling
	if (OwnerCharacter)
	{
		OriginalSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= ((100.0f + MovementSpeedMultiplier)/100.0f);
	}
	
	GetWorld()->GetTimerManager().SetTimer(ChannelProgressUpdateTimerHandle, this, &USCR_Possession_Component::BroadcastChannelProgress, (BarFillSpeed / TimeDilationIncrease), true);
	GetWorld()->GetTimerManager().SetTimer(TimeDilationTimerHandle, this, &USCR_Possession_Component::ResetDilationTimer, (BarFillSpeed / TimeDilationIncrease), true);

}

void USCR_Possession_Component::CancelChanneling()
{
	if (!bInPossessionState) return;

	bInPossessionState = false;
	
	GetWorld()->GetTimerManager().ClearTimer(ChannelTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChannelProgressUpdateTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChannelEnemyHealthViewTimer);
	
	// Restore movement speed
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	}

	if (PreviouslyTargetedActor)
	{
		USCR_CharacterVFX* Character = PreviouslyTargetedActor->FindComponentByClass<USCR_CharacterVFX>();
		Character->RemovePossessionEffect();
		Character->DisableOverlayMaterial();
		USCR_HealthComponent* HealthComp = PreviouslyTargetedActor->FindComponentByClass<USCR_HealthComponent>();
		if (HealthComp)
		{
			HealthComp->bInvulnerable = false;
		}
	}
	PreviouslyTargetedActor = nullptr;
	CurrentlyTargetedActor = nullptr;
	BroadcastChannelCancel();
    StopTimeDilation();
}


void USCR_Possession_Component::AttemptPossess()
{
	
	if (!bInPossessionState || !bCanPossess) return;
	
	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;

	FVector Start;
	FVector Direction;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		FRotator ViewRot;
		PC->GetPlayerViewPoint(Start, ViewRot);
		Direction = ViewRot.Vector();
	}
	else
	{
		Start = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f); // eye height offset
		Direction = OwnerCharacter->GetActorForwardVector();
	}

	FVector End = Start + Direction * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	
	if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag("Possessable"))
	{
		bPossessionInProgress = true;
		StartCameraLerp(Hit.GetActor());
		USCR_HealthComponent* HealthComp = OwnerCharacter->FindComponentByClass<USCR_HealthComponent>();
		if (HealthComp)
		{
			HealthComp->bInvulnerable = true; 
		}
		OnPossessionSuccessful.Broadcast();
	}
	else if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag("Memory"))
	{
		ASCR_MemoryShard* MemoryShard = Cast<ASCR_MemoryShard>(Hit.GetActor());
		{
			if (MemoryShard->bIsKey)
			{
				ASCR_PlayerCharacter* Player = Cast<ASCR_PlayerCharacter>(OwnerCharacter);
				if (Player)
				{
					Player->HasKey = true;
				}
			}
			MemoryShard->OnPickup();
			CancelChanneling();
		}
	}
	else
	{
		// Reset state if failed
		CancelChanneling();
	}
}

void USCR_Possession_Component::TargetHealthView()
{
		
	if (!bInPossessionState || !bCanPossess) return;

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;

	FVector Start;
	FVector Direction;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		FRotator ViewRot;
		PC->GetPlayerViewPoint(Start, ViewRot);
		Direction = ViewRot.Vector();
	}
	else
	{
		Start = OwnerCharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f); // eye height offset
		Direction = OwnerCharacter->GetActorForwardVector();
	}

	FVector End = Start + Direction * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	CurrentlyTargetedActor = bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag("Possessable") ? Hit.GetActor(): nullptr;
	if (PreviouslyTargetedActor && PreviouslyTargetedActor != CurrentlyTargetedActor && bPossessionInProgress == false)
	{
		if (AActor* Target = PreviouslyTargetedActor)
		{

			USCR_CharacterVFX* Character = Target->FindComponentByClass<USCR_CharacterVFX>();
			Character->RemovePossessionEffect();
			Character->DisableOverlayMaterial();
		}
		USCR_HealthComponent* HealthComp = PreviouslyTargetedActor->FindComponentByClass<USCR_HealthComponent>();
		if (HealthComp)
		{
			HealthComp->bInvulnerable = false;
		}
	}

	if (CurrentlyTargetedActor && CurrentlyTargetedActor != PreviouslyTargetedActor && bPossessionInProgress == false)
	{
		if (AActor* Target = CurrentlyTargetedActor)
		{
			USCR_CharacterVFX* Character = Target->FindComponentByClass<USCR_CharacterVFX>();
			Character->ApplyPossessionEffect();
			Character->ApplyOverlayMaterial();
		}
		USCR_HealthComponent* HealthComp = CurrentlyTargetedActor->FindComponentByClass<USCR_HealthComponent>();
		if (HealthComp)
		{
			HealthComp->bInvulnerable = true;
		}
	}
	PreviouslyTargetedActor = CurrentlyTargetedActor;
}

void USCR_Possession_Component::StartCameraLerp(AActor* TargetActor)
{
	bIsPlayer = false;
	
	BroadcastChannelCancel();
	OnPossessionStart.Broadcast();
	OnPossessionGetTarget.Broadcast(TargetActor);
	
	if (!TargetActor) return;

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;

	ASCR_PlayerCharacter* CurrentCharacter = Cast<ASCR_PlayerCharacter>(OwnerCharacter);
	ASCR_PlayerCharacter* TargetCharacter = Cast<ASCR_PlayerCharacter>(TargetActor);
	if (!CurrentCharacter || !TargetCharacter) return;

	USCR_HealthComponent* HealthComponent = TargetCharacter->GetComponentByClass<USCR_HealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->bInvulnerable = true;
	}
	for (UActorComponent* Component : OwnerCharacter->GetComponents())
	{
		if (Component && Component->GetName().Contains(TEXT("AC_Melee")))
		{
			FString Command = "DisableFPMelee";
			Component->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);
		}
	}
	
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		PC->SetViewTargetWithBlend(TargetCharacter, TransitionTime, EViewTargetBlendFunction::VTBlend_Cubic);
		CurrentCharacter->bIsPossessionCameraLerp = true;
	}
	
	bDisabledVFX = false;
	PreviouslyTargetedActor = nullptr;
	CurrentlyTargetedActor = nullptr;
	
	// Delay possession until after camera blend finishes
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(this, &USCR_Possession_Component::FinishPossession, TargetActor),
		TransitionTime,
		false
	);
}

void USCR_Possession_Component::FinishPossession(AActor* TargetActor)
{
	if (!TargetActor) return;

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;

	APawn* TargetPawn = Cast<APawn>(TargetActor);
	if (TargetPawn && Controller != TargetPawn->GetController())
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			PC->SetControlRotation(TargetPawn->GetActorRotation());

			auto* Pawn = Cast<ASCR_AI_Base>(PC->GetPawn());
			
			Controller->UnPossess();
			Controller->Possess(TargetPawn);

			PC->SetViewTargetWithBlend(TargetPawn, 0.2f);

			TargetPawn->bUseControllerRotationYaw = true;
			if (ACharacter* Char = Cast<ACharacter>(TargetPawn))
			{
				Char->GetCharacterMovement()->bOrientRotationToMovement = false;
			}

			if (Pawn)
				Pawn->RepossessAI();
		}
	}
	if (TargetPawn)
	{
		if (USCR_Possession_Component* NewPossessComp = TargetPawn->FindComponentByClass<USCR_Possession_Component>())
		{
			NewPossessComp->bCanPossess = false;
			NewPossessComp->bIsPlayer = true;
			NewPossessComp->GetWorld()->GetTimerManager().SetTimer(
				NewPossessComp->CooldownTimerHandle,
				NewPossessComp,
				&USCR_Possession_Component::ResetPossessionCooldown,
				NewPossessComp->CoolDown,
				false
			);
		}
		if (USCR_HealthComponent* HealthComponent = TargetPawn->GetComponentByClass<USCR_HealthComponent>())
		{
			HealthComponent->bInvulnerable = true;
			HealthComponent->CurrentHealth *= 5.0f;
			HealthComponent->MaxHealth *= 5.0f;
			HealthComponent->GetWorld()->GetTimerManager().SetTimer(
			InvulnerabilityTimerHandle,
			HealthComponent,
			&USCR_HealthComponent::ResetInvulnerability,
			InvulnerabilityTimer,
			false
			);
		}
		if (USCR_ProjectileLauncherComponent* RangeComp = TargetPawn->GetComponentByClass<USCR_ProjectileLauncherComponent>())
		{
			RangeComp->SetIsPlayer(true);
		}

		for (UActorComponent* Component : TargetPawn->GetComponents())
		{
			if (Component && Component->GetName().Contains(TEXT("AC_Melee")))
			{
				FString Command = "EnableFPMelee";
				Component->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);
			}
		}
		
		ASCR_PlayerCharacter* OtherCharacter = Cast<ASCR_PlayerCharacter>(TargetPawn);
		if (OtherCharacter)
		{
			TWeakObjectPtr<USkeletalMeshComponent> CurrentMesh = OtherCharacter->GetMesh();
			if (CurrentMesh.IsValid())
			{
				CurrentMesh->SetVisibility(false, true);
			}
		}
		
		OnPossessionGetTarget.Broadcast(TargetActor);
	}

	// Restore movement speed on new pawn if needed
	if (ACharacter* NewChar = Cast<ACharacter>(TargetPawn))
	{
		NewChar->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	}
	
	USCR_HealthComponent* HealthComp = OwnerCharacter->FindComponentByClass<USCR_HealthComponent>();
	if (HealthComp)
	{
		HealthComp->bInvulnerable = false;
		HealthComp->CurrentHealth /= 5.0f;
		HealthComp->MaxHealth /= 5.0f;
	}

	if (USCR_ProjectileLauncherComponent* RangeComp = OwnerCharacter->GetComponentByClass<USCR_ProjectileLauncherComponent>())
	{
		RangeComp->SetIsPlayer(false);
	}
	
	bInPossessionState = false;
	
	ASCR_PlayerCharacter* CurrentCharacterBase = Cast<ASCR_PlayerCharacter>(OwnerCharacter);
	CurrentCharacterBase->bIsPossessionCameraLerp = false;
	if (AActor* Target = CurrentCharacterBase)
	{
		if (!bIsSpirit)
		{
			USCR_CharacterVFX* Character = Target->FindComponentByClass<USCR_CharacterVFX>();
			Character->RemovePossessionEffect();
			Character->DisableOverlayMaterial();
			PreviouslyTargetedActor = nullptr;
			CurrentlyTargetedActor = nullptr;
		}
	}
	//ASCR_PlayerCharacter* CurrentCharacter = Cast<ASCR_PlayerCharacter>(OwnerCharacter);
	if (CurrentCharacterBase)
	{
		TWeakObjectPtr<USkeletalMeshComponent> CurrentMesh = CurrentCharacterBase->GetMesh();
		if (CurrentMesh.IsValid())
		{
			CurrentMesh->SetVisibility(true, true);
		}
		
	}
	
	bPossessionInProgress = false;
	if (bIsSpirit)
	{
		OwnerCharacter->SetActorEnableCollision(false); // disables collision

	}
	OnPossessionSuccessful.Broadcast();
}

void USCR_Possession_Component::ResetPossessionCooldown()
{
	bCanPossess = true;
}



void USCR_Possession_Component::BroadcastChannelProgress()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float ElapsedTime = (CurrentTime - ChannelStartTime) * TimeDilationIncrease;
	float Progress = FMath::Clamp(ElapsedTime / ChannelDuration, 0.0f, 1.0f);

	OnPossessionProgress.Broadcast(Progress);
	
	if (Progress >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChannelProgressUpdateTimerHandle);

	}
	
}

void USCR_Possession_Component::BroadcastChannelCancel()
{
	float ZeroProgress = 0.0f;
	OnPossessionProgressCancelled.Broadcast(ZeroProgress);
}

void USCR_Possession_Component::StopTimeDilation()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), PlayerDefaultTimeDilation);
	PlayerTimeDilation = PlayerDefaultTimeDilation;
	OwnerPawn->CustomTimeDilation = PlayerDefaultTimeDilation;
	bIsTimeSlowed = false;
}

void USCR_Possession_Component::ResetDilationTimer()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float ElapsedTime = (CurrentTime - ChannelStartTime) * TimeDilationIncrease;
	float DilationProgress = FMath::Clamp(ElapsedTime / TimeSlowDownLength, 0.0f, 1.0f);
	
	if (DilationProgress >= 1.0f)
	{
		StopTimeDilation();
		GetWorld()->GetTimerManager().ClearTimer(TimeDilationTimerHandle);

	}
}

