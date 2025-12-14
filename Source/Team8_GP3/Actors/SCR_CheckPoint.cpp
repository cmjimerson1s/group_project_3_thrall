#include "SCR_Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SCR_ActivatableComponent.h"
#include "Team8_GP3/CustomMacros.h"
#include "Team8_GP3/PD_GameInstance.h"
#include "Team8_GP3/Character/Player/SCR_Possession_Component.h"
#include "Team8_GP3/GameModes/SCR_GMPrototype.h"

ASCR_Checkpoint::ASCR_Checkpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName("Trigger");
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASCR_Checkpoint::OnOverlapEnd);

}

void ASCR_Checkpoint::BeginPlay()
{
	Super::BeginPlay();

	ActivatableComponent = NewObject<USCR_ActivatableComponent>(this, USCR_ActivatableComponent::StaticClass());
	if (ActivatableComponent)
	{
		ActivatableComponent->RegisterComponent();
	}

	Rotation = GetActorRotation(); 
	
	// Raytrace down to move Location to the ground
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - FVector(0.f, 0.f, 10000.f), ECC_Visibility))
	{
		Location = HitResult.Location;
		UE_LOG(LogTemp, Display, TEXT("Location: %f, %f, %f"), Location.X, Location.Y, Location.Z);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint location not adjusted to ground!"));
		Location = GetActorLocation();
	}
	
}

void ASCR_Checkpoint::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<ACharacter>())
	{
		if (USCR_Possession_Component* PC = OtherActor->FindComponentByClass<USCR_Possession_Component>())
		{
			if (PC->bIsPlayer)
			{
				Trigger();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI Overlapped Checkpoint!"));
			}
		}
		
	}
}

void ASCR_Checkpoint::Trigger()
{
	UPD_GameInstance* GameInstance = Cast<UPD_GameInstance>(GetGameInstance());
	GameInstance->PlayerSpawnPointPosition = GetActorLocation();
	GameInstance->PlayerSpawnPointRotation = GetActorRotation();
	GameInstance->FirstTimePlay = false;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Checkpoint Triggered!"));

	if (ASCR_GMPrototype* GM = GetWorld()->GetAuthGameMode<ASCR_GMPrototype>())
	{
		FTransform SaveTransform = FTransform(Rotation, Location);
        GM->SaveGame(SaveTransform);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved!"));
	}
	
	if (ActivatableComponent)
	{
		ActivatableComponent->Disable();
	}
}
