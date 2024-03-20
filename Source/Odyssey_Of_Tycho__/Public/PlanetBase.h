// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Enums.h"

#include "PlanetBase.generated.h"


class ATycho_Character;
class APChunkBase;

UCLASS()
class ODYSSEY_OF_TYCHO___API APlanetBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category="World")
	TSubclassOf<APChunkBase> ChunkType;
	
	UPROPERTY(EditInstanceOnly, Category="World")
	int DrawDistance = 5;

	UPROPERTY(EditInstanceOnly, Category="Chunk")
	TObjectPtr<UMaterialInterface> Material;
	
	UPROPERTY(EditInstanceOnly, Category="Chunk")
	int Size = 32;

	UPROPERTY(EditInstanceOnly, Category="Chunk|Size*Resolution const")
	int BaseResolution = 32;

	UPROPERTY(EditInstanceOnly, Category="Chunk")
	int Resolution = 100;
	
	UPROPERTY(EditInstanceOnly, Category="Height Map")
	EGenerationType GenerationType;

	UPROPERTY(EditInstanceOnly, Category="Height Map")
	float Frequency = 0.03f;
	
	// Sets default values for this actor's properties
	APlanetBase();

	UPROPERTY(EditInstanceOnly, Category="Chunk")
	int Radius = 500;

	UPROPERTY(EditInstanceOnly, Category="Chunk")
	FVector Origin = FVector(0,0,0);

	UPROPERTY(EditInstanceOnly, Category="Planet|Water|Level")
	float TerainLevel_Water;
	UPROPERTY(EditInstanceOnly, Category="Planet|Low Ground|Level")
	float TerainLevel_Low;
	UPROPERTY(EditInstanceOnly, Category="Planet|High Ground|Level")
	float TerainLevel_High;
	UPROPERTY(EditInstanceOnly, Category="Planet|Top Ground|Level")
	float TerainLevel_Top;
	
	UPROPERTY(EditInstanceOnly, Category="Planet|Water|Color")
	FColor TerainColor_Water;
	UPROPERTY(EditInstanceOnly, Category="Planet|Low Ground|Color")
	FColor TerainColor_Low;
	UPROPERTY(EditInstanceOnly, Category="Planet|High Ground|Color")
	FColor TerainColor_High;
	UPROPERTY(EditInstanceOnly, Category="Planet|Top Ground|Color")
	FColor TerainColor_Top;
	
	int64 VertexSum;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	int ChunkCount;
	TArray<TObjectPtr<APChunkBase>> chunks;
	TObjectPtr<APawn> Player;
	void Generate3DWorld();
	void Generate2DWorld();
};
