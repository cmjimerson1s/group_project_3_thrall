// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_FindNextPatrollPoint.h"

EStateTreeRunStatus USTT_FindNextPatrollPoint::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	if (Actor->PatrolPoints.Num() == 0)
		return RunStatus;

	
	
	Location = Actor->PatrolPoints[Actor->currentPatrolPointIndex];

	Controller->SetFocalPoint(FVector(Location.X, Location.Y, Actor->GetActorLocation().Z));
	
	Actor->currentPatrolPointIndex++;

	if (Actor->currentPatrolPointIndex >=  Actor->PatrolPoints.Num())
	{
		Actor->currentPatrolPointIndex = 0;
	}
	
if (ShouldPrint)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Green,
		Location.ToString()
	);

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Green,
		Actor->GetName()
	);
}
	
	
	return RunStatus;
}
