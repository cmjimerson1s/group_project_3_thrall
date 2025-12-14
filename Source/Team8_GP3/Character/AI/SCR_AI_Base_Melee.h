// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCR_AI_Base.h"
#include "SCR_AI_Base_Melee.generated.h"

UCLASS()
class TEAM8_GP3_API ASCR_AI_Base_Melee : public ASCR_AI_Base
{
public:
	// Sets default values for this character's properties
	ASCR_AI_Base_Melee();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

	

	
};
