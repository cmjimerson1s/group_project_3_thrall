#include "SCR_Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Team8_GP3/Actors/Rooms/SCR_RoomManager.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Team8_GP3/Character/Player/SCR_Possession_Component.h"

ASCR_Projectile::ASCR_Projectile()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(16.0f);

	// Set up collision responses for penetration
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic); // Or your preferred channel

	// Initialize all responses to ignore
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Set specific responses
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // For characters
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // For static world
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // For dynamic objects

	// Use overlap events instead of hit events
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASCR_Projectile::OnOverlapBegin);

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // Start with no gravity

	// Projectile mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Start with physics disabled
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetSimulatePhysics(false);
	ProjectileMovement->SetActive(false);

	ActivatableComponent = CreateDefaultSubobject<USCR_ActivatableComponent>(TEXT("ActivatableComponent"));

	// Create Niagara component
	NiagaraTrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraTrailEffect"));
	NiagaraTrailEffect->SetupAttachment(RootComponent);
	NiagaraTrailEffect->SetAutoActivate(false);
}

void ASCR_Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (ActivatableComponent)
	{
		ActivatableComponent->Disable(); // Start disabled
	}
	// Removing physics on Projectile Movement Stop
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ASCR_Projectile::OnProjectileMovementComponentStopped);
}

void ASCR_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCR_Projectile::LaunchProjectile(FVector AimVector)
{
	if (ProjectileMovement)
	{
		// Reset any existing movement
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Velocity = FVector::ZeroVector;

		
		// Set initial velocity
		ProjectileMovement->Velocity = AimVector * InitialSpeed;
		ProjectileMovement->UpdateComponentVelocity();
	}

	// Reset any existing timers
	GetWorldTimerManager().ClearTimer(GravityTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);

	// Set up new timers
	if (InitialGravityDelay > 0.f)
	{
		GetWorldTimerManager().SetTimer(GravityTimerHandle, this, &ASCR_Projectile::EnableGravity, InitialGravityDelay,
											false);	}
	else
	{
		// If no delay, enable gravity immediately
		ProjectileMovement->ProjectileGravityScale = CustomGravityScale;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	                                   bIsTimedProjectile ? TEXT("Timed Projectile: Enabled") : TEXT("Timed Projectile: Disabled"));
	if (bIsTimedProjectile)
	{
		GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &ASCR_Projectile::DestroyDueToTimeout,
		                                       ProjectileLifetime, false);
	}

	ActivateTrailEffect();
}

void ASCR_Projectile::EnableGravity()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = CustomGravityScale;
	}
}

void ASCR_Projectile::DestroyDueToTimeout()
{
	// Trigger expiration effect before destroying
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Projectile expired due to timeout."));
	OnProjectileExpired();
	HandleProjectileDestruction();
}

void ASCR_Projectile::OnProjectileExpired()
{
	if (bActivateOnExpired && ProjectileEffectClass)
	{
		
		// Apply the effect on expiration
		USCR_ProjectileEffectBase* Effect = NewObject<USCR_ProjectileEffectBase>(this, ProjectileEffectClass);
		if (Effect)
		{
			FHitResult HitResult = FHitResult
			{
				// Set the hit location to the current location of the projectile
				this,
				nullptr, // No specific component hit
				GetActorLocation(),
				FVector::UpVector, // Default normal, can be adjusted
			};
			Effect->ApplyEffect(this, HitResult, GetInstigator());
		}
	}
}

void ASCR_Projectile::EndOfProjectile()
{
	GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	SetPhysicsEnabled(false);
	DeactivateTrailEffect();

	if (bDeleteOnStop)
	{
		HandleProjectileDestruction();
	}
	else
	{
		// Stop all movement physics
		if (CollisionComponent)
		{
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (ProjectileMovement)
		{
			ProjectileMovement->StopMovementImmediately();
			ProjectileMovement->SetActive(false);
			ProjectileMovement->Deactivate();
		}

		// Disable ticking
		SetActorTickEnabled(false);
		PrimaryActorTick.bCanEverTick = false;

		// If we hit something, attach to it
		if (StuckToComponent && StuckToComponent->IsValidLowLevel())
		{
			// Attach to the component we hit
			FAttachmentTransformRules AttachmentRules(
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				false
			);

			AttachToComponent(StuckToComponent, AttachmentRules);

			// Register for destruction notification
			if (StuckToActor)
			{
				StuckToActor->OnDestroyed.AddUniqueDynamic(this, &ASCR_Projectile::OnStuckToActorDestroyed);
			}
		}
	}
}

void ASCR_Projectile::ResetProjectile()
{
	SetPhysicsEnabled(false);

	DeactivateTrailEffect();
	
	CurrentPenetrationCount = 0;
    
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    
	StuckToActor = nullptr;
	StuckToComponent = nullptr;
    
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Velocity = FVector::ZeroVector;
		ProjectileMovement->ProjectileGravityScale = 0.f;
	}
    
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
    
	SetActorHiddenInGame(false);
}


