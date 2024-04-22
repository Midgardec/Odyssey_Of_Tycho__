// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "ChunkMeshData.h"
#include "Enums.h"

#include "PChunkBase.generated.h"


class FastNoiseLite;
class APlanetBase;
class UProceduralMeshComponent;

UCLASS()
class ODYSSEY_OF_TYCHO___API APChunkBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APChunkBase();
	void GenerateChunk();
	void GenerateChunkAsync();


	bool b_GeneratorBusy = false;
	bool b_ChunkDataReady = false;

	bool b_MeshGenerated = false;
	bool b_AppliedMesh = false;
	//bool bHMGenerated = false;//height map
	//bool bMeshApplied = false;
	//bool bStartedGeneration = false;
	//bool bMeshCleared = false;
	void RegenerateChunk();

	UPROPERTY(EditDefaultsOnly, Category = "Planet")
	APlanetBase* Planet;
	EGenerationType GenerationType;
	UFUNCTION(BlueprintCallable, Category = "Chunk")
	void ModifyVoxel(const FIntVector Position, const EBlock Block);

	void SetVisible(bool);
	int VertexCount = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Setup() PURE_VIRTUAL(AChunkBase::Setup);
	virtual void Generate2DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate2DHeightMap);
	virtual void Generate3DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate3DHeightMap);
	virtual void GenerateMesh() PURE_VIRTUAL(AChunkBase::GenerateMesh);

	virtual void ModifyVoxelData(const FIntVector Position, const EBlock Block) PURE_VIRTUAL(
		AChunkBase::RemoveVoxelData);

	TObjectPtr<UProceduralMeshComponent> Mesh;
	FastNoiseLite* Noise;
	FastNoiseLite* Noise2;
	FastNoiseLite* Noise3;
	FChunkMeshData MeshData;


private:
	void ApplyMesh();
	void ClearMesh();
	void GenerateHeightMap();
};

