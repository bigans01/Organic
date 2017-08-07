#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "EnclaveCollectionBlueprint.h"

typedef unsigned char(&ElevationMapRef)[8][8];
void EnclaveCollectionBlueprint::SetSurfaceChunkData(ElevationMapRef surfaceChunk)
{
	/* Summary: loads data into the SurfaceChunks array*/
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SurfaceChunks[x][z] = surfaceChunk[x][z];
		}
	}
}
void EnclaveCollectionBlueprint::SetSolidChunkData(ElevationMapRef solidChunk)
{
	/* Summary: loads data into the SolidChunks array*/
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SolidChunks[x][z] = solidChunk[x][z];
		}
	}
}

void EnclaveCollectionBlueprint::SetPaintableChunkData(ElevationMapRef paintableChunk)
{
	/* Summary: loads data into the PaintableChunks array*/
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			PaintableChunks[x][z] = paintableChunk[x][z];
		}
	}
}

void EnclaveCollectionBlueprint::AddNewPaintList(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList)
{
	//auto bluestart = std::chrono::high_resolution_clock::now();
	PaintListMatrix.PainterListMatrix[InKey] = InPaintList;
	//auto blueend = std::chrono::high_resolution_clock::now();
	//auto bluestart = std::chrono::high_resolution_clock::now();
	//int returnval = BPKeyToSingle(InKey);
	//PaintListMatrix.PainterListMatrix2[returnval] = InPaintList;
	//auto blueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> blueelapsed = blueend - bluestart;
	//std::cout << "Elapsed time (Blueprint addition to int): " << blueelapsed.count() << std::endl;
}

void EnclaveCollectionBlueprint::AddNewPaintList2(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList)
{
	//auto bluestart = std::chrono::high_resolution_clock::now();
	//PaintListMatrix.PainterListMatrix[InKey] = InPaintList;
	//auto blueend = std::chrono::high_resolution_clock::now();
	//auto bluestart = std::chrono::high_resolution_clock::now();
	int returnval = BPKeyToSingle(InKey);
	PaintListMatrix.PainterListMatrix2[returnval] = InPaintList;
	//auto blueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> blueelapsed = blueend - bluestart;
	//std::cout << "Elapsed time (Blueprint addition to int): " << blueelapsed.count() << std::endl;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetSurfaceChunkData()
{
	return SurfaceChunks;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetSolidChunkData()
{
	return SolidChunks;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetPaintableChunkData()
{
	return PaintableChunks;
}

int EnclaveCollectionBlueprint::BPKeyToSingle(EnclaveKeyDef::EnclaveKey tempKey)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array

	int x = tempKey.x * 64;
	int y = tempKey.y * 16;
	int z = tempKey.z;
	return x + y + z;
}