void ASCR_Projectile::OnStuckToActorDestroyed(AActor* DestroyedActor)
{
	// Clean up our reference
	StuckToActor = nullptr;
	StuckToComponent = nullptr;

	// Detach before destruction
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	HandleProjectileDestruction();

}

void ASCR_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Projectile Overlap With: ") + OtherActor->GetName());
	if (!OtherActor || OtherActor == this || !OtherComp) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("2nd Projectile Overlap With: ") + OtherActor->GetName());

	if (USCR_Possession_Component* U_P_C = OtherActor->FindComponentByClass<USCR_Possession_Component>())
	{
		bool bIsPlayer = U_P_C->bIsPlayer;
		if ((bIsPlayer && bIgnorePlayer) || (!bIsPlayer && bIgnoreEnemy))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("3rd Projectile Overlap With: ") + OtherActor->GetName());
			return; // Ignore this actor
		}
	}

	float ProjectileSpeed = ProjectileMovement->Velocity.Size();
	if (ProjectileSpeed <= MinPenetrationSpeed)
	{
		SetPhysicsEnabled(false);
		return;
	}

	StuckToActor = OtherActor;
	StuckToComponent = OtherComp;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Projectile hit: ") + OtherActor->GetName());

	
	if (CurrentPenetrationCount <= MaxPenetration)
	{
		if (AutoGravityOnPenetration && CurrentPenetrationCount == 0)
		{			
			EnableGravity();
		}
		CurrentPenetrationCount++;
		float NewSpeed = ProjectileSpeed - PenetrationSpeedLoss;
		if (NewSpeed <= MinPenetrationSpeed)
		{
			EndOfProjectile();
		}
		else
		{
			ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * NewSpeed;
		}
	}
	if (CurrentPenetrationCount >= MaxPenetration)
	{
		EndOfProjectile();
	}

	if (ProjectileEffectClass)
	{
		USCR_ProjectileEffectBase* Effect = NewObject<USCR_ProjectileEffectBase>(this, ProjectileEffectClass);
		if (Effect)
		{
			Effect->ApplyEffect(OtherActor, SweepResult, GetInstigator());
		}
	}
}

void ASCR_Projectile::SetPhysicsEnabled(bool bEnabled)
{
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(bEnabled
			                                        ? ECollisionEnabled::QueryAndPhysics
			                                        : ECollisionEnabled::NoCollision);
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->SetActive(bEnabled);
		if (bEnabled)
		{
			ProjectileMovement->SetUpdatedComponent(CollisionComponent);
			ProjectileMovement->Velocity = FVector::ZeroVector;
			ProjectileMovement->UpdateComponentVelocity();
		}
	}
}


void ASCR_Projectile::HandleProjectileDestruction()
{
	// Clean up before deactivating
	if (StuckToActor)
	{
		StuckToActor->OnDestroyed.RemoveDynamic(this, &ASCR_Projectile::OnStuckToActorDestroyed);
	}

	// Reset projectile state
	ResetProjectile();

	// Disable instead of destroy
	if (ActivatableComponent)
	{
		ActivatableComponent->Disable();
	}
	else
	{
		// Fallback to destroy if no activatable component
		Destroy();
	}
}

void ASCR_Projectile::ActivateTrailEffect()
{
	if (NiagaraTrailEffect)
	{
		NiagaraTrailEffect->Activate(true);
	}
}

void ASCR_Projectile::DeactivateTrailEffect()
{
	if (NiagaraTrailEffect)
	{
		NiagaraTrailEffect->DeactivateImmediate();
		NiagaraTrailEffect->DestroyInstanceNotComponent();
		
	}
}


