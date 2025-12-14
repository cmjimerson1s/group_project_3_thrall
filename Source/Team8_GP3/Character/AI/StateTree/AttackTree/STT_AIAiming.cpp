// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_AIAiming.h"

#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

EStateTreeRunStatus USTT_AIAiming::EnterState(FStateTreeExecutionContext& Context,
                                              const FStateTreeTransitionResult& Transition)
{
	Character->OnAIAiming.Broadcast();
	return Super::EnterState(Context, Transition);
}
