// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SCR_Interactable.h"
#include "SCR_MemoryShard.generated.h"

UCLASS()
class TEAM8_GP3_API ASCR_MemoryShard : public AActor 
{
	GENERATED_BODY()

public:
	ASCR_MemoryShard();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USCR_Interactable* InteractableComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasLore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShardText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShardTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SubtitleText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* MemoryAudioFile;

	UFUNCTION(BlueprintCallable)
	void AddToAllMemories(ASCR_MemoryShard* MemoryShard);
	

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
