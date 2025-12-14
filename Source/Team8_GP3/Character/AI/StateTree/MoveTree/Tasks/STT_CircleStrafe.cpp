// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_CircleStrafe.h"

#include "Kismet/KismetMathLibrary.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"


EStateTreeRunStatus USTT_CircleStrafe::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	switch (DistanceToPlayer)
	{
	case EPlayerDistance::IsInZone:
		break;
	case EPlayerDistance::IsClose:
		ForwardModifier *= -1;
		break;
	case EPlayerDistance::IsFar:
		ForwardModifier = FMath::Abs(ForwardModifier);
		break;
	}

	if (WalkRight)
		DirectionModifier = FMath::Abs(DirectionModifier);
	else
		DirectionModifier *= -1;
	
	return RunStatus;
}

EStateTreeRunStatus USTT_CircleStrafe::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	auto CharacterLocation = Character->GetActorLocation();
	auto TargetLocation = GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget()->GetActorLocation();
	auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, TargetLocation);

	
	auto SidePosition = (UKismetMathLibrary::GetRightVector(LookAtRotation) * DirectionModifier) + CharacterLocation;
	
	if (DistanceToPlayer != EPlayerDistance::IsInZone)
	{
		SidePosition += UKismetMathLibrary::GetForwardVector(LookAtRotation) * ForwardModifier;
	}

	FVector PositionVector = FVector(SidePosition.X, SidePosition.Y, CharacterLocation.Z);
	
	
	Controller->MoveToLocation(PositionVector, 5.0f);
	
	return RunStatus;
}
