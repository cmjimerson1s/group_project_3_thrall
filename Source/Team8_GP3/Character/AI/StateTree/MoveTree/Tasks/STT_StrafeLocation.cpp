// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_StrafeLocation.h"

#include "MathUtil.h"
#include "Kismet/KismetMathLibrary.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"

EStateTreeRunStatus USTT_StrafeLocation::EnterState(FStateTreeExecutionContext& Context,
                                                    const FStateTreeTransitionResult& Transition)
{
	if (ShouldRandomizeStrafeDistance)
	{
		MultiplyValue = FMath::FRandRange(0.0f, FMathf::Abs(MultiplyValue));

		if (FMath::RandBool())
		{
			MultiplyValue = FMathf::Abs(MultiplyValue);
		}
		else
		{
			MultiplyValue *= -1;
		}
	}
	
	auto CharacterLocation = Controller->GetPawn()->GetActorLocation();
	if (!GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget())
	{
		FinishTask(false);
		return RunStatus;
	}
	auto TargetLocation = GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget()->GetActorLocation();

	auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, TargetLocation);

	MoveTargetLocation = UKismetMathLibrary::GetRightVector(LookAtRotation) * MultiplyValue + CharacterLocation;

	return RunStatus;
}
