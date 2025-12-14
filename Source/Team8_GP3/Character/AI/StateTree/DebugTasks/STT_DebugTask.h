// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_DebugTask.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_DebugTask : public UStateTreeTaskBlueprintBase
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
	bool TrueBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldFinish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FinishSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowPrintTextToScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllowPrintTextToLog;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TextToPrint = "Hello";

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;
	
private:
	GENERATED_BODY()

	UFUNCTION()
	void PrintTextToScreen();

	UFUNCTION()
	void PrintTextToLog();

	
};
