// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"
#include "STT_SetAttackZoneOnStart.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_SetAttackZoneOnStart : public UStateTreeTaskBlueprintBase
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Context")
	ASCR_AI_Base_Controller* Controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float AttackZoneMinDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float AttackZoneMaxDistance = 100.0f;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;

private:
	GENERATED_BODY()
};
