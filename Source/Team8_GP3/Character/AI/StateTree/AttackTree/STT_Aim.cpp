// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_Aim.h"

#include "MathUtil.h"
#include "Kismet/KismetMathLibrary.h"

EStateTreeRunStatus USTT_Aim::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (GoRight)
	{
		MultiplyValue = FMathf::Abs(MultiplyValue);
	}
	else
	{
		MultiplyValue *= -1;
	}

	auto CharacterLocation = Controller->GetPawn()->GetActorLocation();
	auto TargetLocation = GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget()->GetActorLocation();

	auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, TargetLocation);

	MoveTargetLocation = UKismetMathLibrary::GetRightVector(LookAtRotation) * MultiplyValue + CharacterLocation;

	FHitResult HitResult;
	FVector Start = Actor->GetActorLocation();
	FVector End = Controller->GetTarget()->GetActorLocation();

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);

	if (bHit)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 2.0f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Red, false, 2.0f);

		if (!Cast<ASCR_AI_Base_Controller>(HitResult.GetActor()->GetInstigatorController()))
		{
			FinishTask(true);	
		}
	}
	
	return RunStatus;
}

EStateTreeRunStatus USTT_Aim::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	GoRight = FMath::RandBool();
	
	return RunStatus;
}
