// Fill out your copyright notice in the Description page of Project Settings.


#include "PMarchingChunkBase.h"


#include "FastNoiseLite.h"
#include "ProceduralMeshComponent.h"

void APMarchingChunkBase::Setup()
{
	// Initialize Voxels
	Voxels.SetNum((Size + 1) * (Size + 1) * (Size + 1));
}

void APMarchingChunkBase::Generate2DHeightMap(const FVector Position)
{
	for (int x = 0; x <= Size; x++)
	{
		for (int y = 0; y <= Size; y++)
		{
			const float Xpos = x + Position.X;
			const float ypos = y + Position.Y;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, ypos) + 1) * Size / 2), 0, Size);

			for (int z = 0; z < Height; z++)
			{
				Voxels[GetVoxelIndex(x, y, z)] = 1.0f;
			}

			for (int z = Height; z < Size; z++)
			{
				Voxels[GetVoxelIndex(x, y, z)] = -1.0f;
			}
		}
	}
}

void APMarchingChunkBase::Generate3DHeightMap(const FVector Position)
{
	for (int x = 0; x <= Size; ++x)
	{
		for (int y = 0; y <= Size; ++y)
		{
			for (int z = 0; z <= Size; ++z)
			{
				if (pointInSphere(this->GetActorLocation().X + x * Resolution, this->GetActorLocation().Y + y * Resolution,
				                  this->GetActorLocation().Z + z * Resolution))
				{
					Voxels[GetVoxelIndex(x, y, z)] = Voxels[GetVoxelIndex(x, y, z)] = 1;//Noise->GetNoise(x + Position.X, y + Position.Y, z + Position.Z);
				}
				else
				{
					Voxels[GetVoxelIndex(x, y, z)] = -1;
				}
			}
		}
	}
	bHMGenerated = true;
}

bool APMarchingChunkBase::pointInSphere(int x, int y, int z)
{
	float dx = x - this->Origin.X;
	float dy = y - this->Origin.Y;
	float dz = z - this->Origin.Z;

	float distance = FMath::Sqrt(dx * dx + dy * dy + dz * dz);
	return distance <= Radius + Resolution/2;
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

	for (int X = 0; X < Size; ++X)
	{
		for (int Y = 0; Y < Size; ++Y)
		{
			for (int Z = 0; Z < Size; ++Z)
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
	bMeshGenerated = true;
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

		auto V1 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i]] * Resolution;
		auto V2 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 1]] * Resolution;
		auto V3 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 2]] * Resolution;

		auto Normal = FVector::CrossProduct(V2 - V1, V3 - V1);
		auto Color1 = FColor::Green;
		auto Color2 = FColor::Green;
		auto Color3 = FColor::Green;

		Normal.Normalize();

		MeshData.Vertices.Append({V1, V2, V3});

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
			Color1,
			Color2,
			Color3
		});

		VertexCount += 3;
	}
}

int APMarchingChunkBase::GetVoxelIndex(const int X, const int Y, const int Z) const
{
	return Z * (Size + 1) * (Size + 1) + Y * (Size + 1) + X;
}

float APMarchingChunkBase::GetInterpolationOffset(const float V1, const float V2) const
{
	const float Delta = V2 - V1;
	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}
