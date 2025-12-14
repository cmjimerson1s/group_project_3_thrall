// Fill out your copyright notice in the Description page of Project Settings.

#include "SCR_Melee_Component.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"


class USCR_HealthComponent;
// Sets default values for this component's properties
USCR_Melee_Component::USCR_Melee_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bHitChecked = false;
	// ...
}

void USCR_Melee_Component::Attack()
{
	Super::Attack();
}


// Called when the game starts
void USCR_Melee_Component::BeginPlay()
{
	Super::BeginPlay();
	if (!Sword)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sword mesh not assigned on %s"), *GetOwner()->GetName());
	}
	// ...
	
}


// Called every frame
void USCR_Melee_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USCR_Melee_Component::SetSwordMesh(UStaticMeshComponent* SwordMesh)
{
	Sword = SwordMesh;
}

void USCR_Melee_Component::PerformAttack()
{
	if (bIsSwinging || !Sword)
		return;

	bIsSwinging = true;
	ElapsedTime = 0.f;

	FRotator CurrentRotation = Sword->GetRelativeRotation(); // Local to parent
	SwingStartRotation = FRotator(CurrentRotation.Pitch - 45.f, CurrentRotation.Yaw, CurrentRotation.Roll);
	SwingEndRotation = FRotator(CurrentRotation.Pitch + 45.f, CurrentRotation.Yaw, CurrentRotation.Roll);

	GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &USCR_Melee_Component::UpdateSwing, 0.01f, true);
}

void USCR_Melee_Component::UpdateSwing()
{
	if (!Sword) return;

	ElapsedTime += 0.01f;
	float Alpha = ElapsedTime / SwingDuration;
	if (!bHitChecked && Alpha >= 0.5f)
	{
		PerformHitCheck();
		bHitChecked = true;
	}
	if (Alpha >= 1.0f)
	{
		FinishSwing();
		return;
	}

	FRotator CurrentRotation = FMath::Lerp(SwingStartRotation, SwingEndRotation, Alpha);
	Sword->SetRelativeRotation(CurrentRotation); // Rotate only the sword
}

void USCR_Melee_Component::FinishSwing()
{
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
	bIsSwinging = false;

	if (Sword)
		Sword->SetRelativeRotation(SwingStartRotation); // Reset to original
}

void USCR_Melee_Component::PerformHitCheck()
{
	if (!Sword) return;

	FVector Start = Sword->GetComponentLocation();
	FVector End = Start + Sword->GetForwardVector() * AttackDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (USCR_HealthComponent* HealthComp = HitActor->FindComponentByClass<USCR_HealthComponent>())
			{
				HealthComp->DamageHealth(WeaponDamage);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Damage"));
			}
		}
	}

	// For visualization
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 2.f);
}

