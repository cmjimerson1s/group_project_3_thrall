// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SCR_SliceTest.generated.h"

UCLASS()
class TEAM8_GP3_API ASCR_SliceTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCR_SliceTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	UMaterialInterface* CapMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Slice(const FVector& SliceOrigin, const FVector& SliceNormal);

private:
	void ConvertToProceduralMesh();

};
