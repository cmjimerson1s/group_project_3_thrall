// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_SliceTest.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASCR_SliceTest::ASCR_SliceTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	// Create ProceduralMeshComponent
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProcMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASCR_SliceTest::BeginPlay()
{
	Super::BeginPlay();
	ConvertToProceduralMesh();
}

// Called every frame
void ASCR_SliceTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCR_SliceTest::ConvertToProceduralMesh()
{
	if (!StaticMesh || !StaticMesh->GetStaticMesh()) return;
	
	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(
		StaticMesh,
		0,
		ProcMesh,
		true
	);

	StaticMesh->DestroyComponent(); // Remove StaticMesh
	ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMesh->SetCollisionObjectType(ECC_PhysicsBody);
	ProcMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ProcMesh->bUseComplexAsSimpleCollision = false;
	//
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	// Extract mesh data from static mesh
	UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
		StaticMesh->GetStaticMesh(),
		0, // LOD 0
		0, // Section 0
		Vertices,
		Triangles,
		Normals,
		UV0,
		Tangents
	);

	// Fill in default white colors
	VertexColors.Init(FLinearColor::White, Vertices.Num());

	// Create procedural mesh with collision
	ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	// Set simulate physics AFTER setting collision
	ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMesh->SetSimulatePhysics(true);
	if (!ProcMesh->ContainsPhysicsTriMeshData(true))
	{
		UE_LOG(LogTemp, Warning, TEXT("ProceduralMesh has no collision data."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProceduralMesh collision is OK."));
	}
}

void ASCR_SliceTest::Slice(const FVector& SliceOrigin, const FVector& SliceNormal)
{
	UProceduralMeshComponent* OtherHalf = NewObject<UProceduralMeshComponent>(this);
	OtherHalf->RegisterComponent();
	OtherHalf->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	OtherHalf->SetWorldLocation(GetActorLocation());

	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		ProcMesh,
		SliceOrigin,
		SliceNormal,
		true,
		OtherHalf,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		CapMaterial
	);

	// Enable physics on the sliced halves
	ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMesh->SetSimulatePhysics(true);

	OtherHalf->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OtherHalf->SetSimulatePhysics(true);
}


