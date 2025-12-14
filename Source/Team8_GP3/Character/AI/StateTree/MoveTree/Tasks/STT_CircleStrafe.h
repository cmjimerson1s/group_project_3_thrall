// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"
#include "STT_CircleStrafe.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_CircleStrafe : public UStateTreeTaskBlueprintBase
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base_Controller* Controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base* Character;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float StrafeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WalkRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerDistance DistanceToPlayer;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

private:
	GENERATED_BODY()

	UPROPERTY()
	float DirectionModifier = 100.0f;
	
	UPROPERTY()
	float ForwardModifier = 100.0f;
};
