// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_AIReloading.h"

#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

EStateTreeRunStatus USTT_AIReloading::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition)
{
	Character->OnAIReloading.Broadcast();
	return Super::EnterState(Context, Transition);
	
}
