// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_ProjectileLauncherComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SCR_Projectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_GP3/GameModes/SCR_GMPrototype.h"

// Sets default values for this component's properties
USCR_ProjectileLauncherComponent::USCR_ProjectileLauncherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USCR_ProjectileLauncherComponent::Attack()
{
	FireProjectile();
}


// Called when the game starts
void USCR_ProjectileLauncherComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem && AttackInputMappingContext)
		{
			Subsystem->AddMappingContext(AttackInputMappingContext, 3);
		}
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USCR_ProjectileLauncherComponent::InitProjectiles, .1f, false);
	// GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USCR_ProjectileLauncherComponent::InitProjectiles);
}

void USCR_ProjectileLauncherComponent::BindInput(UEnhancedInputComponent* InputComponent)
{
	InputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &USCR_ProjectileLauncherComponent::Attack);
}

void USCR_ProjectileLauncherComponent::InitProjectiles()
{
	if (SetProjectilePools())
	{
		SpawnProjectile();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USCR_ProjectileLauncherComponent::InitProjectiles - Failed to set projectile pools!"));
	}
}

bool USCR_ProjectileLauncherComponent::SetProjectilePools()
{
	bool SetAIPool = (AIProjectilePool == nullptr) && (AIProjectileClass != nullptr);
	bool SetPlayerPool = (PlayerProjectilePool == nullptr) && (PlayerProjectileClass != nullptr);
	if (SetAIPool)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT(
				   "USCR_ProjectileLauncherComponent::BeginPlay - AIProjectilePool is already set or no Projectile Class was given!"
			   ));
	}
	if (SetAIPool)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT(
				   "USCR_ProjectileLauncherComponent::BeginPlay - PlayerProjectilePool is already set or no Projectile Class was given!"
			   ));
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("USCR_ProjectileLauncherComponent::BeginPlay - World is null!"));
		return false;
	}
	AGameModeBase* GMBase = World->GetAuthGameMode();
	if (!GMBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("USCR_ProjectileLauncherComponent::BeginPlay - AuthGameMode is null!"));
		return false;
	}
	TObjectPtr<ASCR_GMPrototype> GameMode = Cast<ASCR_GMPrototype>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("USCR_ProjectileLauncherComponent::BeginPlay - GameMode is not of type ASCR_GMPrototype!"));
		return false;
	}
	TSet<ASCR_ObjectPool*>& ObjectPools = GameMode->ObjectPools;
	for (ASCR_ObjectPool* Pool : ObjectPools)
	{
		if (Pool)
		{
			if (SetAIPool && Pool->PooledActorClass == AIProjectileClass)
			{
				AIProjectilePool = Pool;
			}
			if (SetPlayerPool && Pool->PooledActorClass == PlayerProjectileClass)
			{
				PlayerProjectilePool = Pool;
			}
		}
	}
	return (AIProjectilePool && PlayerProjectilePool);
}

void USCR_ProjectileLauncherComponent::SpawnProjectile()
{
	TObjectPtr<ASCR_ObjectPool> ProjectilePool = bIsPlayer ? PlayerProjectilePool : AIProjectilePool;
	TObjectPtr<USceneComponent> SpawnPoint = bIsPlayer ? PlayerSpawnPoint : AISpawnPoint;
	if (ProjectilePool && SpawnPoint)
	{
		FTransform SpawnTransform = SpawnPoint->GetComponentTransform();
		FVector SpawnLocation = SpawnTransform.GetLocation();
		FRotator SpawnRotation = SpawnTransform.Rotator();

		if (AActor* ProjectileObject = ProjectilePool->GetPooledActorRoundRobin())
		{
			ProjectileObject->SetActorLocation(SpawnLocation);
			ProjectileObject->SetActorRotation(SpawnRotation);
			Projectile = Cast<ASCR_Projectile>(ProjectileObject);
			if (Projectile)
			{
				Projectile->ResetProjectile();

				Projectile->AttachToComponent(SpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				ArrowSpawn.Broadcast(Projectile);
				OwnerCharacter->OnDestroyed.AddUniqueDynamic(Projectile, &ASCR_Projectile::OnStuckToActorDestroyed);
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
				                                       &USCR_ProjectileLauncherComponent::SpawnProjectile, .1, false);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
			                                       &USCR_ProjectileLauncherComponent::SpawnProjectile, .1, false);
		}
	}
}

void USCR_ProjectileLauncherComponent::SetIsPlayer(bool Player)
{
	bIsPlayer = Player;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	                                 Player ? TEXT("Player Projectile Launcher") : TEXT("AI Projectile Launcher"));
	if (Projectile)
	{
		Projectile->HandleProjectileDestruction();
		Projectile = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Projectile cleared due to player change"));
	}
	SpawnProjectile();
}

void USCR_ProjectileLauncherComponent::FireProjectile()
{
	if (Projectile)
	{
		// Detach from spawn point
		Projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Enable movement but keep gravity disabled initially
		Projectile->SetPhysicsEnabled(true);

		FVector AimVector;
		
		if (!bIsPlayer)
		{
			if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				FVector UnNormalizedAimVector = PlayerCharacter->GetActorLocation() - AISpawnPoint->GetComponentLocation();
				AimVector = UnNormalizedAimVector.GetSafeNormal();
			}
			
		}
		else
		{
			AimVector = PlayerSpawnPoint->GetForwardVector();
		}

		
		Projectile->LaunchProjectile(AimVector);

		OwnerCharacter->OnDestroyed.RemoveDynamic(Projectile, &ASCR_Projectile::OnStuckToActorDestroyed);

		OnAttack.Broadcast();

		Projectile = nullptr;

		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this,
		                                       &USCR_ProjectileLauncherComponent::SpawnProjectile, ReloadTime, false);
	}
}

void USCR_ProjectileLauncherComponent::SetSpawnPoints(USceneComponent* NewAIProjectileSpawnPoint,
                                                      USceneComponent* NewPlayerProjectileSpawnPoint)
{
	AISpawnPoint = NewAIProjectileSpawnPoint;
	PlayerSpawnPoint = NewPlayerProjectileSpawnPoint;
}


// Called every frame
void USCR_ProjectileLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DebugFireTest)
	{
		Attack();
		DebugFireTest = false;
	}
}

void USCR_ProjectileLauncherComponent::TestFunction()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Clicked");
}
