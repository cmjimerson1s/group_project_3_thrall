// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_GenerateRandom.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_GenerateRandom : public UStateTreeTaskBlueprintBase
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinRange = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
	int32 ReturnedInt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
	float ReturnedFloat = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
	bool RandomBool = false;
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;

private:
	GENERATED_BODY()


};
