// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_BaseAttackComponent.h"
#include "SCR_Melee_Component.generated.h"


class USCR_HealthComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_Melee_Component : public USCR_BaseAttackComponent
{
private:
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCR_Melee_Component();

	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	UStaticMeshComponent* Sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float AttackDistance = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float WeaponDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float SwingDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	FRotator SwingStartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	FRotator SwingEndRotation;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void SetSwordMesh(UStaticMeshComponent* SwordMesh);
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PerformAttack();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void UpdateSwing();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void FinishSwing();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PerformHitCheck();
private:
	FTimerHandle SwingTimerHandle;
	float ElapsedTime = 0.f;
	bool bIsSwinging = false;
	bool bHitChecked = false;


};
