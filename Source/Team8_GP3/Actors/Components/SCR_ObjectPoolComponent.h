// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_ObjectPoolComponent.generated.h"


enum class ECharacterType : uint8;
class ASCR_AI_Base;
class AAIManager;
class USCR_ActivatableComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_ObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	USCR_ObjectPoolComponent();
	
	UFUNCTION(BlueprintCallable, Category="Pooling")
	ASCR_AI_Base* GetPooledActor(ECharacterType type);

	UFUNCTION(BlueprintCallable, Category="Pooling")
	void SpawnUnits(ECharacterType unitType, int ammount);

	UFUNCTION(BlueprintCallable, Category="Pooling")
	void ClearPool();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectPool|Class Assignmenet")
	TSubclassOf<ASCR_AI_Base> RangedAgentsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectPool|Class Assignmenet")
	TSubclassOf<ASCR_AI_Base> MeleeAgentsClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectPool|Pooling")
	TArray<ASCR_AI_Base*> RangedPool;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectPool|Pooling")
	TArray<ASCR_AI_Base*> MeleePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectPool|Pooling")
	int32 PoolSize = 10;

	UPROPERTY(Transient)
	AAIManager* Owner;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 CurrentPoolIndex = 0;

	void TopUpPool(ECharacterType type);

	void CleanupPool();
};
