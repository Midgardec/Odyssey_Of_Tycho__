// Fill out your copyright notice in the Description page of Project Settings.


#include "PChunkBase.h"

#include <stdexcept>
#include <PlanetBase.h>
#include "FastNoiseLite.h"
#include "ProceduralMeshComponent.h"


// Sets default values
APChunkBase::APChunkBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();

	// Mesh Settings
	Mesh->SetCastShadow(false);

	// Set Mesh as root
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void APChunkBase::BeginPlay()
{
	Super::BeginPlay();

	Noise->SetFrequency(Planet->Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);

	Setup();
}

void APChunkBase::GenerateChunk()
{
	bStartedGeneration = true;
	GenerateHeightMap();
	while (!bHMGenerated)
	{
		//
	}
	GenerateMesh();

	//UE_LOG(LogTemp, Warning, TEXT("Vertex Count : %d"), VertexCount);
	while (!bMeshGenerated)
	{
		//
	}
	bMeshApplied = false;
	ApplyMesh();
}

void APChunkBase::RegenerateChunk()
{
	ClearMesh();
	while (!bMeshCleared)
	{
	}
	bMeshCleared = false;
	GenerateChunk();
}

void APChunkBase::GenerateHeightMap()
{
	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DHeightMap(GetActorLocation() / Planet->Resolution);
		break;
	case EGenerationType::GT_2D:
		Generate2DHeightMap(GetActorLocation() / Planet->Resolution);
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}
}

void APChunkBase::ApplyMesh()
{
	Mesh->CreateMeshSection(
		0,
		MeshData.Vertices,
		MeshData.Triangles,
		MeshData.Normals,
		MeshData.UV0,
		MeshData.Colors,
		TArray<FProcMeshTangent>(),
		true
	);
	Mesh->SetMaterial(0, Planet->Material);
	bMeshApplied = true;
}

void APChunkBase::ClearMesh()
{
	VertexCount = 0;
	MeshData.Clear();
	bMeshCleared = true;
}

void APChunkBase::ModifyVoxel(const FIntVector Position, const EBlock Block)
{
	if (Position.X >= Planet->Size || Position.Y >= Planet->Size || Position.Z >= Planet->Size || Position.X < 0 || Position.Y < 0 || Position.Z
		< 0)
		return;

	ModifyVoxelData(Position, Block);

	ClearMesh();
	while (!bMeshCleared)
	{
		//
	}
	GenerateMesh();

	//UE_LOG(LogTemp, Warning, TEXT("Vertex Count : %d"), VertexCount);
	while (!bMeshGenerated)
	{
		//
	}
	bMeshApplied = false;
	ApplyMesh();
}

void APChunkBase::SetVisible(bool newVisibility)
{
	Mesh->SetVisibility(newVisibility);
	if (!newVisibility)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
