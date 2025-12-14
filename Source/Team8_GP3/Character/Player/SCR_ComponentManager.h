// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCR_CharacterVFX.h"
#include "SCR_Dash_Component.h"
#include "SCR_HealthComponent.h"
#include "Components/ActorComponent.h"
#include "SCR_ComponentManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_ComponentManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USCR_ComponentManager();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USCR_HealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USCR_Dash_Component* DashComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USCR_Possession_Component* PossessionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USCR_CharacterVFX* CharacterVFX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* OwnerCharacter;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
