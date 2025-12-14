// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team8_GP3/Character/Attacks/SCR_BaseAttackComponent.h"
#include "SCR_MeleeAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_GP3_API USCR_MeleeAttackComponent : public USCR_BaseAttackComponent
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;
};