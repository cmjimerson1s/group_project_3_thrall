// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_AI_Base.h"

#include "SCR_AIBrainSubsystem.h"
#include "AIController/SCR_AI_Base_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_GP3/Character/Player/SCR_HealthComponent.h"
#include "Team8_GP3/Interfaces/Attack.h"


// Sets default values
ASCR_AI_Base::ASCR_AI_Base()
{
	ActivatableComponent = CreateDefaultSubobject<USCR_ActivatableComponent>(TEXT("Activatable component"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Example function, perhaps in an actor class
void ASCR_AI_Base::DrawLineExample(UWorld* WorldContext, const FVector& Start, const FVector& End)
{
	if (!WorldContext) return;
	
	DrawDebugLine(
		WorldContext,       // UWorld*
		Start,              // Starting point
		End,                // Ending point
		FColor::Red,        // Line color
		true,              // Persistent (false = disappears after time)
		-1.0f,               // Duration in seconds
		0,                  // Depth priority (0 = default)
		2.0f                // Line thickness
	);
}

void ASCR_AI_Base::SetSpawn()
{
	PatrolPoints.Remove(SpawnLocation);
	SpawnLocation = GetActorLocation();
	PatrolPoints.Add(SpawnLocation);

	OnAISpawn.Broadcast();
}

// Called when the game starts or when spawned
void ASCR_AI_Base::BeginPlay()
{
	bool bWasPlacedInLevel = HasAnyFlags(RF_WasLoaded);
	Super::BeginPlay();

	SubscribeToEvents();

	SpawnLocation = GetActorLocation();
	PatrolPoints.Add(SpawnLocation);

	currentPatrolPointIndex = 0;

	if (!bWasPlacedInLevel) return;
	//Code here will be for units that are manually placed in the level

	
	TArray<AActor*> managers;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIManager::StaticClass(), managers);

	for (AActor* Manager : managers)
	{
		if (Manager->ActorHasTag(AiManagerTag))
		{
			Cast<AAIManager>(Manager)->AddAgent(this);
			AIManager = Cast<AAIManager>(Manager);
			AIManager->TotalEnemies++;
		}
	}
	
}

void ASCR_AI_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DrawDebugPatrolLines)
	if (PatrolPoints.Num() > 0)
	{
		FVector startLocation = PatrolPoints[PatrolPoints.Num()-1];	
		for (FVector PatrolPoint : PatrolPoints)
		{
			DrawLineExample(GetWorld(), startLocation, PatrolPoint);

			startLocation = PatrolPoint;
		}
	}
}

// Called to bind functionality to input
void ASCR_AI_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASCR_AI_Base::AssignController(ASCR_AI_Base_Controller* controllerToBeAssigned)
{
	if (!AIController)
	{
		AIController = Cast<ASCR_AI_Base_Controller>(controllerToBeAssigned);
	}
}

void ASCR_AI_Base::RepossessAI()
{
	if (AIController)
		AIController->Possess(this);
}

void ASCR_AI_Base::ReturnAllTokens()
{
	if (StoredTokens < 0)
		return;
	
	GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->ReturnAttackToken(CharacterType, StoredTokens);
	
	StoredTokens = 0;
}

void ASCR_AI_Base::Attack()
{
	if (Attacking)
		return;
	
	for (UActorComponent* Component : GetComponents())
	{
			
		if (Component && Cast<IAttack>(Component))
		{
			Cast<IAttack>(Component)->Attack();;
				
		}
	}

	OnAiAttack.Broadcast();
	Attacking = true;
}

void ASCR_AI_Base::AttackFinished()
{
	StoredTokens = 0;
	Attacking = false;
}

void ASCR_AI_Base::SubscribeToEvents()
{
	GetComponentByClass<USCR_HealthComponent>()->OnDeath.AddDynamic(this, &ASCR_AI_Base::Died);
	GetComponentByClass<USCR_HealthComponent>()->OnAIDamageTaken.AddDynamic(this, &ASCR_AI_Base::AggroEnemy);
}

void ASCR_AI_Base::Died()
{
	ReturnAllTokens();
	
	AIController->UnPossess();
}

void ASCR_AI_Base::AggroEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("Took damage"));
	GetComponentByClass<USCR_HealthComponent>()->OnAIDamageTaken.RemoveDynamic(this, &ASCR_AI_Base::AggroEnemy);

	AIController->State = EAIState::Attacking;
	AIController->ActivateAI();

	OnAIAggro.Broadcast();
}



