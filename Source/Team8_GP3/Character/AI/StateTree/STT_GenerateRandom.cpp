// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_GenerateRandom.h"

EStateTreeRunStatus USTT_GenerateRandom::EnterState(FStateTreeExecutionContext& Context,
                                                    const FStateTreeTransitionResult& Transition)
{
	ReturnedInt = FMath::RandRange(MinRange, MaxRange);
	ReturnedFloat = FMath::RandRange(MinRange, MaxRange);

	RandomBool = FMath::RandBool();

	return Super::EnterState(Context, Transition);
	
}
