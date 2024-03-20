// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "ChunkMeshData.h"
#include "Enums.h"

#include "PChunkBase.generated.h"


class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class ODYSSEY_OF_TYCHO___API APChunkBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APChunkBase();
	void GenerateChunk();

	bool bMeshGenerated = false;
	bool bHMGenerated = false;//height map
	bool bMeshApplied = false;
	bool bStartedGeneration = false;
	bool bMeshCleared = false;
	void RegenerateChunk();
	
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	int Size = 64;
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	FVector Origin = FVector(0, 0, 0);
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	float Radius = 500;
	UPROPERTY(EditDefaultsOnly, Category="Chunk")
	float Resolution = 500;
	
	TObjectPtr<UMaterialInterface> Material;
	float Frequency;
	EGenerationType GenerationType;

	UFUNCTION(BlueprintCallable, Category="Chunk")
	void ModifyVoxel(const FIntVector Position, const EBlock Block);

	void SetVisible(bool);
	int VertexCount = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Setup() PURE_VIRTUAL(AChunkBase::Setup);
	virtual void Generate2DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate2DHeightMap);
	virtual void Generate3DHeightMap(const FVector Position) PURE_VIRTUAL(AChunkBase::Generate3DHeightMap);
	virtual void GenerateMesh() PURE_VIRTUAL(AChunkBase::GenerateMesh);
	
	virtual void ModifyVoxelData(const FIntVector Position, const EBlock Block) PURE_VIRTUAL(
		AChunkBase::RemoveVoxelData);

	TObjectPtr<UProceduralMeshComponent> Mesh;
	FastNoiseLite* Noise;
	FChunkMeshData MeshData;
	

	
private:
	void ApplyMesh();
	void ClearMesh();
	void GenerateHeightMap();
};
