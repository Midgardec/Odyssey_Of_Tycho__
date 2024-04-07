// Fill out your copyright notice in the Description page of Project Settings.


#include "PMarchingChunkBase.h"


#include "FastNoiseLite.h"
#include "PlanetBase.h"
#include "ProceduralMeshComponent.h"

void APMarchingChunkBase::Setup()
{
	// Initialize Voxels
	Voxels.SetNum((Planet->Size + 1) * (Planet->Size + 1) * (Planet->Size + 1));
}

void APMarchingChunkBase::Generate2DHeightMap(const FVector Position)
{
	for (int x = 0; x <= Planet->Size; x++)
	{
		for (int y = 0; y <= Planet->Size; y++)
		{
			const float Xpos = x + Position.X;
			const float ypos = y + Position.Y;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, ypos) + 1) * Planet->Size / 2), 0,
				Planet->Size);

			for (int z = 0; z < Height; z++)
			{
				Voxels[GetVoxelIndex(x, y, z)] = 1.0f;
			}

			for (int z = Height; z < Planet->Size; z++)
			{
				Voxels[GetVoxelIndex(x, y, z)] = -1.0f;
			}
		}
	}
}

float APMarchingChunkBase::CalculateValue(int x, int y, int z, FVector Pos) {
	// Получаем индекс текущего воксела

	// Вычисляем расстояние от центра куба до текущего воксела
	float distanceFromCenter = sqrt(pow(this->GetActorLocation().X + x * Planet->Resolution - Planet->Origin.X, 2) +
		pow(this->GetActorLocation().Y + y * Planet->Resolution - Planet->Origin.Y, 2) +
		pow(this->GetActorLocation().Z + z * Planet->Resolution - Planet->Origin.Z, 2));

	float noise = 0;
	float density=0;// = Planet->Radius - distanceFromCenter;
	//// Применяем комбинацию шумовых функций для генерации рельефа
	//float baseNoise = (Noise2->GetNoise(x + Position.X, y + Position.Y, z + Position.Z) + 1) / 2; // Базовый шум от 0 до 1
	//float detailNoise = FMath::Abs(Noise3->GetNoise(x * 4 + Position.X, y * 4 + Position.Y, z * 4 + Position.Z)); // Детальный шум для создания расщелин и пещер
	//float noise = baseNoise * 0.7f + detailNoise * 0.3f; // Комбинируем базовый и детальный шум
	/*if (!(distanceFromCenter <= (Planet->Radius - Planet->TerrainHeight))) {*/

	float freq = Planet->Frequency2;
	float amplitude = 1.f;
	for (int i = 0; i < 5; i++) {
		Noise2->SetFrequency(freq);
		density += Noise2->GetNoise(x + Pos.X, y + Pos.Y, z + Pos.Z) * amplitude;
		freq *= 2;
		amplitude *= 0.5;
	}
	density = density * (1.0f - distanceFromCenter / Planet->Radius);
/*}
else {
	density = 1 * (1.0f - distanceFromCenter / (Planet->Radius - Planet->TerrainHeight));
}*/

// Вычисляем значение плотности воксела с использованием экспоненциального спада
// Ограничиваем значение плотности в диапазоне от 0 до 1
//density = FMath::Clamp(density, -1.0f, 1.0f);

float BaseLevelRadius = Planet->Radius;

if (distanceFromCenter > BaseLevelRadius)
	return -abs(density);

	return density;
}

void APMarchingChunkBase::Generate3DHeightMap(const FVector Position)
{
	for (int x = 0; x <= Planet->Size; ++x)
	{
		for (int y = 0; y <= Planet->Size; ++y)
		{
			for (int z = 0; z <= Planet->Size; ++z)
			{
				float dens = CalculateValue(x, y, z, Position);

				Voxels[GetVoxelIndex(x, y, z)] = dens;

			}
		}
	}
}

bool APMarchingChunkBase::pointInSphere(int x, int y, int z)
{
	float dx = x - Planet->Origin.X;
	float dy = y - Planet->Origin.Y;
	float dz = z - Planet->Origin.Z;

	float distance = FMath::Sqrt(dx * dx + dy * dy + dz * dz);
	return distance <= Planet->Radius - Planet->TerrainHeight + Planet->Resolution * 0.9;
}

