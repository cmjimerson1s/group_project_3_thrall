// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "SCR_ProjectileEffectBase.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Team8_GP3/Actors/Components/SCR_ActivatableComponent.h"
#include "NiagaraComponent.h"
#include "SCR_Projectile.generated.h"


UCLASS(Blueprintable)
class TEAM8_GP3_API ASCR_Projectile : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ASCR_Projectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USCR_ActivatableComponent* ActivatableComponent;

    

    UPROPERTY(Transient)
    AActor* StuckToActor = nullptr;

    UPROPERTY(Transient)
    UPrimitiveComponent* StuckToComponent = nullptr;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Initialize the projectile with custom parameters
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void LaunchProjectile(FVector AimVector);

    // Components
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = Movement)
    UProjectileMovementComponent* ProjectileMovement;

    // Projectile mesh
    UPROPERTY(VisibleAnywhere, Category = Mesh)
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Effects")
    UNiagaraComponent* NiagaraTrailEffect;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Effects")
    // UNiagaraSystem* NiagaraTrailSystem;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Effects")
    // UNiagaraSystem* NiagaraImpactSystem;

    // Projectile settings

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    float InitialSpeed = 3000.f; // Initial speed of the projectile
    
    UPROPERTY(EditAnywhere, Category = "Projectile Settings")
    float InitialGravityDelay = 0.5f; // Time before gravity starts affecting the projectile
    
    UPROPERTY(EditAnywhere, Category = "Projectile Settings")
    float CustomGravityScale = 1.0f; // Multiplier for gravity effect
    
    UPROPERTY(EditAnywhere, Category = "Projectile Settings")
    bool bIsTimedProjectile = false; // If true, projectile will disappear after Lifetime

    UPROPERTY(EditAnywhere, Category = "Projectile Settings", meta = (EditCondition = "bIsTimedProjectile"))
    bool bActivateOnExpired = true; // If true, the projectile will activate its effect on expiration
    
    UPROPERTY(EditAnywhere, Category = "Projectile Settings", meta = (EditCondition = "bIsTimedProjectile"))
    float ProjectileLifetime = 5.0f; // Time before projectile disappears (if bIsTimedProjectile is true)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    float PenetrationSpeedLoss = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    float MinPenetrationSpeed = 500.f; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    int MaxPenetration = 3;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    bool AutoGravityOnPenetration = true; // If true, gravity will be enabled after the first penetration

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    bool bDeleteOnStop = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    bool bIgnorePlayer = false; // If true, the projectile will not affect the player

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
    bool bIgnoreEnemy = false; // If true, the projectile will not affect enemies
  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings", meta = (EditInline))
    TSubclassOf<USCR_ProjectileEffectBase> ProjectileEffectClass;
    
    UFUNCTION(BlueprintCallable)
    void OnProjectileExpired();

    UFUNCTION(BlueprintCallable)
    void SetPhysicsEnabled(bool bEnabled);

    UFUNCTION(Blueprintable)
    void OnProjectileMovementComponentStopped(const FHitResult& ImpactResult)
    {
        SetPhysicsEnabled(false);
    }


    UFUNCTION()
    void OnStuckToActorDestroyed(AActor* DestroyedActor);

    UFUNCTION(BlueprintCallable, Category = "Projectile|Effects")
    void ActivateTrailEffect();

    UFUNCTION(BlueprintCallable, Category = "Projectile|Effects")
    void DeactivateTrailEffect();
    
    UFUNCTION()
    void HandleProjectileDestruction();

    UFUNCTION()
    void ResetProjectile();

    // UFUNCTION(BlueprintCallable, Category = "Projectile|Effects")
    // void SpawnImpactEffect(const FHitResult& HitResult);


private:
    int CurrentPenetrationCount = 0; // Current number of penetrations
    
    // Timer handle for gravity activation
    FTimerHandle GravityTimerHandle;

    // Timer handle for projectile lifetime
    FTimerHandle LifetimeTimerHandle;

    // Enable gravity after delay
    void EnableGravity();

    // Destroy Projectile when lifetime expires
    void DestroyDueToTimeout();
    
    bool bIsActive = false;
    
    // Modify EndOfProjectile to be callable from Blueprint
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void EndOfProjectile();

    

    // Handle hit events
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};