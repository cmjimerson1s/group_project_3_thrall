#include "SCR_RoomManager.h"

#include <string>

#include "SCR_RoomManagerSubsystem.h"
#include "Team8_GP3/CustomMacros.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Kismet/GameplayStatics.h"
#include "SCR_EnemySpawnPoint.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include <cfloat>

#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Team8_GP3/PD_GameInstance.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

ASCR_RoomManager::ASCR_RoomManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName("Trigger");
}

void ASCR_RoomManager::BeginPlay()
{
	Super::BeginPlay();
	
	// if (GetWorld() == nullptr) return;
	//
	// USCR_RoomManagerSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<USCR_RoomManagerSubsystem>();
	//
	// if (Subsystem)Subsystem->RegisterRoomManager(this);

	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASCR_RoomManager::RegisterActors, .1f, false);
	// GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASCR_RoomManager::RegisterActors);
	// InvokeFunction(TimeToRegister, RegisterActors());
}

void ASCR_RoomManager::Tick(float DeltaTime)
{
	if (isCycling) Cycling(DeltaTime);
	if (isRandomizing) Randomizing(DeltaTime);
}

void ASCR_RoomManager::StartSpawning()
{
	switch (SpawningType)
	{
	case SpawningAlgorithm::cycling:
		CycleSpawners();
		break;
		
	case SpawningAlgorithm::random:
		RandomizeSpawners();
		break;

	default:

		break;
	}
}

void ASCR_RoomManager::ResetSpawners()
{
	for (ASCR_EnemySpawnPoint* spawnpoint : EnemySpawnPoints)
	{
		USCR_ActivatableComponent* Activatable = spawnpoint->ActivatableComponent;
		if (!Activatable->bIsEnabled)
		{
			TotalEnemies += spawnpoint->TotalAmountToSpawn;
			Activatable->Enable();
		}
		else
		{
			TotalEnemies += spawnpoint->AmountSpawned;
			spawnpoint->Reset();
		}
	}
}

void ASCR_RoomManager::CycleSpawners()
{
	ResetSpawners();
	DelayTimer = 0;
	isCycling = true;
}

void ASCR_RoomManager::Cycling(float DeltaTime)
{
	Delay = FMath::Clamp(Delay, 0.1f, 1000);
	if (DelayTimer > Delay)
	{
		DelayTimer = 0;
		
		if (EnemySpawnPoints.Num() == 0) return;
		
		bool execute = false;
		
		for (ASCR_EnemySpawnPoint* spawnpoint : EnemySpawnPoints)
		{ if (spawnpoint->ActivatableComponent->bIsEnabled) execute = true; }
		
		if (!execute) { isCycling = false; return;}

		currentCycle = currentCycle % EnemySpawnPoints.Num();
		currentCycle = FMath::Clamp(currentCycle, 0, EnemySpawnPoints.Num() - 1);
		
		AActor* Enemy = EnemySpawnPoints[currentCycle]->ActivateEnemy();
		
		if (!EnemySpawnPoints[currentCycle]->ActivatableComponent->bIsEnabled)
		{
			currentCycle = (currentCycle + 1) % EnemySpawnPoints.Num();
			DelayTimer = Delay + 1;
			return;
		}
		
		currentCycle = (currentCycle + 1) % EnemySpawnPoints.Num();
		
		if (USCR_HealthComponent* Health = Enemy->FindComponentByClass<USCR_HealthComponent>())
		{
			//Health->OnDeathWithLocation.AddDynamic(this, &ASCR_RoomManager::OnEnemyDeathWithLocation);
		}
	}
	else
	{
		DelayTimer += DeltaTime;
	}
}

void ASCR_RoomManager::RandomizeSpawners()
{
	ResetSpawners();
	isRandomizing = true;
}

