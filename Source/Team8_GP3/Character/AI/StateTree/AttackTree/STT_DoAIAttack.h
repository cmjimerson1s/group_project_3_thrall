// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"
#include "STT_DoAIAttack.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_DoAIAttack : public UStateTreeTaskBlueprintBase
{
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base_Controller* Controller;
private:
	GENERATED_BODY()
};
