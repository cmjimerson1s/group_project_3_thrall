// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_Melee_Attack.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"


// Sets default values
ASCR_Melee_Attack::ASCR_Melee_Attack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	RootComponent = Sword;
}

// Called when the game starts or when spawned
void ASCR_Melee_Attack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCR_Melee_Attack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsSwinging)
	{
		ElapsedTime += DeltaTime;
		float Progress = ElapsedTime / SwingDuration;

		if (Progress >= 1.0f)
		{
			FinishSwing();
		}
		else
		{
			// Interpolate the rotation
			FRotator NewRotation = FMath::Lerp(SwingStartRotation, SwingEndRotation, Progress);
			Sword->SetRelativeRotation(NewRotation);

			// Perform hit check halfway through the swing
			if (!bHitChecked && Progress >= 0.5f)
			{
				PerformHitCheck();
				bHitChecked = true;
			}
		}
	}
}
void ASCR_Melee_Attack::PerformAttack()
{
	if (bIsSwinging) return; // Prevent overlapping swings
	UE_LOG(LogTemp, Log, TEXT("Swinging"));
	bIsSwinging = true;
	bHitChecked = false;
	ElapsedTime = 0.0f;

	// Reset rotation to start
	Sword->SetRelativeRotation(SwingStartRotation);
}

void ASCR_Melee_Attack::UpdateSwing()
{
}

void ASCR_Melee_Attack::FinishSwing()
{
	bIsSwinging = false;
	ElapsedTime = 0.0f;
	bHitChecked = false;
	Sword->SetRelativeRotation(SwingStartRotation);

}

void ASCR_Melee_Attack::PerformHitCheck()
{
	// Line trace or collision check to apply damage
	FVector Start = Sword->GetComponentLocation();
	FVector End = Start + Sword->GetForwardVector() * AttackDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (USCR_HealthComponent * HealthComp = HitActor->FindComponentByClass<USCR_HealthComponent>())
			{
				HealthComp->DamageHealth(WeaponDamage);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Damage"));// Apply your custom damage
			}
		}
	}
}
