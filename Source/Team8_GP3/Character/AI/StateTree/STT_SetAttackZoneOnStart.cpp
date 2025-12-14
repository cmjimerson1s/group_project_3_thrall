// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_SetAttackZoneOnStart.h"

EStateTreeRunStatus USTT_SetAttackZoneOnStart::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Controller->AttackZoneMinDistance = AttackZoneMinDistance;
	Controller->AttackZoneMaxDistance = AttackZoneMaxDistance;
	
	return RunStatus;
}
