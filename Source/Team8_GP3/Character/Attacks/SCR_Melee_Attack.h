// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Melee_Attack.generated.h"

UCLASS()
class TEAM8_GP3_API ASCR_Melee_Attack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCR_Melee_Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
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
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PerformAttack();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void UpdateSwing();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void FinishSwing();
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PerformHitCheck();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	bool bIsSwinging = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	bool bHitChecked = false;
private:
	FTimerHandle SwingTimerHandle;
	float ElapsedTime = 0.f;

};