bool APMarchingChunkBase::pointInTerrain(int x, int y, int z)
{
	float dx = x - Planet->Origin.X;
	float dy = y - Planet->Origin.Y;
	float dz = z - Planet->Origin.Z;

	float distance = FMath::Sqrt(dx * dx + dy * dy + dz * dz);
	return distance <= Planet->Radius + Planet->Resolution * 0.9;
}

void APMarchingChunkBase::GenerateMesh()
{
	// Triangulation order
	if (SurfaceLevel > 0.0f)
	{
		TriangleOrder[0] = 0;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 2;
	}
	else
	{
		TriangleOrder[0] = 2;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 0;
	}

	float Cube[8];

	for (int X = 0; X < Planet->Size; ++X)
	{
		for (int Y = 0; Y < Planet->Size; ++Y)
		{
			for (int Z = 0; Z < Planet->Size; ++Z)
			{
				for (int i = 0; i < 8; ++i)
				{
					Cube[i] = Voxels[GetVoxelIndex(X + VertexOffset[i][0], Y + VertexOffset[i][1],
						Z + VertexOffset[i][2])];
				}

				March(X, Y, Z, Cube);
			}
		}
	}
	b_ChunkDataReady = true;
}

FLinearColor APMarchingChunkBase::setColorsToVertex(FVector v1) const
{
	float distance = FVector::Dist(this->GetActorLocation() + v1, Planet->Origin);
	if (distance < Planet->Radius - Planet->TerrainHeight + Planet->TerrainLevel_Water * (Planet->TerrainHeight))
	{
		return Planet->TerrainColor_Water;
	}
	if (distance < Planet->Radius - Planet->TerrainHeight + Planet->TerrainLevel_Low * (Planet->TerrainHeight))
	{
		return Planet->TerrainColor_Low;
	}
	if (distance < Planet->Radius - Planet->TerrainHeight + Planet->TerrainLevel_High * (Planet->TerrainHeight))
	{
		return Planet->TerrainColor_High;
	}
	if (distance < Planet->Radius - Planet->TerrainHeight + Planet->TerrainLevel_Top * (Planet->TerrainHeight))
	{
		return Planet->TerrainColor_Top;
	}
	return FColor::White;
}

void APMarchingChunkBase::March(const int X, const int Y, const int Z, const float Cube[8])
{
	int VertexMask = 0;
	FVector EdgeVertex[12];

	//Find which vertices are inside the surface and which are outside
	for (int i = 0; i < 8; ++i)
	{
		if (Cube[i] <= SurfaceLevel) VertexMask |= 1 << i;
	}

	const int EdgeMask = CubeEdgeFlags[VertexMask];

	if (EdgeMask == 0) return;

	// Find intersection points
	for (int i = 0; i < 12; ++i)
	{
		if ((EdgeMask & 1 << i) != 0)
		{
			const float Offset = Interpolation
				? GetInterpolationOffset(Cube[EdgeConnection[i][0]], Cube[EdgeConnection[i][1]])
				: 0.5f;

			EdgeVertex[i].X = X + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]);
			EdgeVertex[i].Y = Y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]);
			EdgeVertex[i].Z = Z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2]);
		}
	}

	// Save triangles, at most can be 5
	for (int i = 0; i < 5; ++i)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break;

		auto V1 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i]] * Planet->Resolution;
		auto V2 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 1]] * Planet->Resolution;
		auto V3 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 2]] * Planet->Resolution;

		auto Normal = FVector::CrossProduct(V2 - V1, V3 - V1);

		Normal.Normalize();

		MeshData.Vertices.Append({ V1, V2, V3 });

		MeshData.Triangles.Append({
			VertexCount + TriangleOrder[0],
			VertexCount + TriangleOrder[1],
			VertexCount + TriangleOrder[2]
			});

		MeshData.Normals.Append({
			Normal,
			Normal,
			Normal
			});

		MeshData.Colors.Append({
			setColorsToVertex(V1),
			setColorsToVertex(V2),
			setColorsToVertex(V3)
			});


		VertexCount += 3;
	}
}

int APMarchingChunkBase::GetVoxelIndex(const int X, const int Y, const int Z) const
{
	return Z * (Planet->Size + 1) * (Planet->Size + 1) + Y * (Planet->Size + 1) + X;
}

float APMarchingChunkBase::GetInterpolationOffset(const float V1, const float V2) const
{
	const float Delta = V2 - V1;
	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}
