#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Checkpoint.generated.h"

class UBoxComponent;
class USCR_ActivatableComponent;

UCLASS()
class TEAM8_GP3_API ASCR_Checkpoint : public AActor
{
	GENERATED_BODY()

public:
	ASCR_Checkpoint();

	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	// 					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	// 					bool bFromSweep, const FHitResult& SweepResult);

public:
	
	UFUNCTION(BlueprintCallable)//, CallInEditor, Category="MyCategory")
	void Trigger();

	UPROPERTY(EditAnywhere)
	FRotator Rotation;

	UPROPERTY(EditAnywhere)
	FVector Location;
	
	UPROPERTY(VisibleAnywhere)
	USCR_ActivatableComponent* ActivatableComponent;

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;
};
