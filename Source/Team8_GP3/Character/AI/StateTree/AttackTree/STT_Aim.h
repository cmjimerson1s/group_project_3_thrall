// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"
#include "STT_Aim.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_Aim : public UStateTreeTaskBlueprintBase
{
protected:
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
	                                       const FStateTreeTransitionResult& Transition) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base_Controller* Controller;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GoRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Output")
	FVector MoveTargetLocation;

	UPROPERTY()
	float MultiplyValue = 100.0f;
	
private:
	GENERATED_BODY()

	

	
};
