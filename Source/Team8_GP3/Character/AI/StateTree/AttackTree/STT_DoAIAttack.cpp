// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_DoAIAttack.h"

EStateTreeRunStatus USTT_DoAIAttack::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{

	Controller->DoAIAttack();
	
	FinishTask(true);
	return RunStatus;
}
