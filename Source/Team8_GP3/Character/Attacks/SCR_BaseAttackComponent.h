// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team8_GP3/Interfaces/Attack.h"
#include "SCR_BaseAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_BaseAttackComponent : public UActorComponent, public IAttack
{
public:
	virtual void Attack() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USCR_BaseAttackComponent();
};