void ASCR_RoomManager::Randomizing(float DeltaTime)
{
	if (DelayTimer > RandomDelay)
	{
		DelayTimer = 0;

		bool execute = false;
		
		for (ASCR_EnemySpawnPoint* spawnpoint : EnemySpawnPoints)
		{ if (spawnpoint->ActivatableComponent->bIsEnabled) execute = true; }
		
		if (!execute) { isRandomizing = false; return;}

		int RandomIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);

		AActor* Enemy = EnemySpawnPoints[RandomIndex]->ActivateEnemy();
		
		if (Enemy)
		{
			if (USCR_HealthComponent* Health = Enemy->FindComponentByClass<USCR_HealthComponent>())
			{
				//Health->OnDeathWithLocation.AddDynamic(this, &ASCR_RoomManager::OnEnemyDeathWithLocation);
			}
		}
		else
		{
			DelayTimer = RandomDelay + 1;
		}
	}
	else
	{
		DelayTimer += DeltaTime;
	}
}

void ASCR_RoomManager::UnregisterEnemy(ASCR_AI_Base* enemy)
{
	
}


void ASCR_RoomManager::RegisterActors()
{
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	for (AActor* OtherActor : OverlappingActors)
	{
		if (ASCR_EnemySpawnPoint* SpawnPoint = Cast<ASCR_EnemySpawnPoint>(OtherActor))
		{
			EnemySpawnPoints.Add(SpawnPoint);
			TotalEnemies += SpawnPoint->TotalAmountToSpawn;
			continue;
		}

		if (ASCR_AI_Base* Enemy = Cast<ASCR_AI_Base>(OtherActor))
		{
			TotalEnemies++;

			if (USCR_HealthComponent* Health = Enemy->FindComponentByClass<USCR_HealthComponent>())
			{
				//Health->OnDeathWithLocation.AddDynamic(this, &ASCR_RoomManager::OnEnemyDeathWithLocation);
			}
			continue;
		}
	}
	if (TriggerBox)
	{
		TriggerBox->DestroyComponent();
		TriggerBox = nullptr;
	}
}

void ASCR_RoomManager::OnEnemyDeathWithLocation(FVector DeathLocation)
{
	DeadEnemiesAmount++;

	if (CalculateOdds()) DropMemoryShard(DeathLocation);
}

bool ASCR_RoomManager::CalculateOdds() const
{
	// const int RemainingEnemies = TotalEnemies - DeadEnemiesAmount;
	// const int RemainingShards = MemoryShardsToDrop.Num();
	//
	// if (RemainingShards <= 0)
	// {
	// 	return false;
	// }
	//
	// const float DropChance = static_cast<float>(RemainingShards) / static_cast<float>(RemainingEnemies);
	//
	// UE_LOG(LogTemp, Warning, TEXT("odds: %f"), DropChance);
	//
	// return UKismetMathLibrary::RandomBoolWithWeight(DropChance);

	int RemainingEnemies = TotalEnemies - DeadEnemiesAmount;

	if (RemainingEnemies <= 1) return true;
	
	return false;
}

void ASCR_RoomManager::DropMemoryShard(const FVector& SpawnLocation)
{
	int shardAmount = MemoryShardsToDrop.Num();
	
	for (int i = 0; i < shardAmount; i++)
	{
		float Radius = ShardSpawningRadius;
		FVector Center = SpawnLocation;
		FVector location;
	
		if (shardAmount == 1) location = SpawnLocation;
		else
		{
			float AngleRadians = (2 * PI / shardAmount) * i;
			location = Center + FVector(FMath::Cos(AngleRadians), FMath::Sin(AngleRadians), 0.f) * Radius;
		}
			
		FActorSpawnParameters SpawnParams;
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(MemoryShardsToDrop[i], location, FRotator::ZeroRotator, SpawnParams);
	}
	//FString Message = FString::Printf(TEXT("Deactivated in room %d"), TotalEnemies);
	//UKismetSystemLibrary::PrintString(this, Message, true, true, FLinearColor::Red, 10.0f);
	//GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->SendActivatedStatus(false);

	Destroy();
}
