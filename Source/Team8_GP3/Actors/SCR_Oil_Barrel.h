// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Oil_Barrel.generated.h"

UCLASS()
class TEAM8_GP3_API ASCR_Oil_Barrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCR_Oil_Barrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BarrelBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	UStaticMeshComponent* ExplosionArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool bIsExploding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionElapsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spill")
	UStaticMeshComponent* SpillArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spill")
	float SpillRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spill")
	bool bIsSpilling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spill")
	float SpillElapsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spill")
	float SpillDuration;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintCallable)
	void Spill();
};


