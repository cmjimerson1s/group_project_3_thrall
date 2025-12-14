// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/AIController/SCR_AI_Base_Controller.h"
#include "STT_ToggleFocusTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_ToggleFocusTarget : public UStateTreeTaskBlueprintBase
{
protected:
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base_Controller* Controller;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldFocus = true;

private:
	GENERATED_BODY()
};
