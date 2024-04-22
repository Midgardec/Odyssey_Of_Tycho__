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
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();
	Noise2 = new FastNoiseLite();
	Noise3 = new FastNoiseLite();

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

	Noise2->SetFrequency(Planet->Frequency2);
	Noise2->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	Noise2->SetFractalType(FastNoiseLite::FractalType_FBm);

	Noise3->SetFrequency(Planet->Frequency3);
	Noise3->SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	Noise3->SetFractalType(FastNoiseLite::FractalType_FBm);

	Setup();
}
void APChunkBase::Tick(float DeltaTime)
{
	if (b_ChunkDataReady && !b_AppliedMesh) {
		ApplyMesh();
		b_AppliedMesh = true;
	}
}
void APChunkBase::GenerateChunk()
{
	GenerateHeightMap();
	GenerateMesh();
	while (!b_ChunkDataReady) {

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f"), b_ChunkDataReady));
	}
	b_AppliedMesh = false;
	//ApplyMesh();
}

void APChunkBase::GenerateChunkAsync()
{
	b_GeneratorBusy = true;
	/*AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]()
		{
			auto WorldGenTask = new FAsyncTask<FAsyncChunkGenerator>(this);
			WorldGenTask->StartBackgroundTask();
			WorldGenTask->EnsureCompletion();
			delete WorldGenTask;
		});*/
	GenerateChunk();
	while (!b_ChunkDataReady)
	{
		//
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f"), b_ChunkDataReady));
	}
	
}

void APChunkBase::RegenerateChunk()
{
	ClearMesh();
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
	Mesh->SetMaterial(0, Planet->Material);
	Mesh->CreateMeshSection_LinearColor(
		0,
		MeshData.Vertices,
		MeshData.Triangles,
		MeshData.Normals,
		MeshData.UV0,
		MeshData.Colors,
		TArray<FProcMeshTangent>(),
		true
	);
	b_ChunkDataReady = false;
	b_GeneratorBusy = false;
	
}

void APChunkBase::ClearMesh()
{
	VertexCount = 0;
	MeshData.Clear();
}

void APChunkBase::ModifyVoxel(const FIntVector Position, const EBlock Block)
{
	if (Position.X >= Planet->Size || Position.Y >= Planet->Size || Position.Z >= Planet->Size || Position.X < 0 || Position.Y < 0 || Position.Z
		< 0)
		return;

	ModifyVoxelData(Position, Block);

	ClearMesh();
	GenerateMesh();

	//UE_LOG(LogTemp, Warning, TEXT("Vertex Count : %d"), VertexCount);
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

//void FAsyncChunkGenerator::DoWork()
//{
//	ChunkGenerator->GenerateChunk();
//}
