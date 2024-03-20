// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetBase.h"
#include "Generators/MarchingCubes.h"
#include "Math/UnrealMathUtility.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"

#include "PChunkBase.h"
#include "Tycho_character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlanetBase::APlanetBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlanetBase::BeginPlay()
{
	Super::BeginPlay();
	Origin = this->GetActorLocation() + FVector(Size * Resolution /2, Size * Resolution/2 , Size * Resolution/2 );
	//Origin = this->GetActorLocation();
	Radius = FMath::Clamp(Radius, Resolution, (DrawDistance*2+1)*Size*Resolution/2 - Resolution);
	GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Silver, FString::Printf(TEXT("Radius: %i"),Radius));
	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DWorld();
		break;
	case EGenerationType::GT_2D:
		Generate2DWorld();
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d Chunks Created"), ChunkCount);
}

void APlanetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		//this->VertexSum = 0;
		for (auto ch : chunks)
        	{
				if(!ch){break;}
				if(!ch->bStartedGeneration)
				{
					ch->GenerateChunk();
					
					
				}
			
				//this->VertexSum +=ch->VertexCount;
				/*if (FVector::Distance(Player->GetActorLocation(), ch->GetActorLocation()) > 10000)
        		{
        			ch->SetVisible(false);
        		}
        		else
        		{
        			ch->SetVisible(true);
        		}*/
        	}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("vertex count: %lld"), this->VertexSum));
}

void APlanetBase::Generate3DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			for (int z = -DrawDistance; z <= DrawDistance; ++z)
			{
				auto transform = FTransform(
					FRotator::ZeroRotator,
					FVector(this->GetActorLocation().X + x * Size * Resolution, this->GetActorLocation().Y + y * Size * Resolution,
					        this->GetActorLocation().Z + z * Size * Resolution),
					FVector::OneVector
				);

				const auto chunk = GetWorld()->SpawnActorDeferred<APChunkBase>(
					ChunkType,
					transform,
					this
				);

				chunk->GenerationType = EGenerationType::GT_3D;
				chunk->Frequency = Frequency;
				chunk->Material = Material;
				chunk->Size = Size;
				chunk->Radius = Radius;
				chunk->Origin = Origin;
				chunk->Resolution = Resolution;

				chunks.Add(chunk);
				UGameplayStatics::FinishSpawningActor(chunk, transform);

				ChunkCount++;
			}
		}
	}
}

void APlanetBase::Generate2DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			auto transform = FTransform(
				FRotator::ZeroRotator,
				FVector(x * Size * 100, y * Size * 100, 0),
				FVector::OneVector
			);

			const auto chunk = GetWorld()->SpawnActorDeferred<APChunkBase>(
				ChunkType,
				transform,
				this
			);

			chunk->GenerationType = EGenerationType::GT_2D;
			chunk->Frequency = Frequency;
			chunk->Material = Material;
			chunk->Size = Size;

			UGameplayStatics::FinishSpawningActor(chunk, transform);

			ChunkCount++;
		}
	}
}
