// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCR_Projectile.h"
#include "Components/ActorComponent.h"
#include "Team8_GP3/Actors/SCR_ObjectPool.h"
#include "Team8_GP3/Character/Attacks/SCR_BaseAttackComponent.h"
#include "SCR_ProjectileLauncherComponent.generated.h"


class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowSpawn, AActor*, ProjectileActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_GP3_API USCR_ProjectileLauncherComponent : public USCR_BaseAttackComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCR_ProjectileLauncherComponent();

	UFUNCTION(BlueprintCallable)
	void SetSpawnPoints(USceneComponent* NewAIProjectileSpawnPoint, 
	                   USceneComponent* NewPlayerProjectileSpawnPoint);
	UFUNCTION()
	void TestFunction();

	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Pool", meta =(EditInline))
	TSubclassOf<ASCR_Projectile> AIProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool", meta =(EditInline))
	TSubclassOf<ASCR_Projectile> PlayerProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool", meta =(EditInline))
	ASCR_ObjectPool* AIProjectilePool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool", meta =(EditInline))
	ASCR_ObjectPool* PlayerProjectilePool = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> AISpawnPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> PlayerSpawnPoint;
	
	UPROPERTY(EditAnywhere)
	float ReloadTime = 1.f;

	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void SetIsPlayer(bool Player);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnArrowSpawn ArrowSpawn;



	UFUNCTION()
	bool GetIsPlayer() const
	{
		return bIsPlayer;
	}

	UPROPERTY(BlueprintAssignable)
	FOnAttackDelegate OnAttack;
protected:
	// Called when the game starts

	UPROPERTY(Transient)
	ASCR_Projectile* Projectile = nullptr;
	//Only for testing
	UPROPERTY(EditAnywhere)
	bool DebugFireTest = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* AttackInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;
	
	virtual void BeginPlay() override;
	
	void FireProjectile();
	
	FTimerHandle ReloadTimerHandle;

private:
	float CurrentReloadTime = 0.f;

	bool bIsPlayer = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	ACharacter* OwnerCharacter;
	void BindInput(UEnhancedInputComponent* InputComponent);
	void InitProjectiles();
	bool SetProjectilePools();
};


