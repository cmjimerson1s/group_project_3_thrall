// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_AttackToken.h"

EStateTreeRunStatus USTT_AttackToken::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	if (TokenAction == ETokenAction::Reserve)
		FinishTask(ReserveToken());
	else
		FinishTask(ReturnToken());
	
	return RunStatus;
}

bool USTT_AttackToken::ReturnToken() const
{
	if (Character->StoredTokens == 0)
		return true;
	
	GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->ReturnAttackToken(Type, Character->StoredTokens);

	Character->StoredTokens = 0;
	
	return true;
}

bool USTT_AttackToken::ReserveToken() const
{
	if (Character->StoredTokens > 0)
		return true;

	auto success = GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->ReserveAttackToken(Type, AttackCost);

	if (success)
		Character->StoredTokens += AttackCost;

	return success;
}
