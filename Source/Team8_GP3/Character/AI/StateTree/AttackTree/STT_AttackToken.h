// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"
#include "Team8_GP3/Character/AI/SCR_AI_Base.h"
#include "STT_AttackToken.generated.h"

UENUM(BlueprintType)
enum class ETokenAction : uint8
{
	Reserve     UMETA(DisplayName = "Reserve"),
	Return		UMETA(DisplayName = "Return")
};

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USTT_AttackToken : public UStateTreeTaskBlueprintBase
{
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) override;

	UFUNCTION()
	bool ReturnToken() const;

	UFUNCTION()
	bool ReserveToken() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETokenAction TokenAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int AttackCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	ASCR_AI_Base* Character;
	
private:
	GENERATED_BODY()
};
