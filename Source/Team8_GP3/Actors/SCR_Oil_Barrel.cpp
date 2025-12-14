// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_Oil_Barrel.h"

// Sets default values
ASCR_Oil_Barrel::ASCR_Oil_Barrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BarrelBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelBody"));
	RootComponent = BarrelBody;
	ExplosionArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosionArea"));
	ExplosionArea->SetupAttachment(RootComponent);
	SpillArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpillArea"));
	SpillArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASCR_Oil_Barrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCR_Oil_Barrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsSpilling && SpillArea)
	{
		SpillElapsed += DeltaTime;
		float Alpha = FMath::Clamp(SpillElapsed / SpillDuration, 0.f, 1.f);
		float XYScale = FMath::Lerp(0.f, SpillRadius, Alpha);
		FVector CurrentScale = SpillArea->GetComponentScale();
		SpillArea->SetWorldScale3D(FVector(XYScale, XYScale, CurrentScale.Z));
		if (Alpha >= 1.f)
		{
			bIsSpilling = false;
		}
	}
	if (bIsExploding && ExplosionArea)
	{
		ExplosionElapsed += DeltaTime;
		float Alpha = FMath::Clamp(ExplosionElapsed / ExplosionDuration, 0.f, 1.f);
		float Scale = FMath::Lerp(0.f, ExplosionRadius, Alpha);
		ExplosionArea->SetWorldScale3D(FVector(Scale));

		if (Alpha >= 1.f)
		{
			bIsExploding = false;
			ExplosionArea->SetVisibility(false);
			ExplosionArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
void ASCR_Oil_Barrel::Explode()
{
	if (ExplosionArea)
	{
		ExplosionElapsed = 0.f;
		bIsExploding = true;
		ExplosionArea->SetWorldScale3D(FVector::ZeroVector);
		ExplosionArea->SetVisibility(true);
	}
}

void ASCR_Oil_Barrel::Spill()
{
	if (SpillArea)
	{
		SpillElapsed = 0.f;
		bIsSpilling = true;
		FVector InitialScale = SpillArea->GetComponentScale();
		SpillArea->SetWorldScale3D(FVector(0.f, 0.f, InitialScale.Z)); // keep Z scale
		SpillArea->SetVisibility(true);
	}
}
