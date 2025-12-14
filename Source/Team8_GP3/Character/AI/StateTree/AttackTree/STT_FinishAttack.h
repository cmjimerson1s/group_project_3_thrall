// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "STT_FinishAttack.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_FinishAttack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
        	ASCR_AI_Base* Character;
};
