// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_AI_Base_Controller.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"


// Sets default values
ASCR_AI_Base_Controller::ASCR_AI_Base_Controller(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASCR_AI_Base_Controller::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCR_AI_Base_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DrawDebug)
		DrawGizmo();
	
	IsPlayerInAttackZone();
}

void ASCR_AI_Base_Controller::ActivateAI()
{
	OnAgentActivate.Broadcast();
}

void ASCR_AI_Base_Controller::DoAIAttack()
{
	if (!AIPawn)
		return;
	
	AIPawn->Attack();
}

void ASCR_AI_Base_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPawn = Cast<ASCR_AI_Base>(InPawn);

	AIPawn->AssignController(this);
}

void ASCR_AI_Base_Controller::OnUnPossess()
{
	Super::OnUnPossess();

	AIPawn->ReturnAllTokens();
}

void ASCR_AI_Base_Controller::DrawGizmo()
{

	//Attack buffer zone
	
	FVector Center = AIPawn->GetActorLocation();
	
	DrawDebugCircle(
	   GetWorld(),
	   Center,
	   AttackZoneMinDistance,
	   Segments,
	   Color,
	   false,        // Persistent lines
	   lifetime,         // Lifetime
	   0,            // Depth priority
	   2.0f,         // Thickness
	   YAxis,
	   ZAxis,
	   true
   );

	DrawDebugCircle(
	   GetWorld(),
	   Center,
	   AttackZoneMaxDistance,
	   Segments,
	   Color,
	   false,        // Persistent lines
	   lifetime,         // Lifetime
	   0,            // Depth priority
	   2.0f,         // Thickness
	   YAxis,
	   ZAxis,
	   true
   );
}

bool ASCR_AI_Base_Controller::IsPlayerInAttackZone()
{
	auto* target = GetTarget();

	if(!target || !AIPawn) return false;

	float RangeToPlayerTarget = FVector::Dist(AIPawn->GetActorLocation(), target->GetActorLocation());

	if (RangeToPlayerTarget > AttackZoneMaxDistance)
	{
		PlayerDistance = EPlayerDistance::IsFar;
	}

	if (RangeToPlayerTarget < AttackZoneMinDistance)
	{
		PlayerDistance = EPlayerDistance::IsClose;
	}
	
	if(RangeToPlayerTarget >= AttackZoneMinDistance && RangeToPlayerTarget <= AttackZoneMaxDistance)
	{
		Color = FColor::Green;
		PlayerDistance = EPlayerDistance::IsInZone;
		return true;
	}
	Color = FColor::Red;
	return false;
}

AActor* ASCR_AI_Base_Controller::GetTarget()
{
	return GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget();
}
