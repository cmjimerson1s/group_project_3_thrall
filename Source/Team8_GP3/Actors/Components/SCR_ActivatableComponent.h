#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCR_ActivatableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivatedEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM8_GP3_API USCR_ActivatableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USCR_ActivatableComponent();

    AActor* Owner;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Activatable")
    bool bIsEnabled = true;
    
    UFUNCTION(BlueprintCallable, Category="Activatable")
    bool SetEnabled(bool enabled);

    UFUNCTION(BlueprintCallable,CallInEditor, Category="Activatable")
    void Enable();

    UFUNCTION(BlueprintCallable,CallInEditor, Category="Activatable")
    void Disable() ;

    UFUNCTION(BlueprintCallable,CallInEditor, Category="Activatable")
    void Toggle();

    UPROPERTY(BlueprintAssignable, Category="Activatable")
    FOnActivatedEvent OnEnable;

    UPROPERTY(BlueprintAssignable, Category="Activatable")
    FOnActivatedEvent OnDisable;

    //TO USE EVENTS
    //ActivatableComponent->OnEnable.AddDynamic(this, &AMyActor::MyFunction);
};
