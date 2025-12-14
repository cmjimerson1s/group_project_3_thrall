// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_FinishAttack.h"

#include "Team8_GP3/Character/AI/SCR_AI_Base.h"

EStateTreeRunStatus USTT_FinishAttack::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	Character->AttackFinished();
	FinishTask(true);
	return RunStatus;
}
