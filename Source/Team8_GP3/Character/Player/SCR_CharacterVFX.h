// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_CharacterVFX.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_CharacterVFX : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCR_CharacterVFX();
	//Health Outline Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthVFX")
	UMaterialInstance* HighHealthVFXMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthVFX")
	UMaterialInstance* MediumHealthVFXMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthVFX")
	UMaterialInstance* LowHealthVFXMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthVFX")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsColorOn;
	UFUNCTION(BlueprintCallable)
	void ApplyOverlayMaterial();
	UFUNCTION(BlueprintCallable)
	void DisableOverlayMaterial();
	//Possession VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PossessionVFX")
	UNiagaraSystem* PossessionVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PossessionVFX")
	UNiagaraComponent* PossessionVFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PossessionVFX")
	USceneComponent* PossessionVFXRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PossessionVFX")
	bool bIsPossessionVFX;
	UFUNCTION(BlueprintCallable)
	void ApplyPossessionEffect();
	UFUNCTION(BlueprintCallable)
	void RemovePossessionEffect();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
