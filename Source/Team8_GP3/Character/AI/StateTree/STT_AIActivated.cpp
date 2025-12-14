// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_AIActivated.h"

EStateTreeRunStatus USTT_AIActivated::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	switch (Controller->State)
	{
	case EAIState::Attacking:
		FinishTask(true);
		break;
	case EAIState::Idle:
		FinishTask(false);
		break;
	}
	
	return RunStatus;
}
