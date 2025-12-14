#include "SCR_EnemySpawnPoint.h"
#include "SCR_EnemySpawnPoint.h"
#include "Team8_GP3/CustomMacros.h"
#include "Engine/World.h"
   
ASCR_EnemySpawnPoint::ASCR_EnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionProfileName("Trigger");

	ActivatableComponent = CreateDefaultSubobject<USCR_ActivatableComponent>(TEXT("ActivatableComponent"));

}

void ASCR_EnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	ActivatableComponent->OnEnable.AddDynamic(this, &ASCR_EnemySpawnPoint::Reset);
}

void ASCR_EnemySpawnPoint::Reset()
{
	AmountSpawned = 0;
}


AActor* ASCR_EnemySpawnPoint::SpawnEnemy()
{
	if (!ActivatableComponent->bIsEnabled) return nullptr;
	
	TSubclassOf<AActor> EnemyClassToSpawn = nullptr;

	switch (EnemyToSpawn)
	{
	case EnemyType::knight:
		EnemyClassToSpawn = KnightBlueprint;
		break;
	case EnemyType::archer:
		EnemyClassToSpawn = ArcherBlueprint;
		break;
	case EnemyType::mage:
		EnemyClassToSpawn = MageBlueprint;
		break;
	default:
		print("Unknown enemy type selected.");
		return nullptr;
	}

	if (EnemyClassToSpawn && GetWorld())
	{
		AmountSpawned++;
		if (AmountSpawned > TotalAmountToSpawn) InvokeFunction(.1f,ActivatableComponent->Disable());
		return GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn, GetActorLocation(), GetActorRotation());
	}
	else
	{
		print("Missing Enemy Blueprint or World is null.");
		return nullptr;
	}
}

AActor* ASCR_EnemySpawnPoint::ActivateEnemy()
{
	if (!ActivatableComponent->bIsEnabled) return nullptr;
	if (!Pool) return nullptr;

	USCR_ActivatableComponent* PooledComp = Pool->GetPooledActivatableComponent();
	if (PooledComp)
	{
		AActor* owner = PooledComp->GetOwner();
		owner->SetActorLocation(GetActorLocation());
		owner->SetActorRotation(GetActorRotation());
		PooledComp->Enable();

		AmountSpawned++;
		//FString Test = FString::Printf(TEXT("%d == %d"), AmountSpawned, TotalAmountToSpawn);
		//print(Test);
		if (AmountSpawned == TotalAmountToSpawn) InvokeFunction(.1f,ActivatableComponent->Disable());
		return owner;
	}
	return nullptr;
}

