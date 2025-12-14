// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_CharacterVFX.h"

#include "SCR_HealthComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USCR_CharacterVFX::USCR_CharacterVFX()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void USCR_CharacterVFX::BeginPlay()
{
	Super::BeginPlay();
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}

	PossessionVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
					PossessionVFX,
					PossessionVFXRoot,
					NAME_None,
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::KeepRelativeOffset,
					false,
					false
				);

	// ...
	
}

// Called every frame
void USCR_CharacterVFX::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//ApplyOverlayMaterial();
	// ...
}

void USCR_CharacterVFX::ApplyOverlayMaterial()
{
	USCR_HealthComponent* HealthComp = OwnerCharacter->FindComponentByClass<USCR_HealthComponent>();
	if (HealthComp)
	{
		CurrentHealth = HealthComp->CurrentHealth / HealthComp->MaxHealth;
	}
	
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return;

	if (HighHealthVFXMaterial && MediumHealthVFXMaterial && LowHealthVFXMaterial)
	{
		if (CurrentHealth <= 1.0f && CurrentHealth >= 0.75f)
		{
			if (Mesh->GetOverlayMaterial() != HighHealthVFXMaterial)
			{
				Mesh->SetOverlayMaterial(HighHealthVFXMaterial);
			}
		} else if (CurrentHealth <= .74f && CurrentHealth >= .25f)
		{
			if (Mesh->GetOverlayMaterial() != MediumHealthVFXMaterial)
			{
				Mesh->SetOverlayMaterial(MediumHealthVFXMaterial);
			}
		} else if (CurrentHealth <= .24f)
		{
			if (Mesh->GetOverlayMaterial() != LowHealthVFXMaterial)
			{
				Mesh->SetOverlayMaterial(LowHealthVFXMaterial);
			}
		}

	}
	else
	{
		if (Mesh->GetOverlayMaterial() != nullptr)
		{
			Mesh->SetOverlayMaterial(nullptr);
		}
	}
}

void USCR_CharacterVFX::DisableOverlayMaterial()
{
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (Mesh && Mesh->GetOverlayMaterial() != nullptr)
	{
		Mesh->SetOverlayMaterial(nullptr);
	}
}

void USCR_CharacterVFX::ApplyPossessionEffect()
{
		PossessionVFXComponent->SetVisibility(true, true);
		PossessionVFXComponent->Activate();

}

void USCR_CharacterVFX::RemovePossessionEffect()
{
		PossessionVFXComponent->SetVisibility(false, true);
		PossessionVFXComponent->Deactivate();

}
