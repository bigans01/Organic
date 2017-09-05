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

void EnclaveCollectionBlueprint::SetCustomPaintableChunkData(ElevationMapRef paintableChunk)
{
	/* Summary: loads data into the PaintableChunks array*/
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			CustomPaintableChunks[x][z] = paintableChunk[x][z];
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



ElevationMapRef& EnclaveCollectionBlueprint::GetSurfaceChunkData()
{
	return SurfaceChunks;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetSolidChunkData()
{
	return SolidChunks;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetCustomPaintableChunkData()
{
	return CustomPaintableChunks;
}

ElevationMapRef& EnclaveCollectionBlueprint::GetStandardPaintableChunkData()
{
	//std::cout << "before return call" << int(StandardPaintableChunks[0][1]) << std::endl;
	return StandardPaintableChunks;
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

void EnclaveCollectionBlueprint::CarveSlope()
{
	/* Summary: carve a single collection from west to east, sloping upward (this description is subject ot change as this function is improved */

	// set up variables
	ECBXAxisCarvePlan tempPlan;

	// add an entry to CollectionPainterMap
	ECBCollectionPainter tempCollectionPainter;
	tempCollectionPainter.blockID = 2; // dirt?
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			tempCollectionPainter.chunksToPaint[x][z] = 63;
		}
	}
	std::string indexToUse = std::to_string(CurrentCollectionPainterMapIndex);
	CollectionPainterMap[indexToUse] = tempCollectionPainter;


	// west chunk carvings... from x =0 to x=3
	for (int x = 0; x < 4; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SolidChunks[x][z] = 127;		// all chunks except top chunk will be solid
			CustomPaintableChunks[x][z] = 64;		// paint only the top chunk (for now)
		}
	}

	// east chunk carvings...from x =4 to x=7
	for (int x = 4; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SolidChunks[x][z] = 255;		// all chunks painted
			CustomPaintableChunks[x][z] = 128;	// paint only the top chunk (for now)
		}
	}

	// modify CarvePlan
	// west chunks
	int currentYpos = 0;
	for (int x = 0; x < 4; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			// 6, 7, 0
			tempPlan.CPArray[0 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 0);		// BlockKeyToSingle(0 + (x * 4), currentYpos, 0 + (z * 4)) + 1, BlockKeyToSingle(0 + (x * 4), currentYpos, 0 + (z * 4))
			tempPlan.CPArray[0 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 1);
			tempPlan.CPArray[0 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 2);
			tempPlan.CPArray[0 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 3);
														 									
			tempPlan.CPArray[1 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 0);
			tempPlan.CPArray[1 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 1);
			tempPlan.CPArray[1 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 2);
			tempPlan.CPArray[1 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 3);
														 									
			tempPlan.CPArray[2 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 0);
			tempPlan.CPArray[2 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 1);
			tempPlan.CPArray[2 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 2);
			tempPlan.CPArray[2 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 3);
														 									
			tempPlan.CPArray[3 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 0);
			tempPlan.CPArray[3 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 1);
			tempPlan.CPArray[3 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 2);
			tempPlan.CPArray[3 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 3);
		}																										
		currentYpos++;																							
	}																											
																												
	// east chunks		
	currentYpos = 0;
	for (int x = 4; x < 8; x++)																					
	{																											
		for (int z = 0; z < 8; z++)																				
		{										
			if (x == 6 && z == 0)
			{
				std::cout << "test value of block key to single: " << BlockKeyToSingle(0, currentYpos, 0) + 1 << std::endl;
			}
			tempPlan.CPArray[0 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 0);
			tempPlan.CPArray[0 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 1);
			tempPlan.CPArray[0 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 2);
			tempPlan.CPArray[0 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(0, currentYpos, 3);
																							
			tempPlan.CPArray[1 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 0);
			tempPlan.CPArray[1 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 1);
			tempPlan.CPArray[1 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 2);
			tempPlan.CPArray[1 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(1, currentYpos, 3);
																							
			tempPlan.CPArray[2 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 0);
			tempPlan.CPArray[2 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 1);
			tempPlan.CPArray[2 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 2);
			tempPlan.CPArray[2 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(2, currentYpos, 3);
																							
			tempPlan.CPArray[3 + (x * 4)][0 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 0);
			tempPlan.CPArray[3 + (x * 4)][1 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 1);
			tempPlan.CPArray[3 + (x * 4)][2 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 2);
			tempPlan.CPArray[3 + (x * 4)][3 + (z * 4)] = BlockKeyToSingle(3, currentYpos, 3);
		}
		currentYpos++;
	}
	XAxisCPVector.push_back(tempPlan);


	// set up standard chunk flags
	// West flags, not border chunks
	for (int z = 0; z < 8; z++)
	{
		StandardPaintableChunks[0][z] = 63;
		//std::cout << "current value: " << int(StandardPaintableChunks[0][z]) << std::endl;
	}

	// North flags, not border chunks
	for (int x = 0; x < 8; x++)
	{
		StandardPaintableChunks[x][0] = 63;
	}

	// East flags, not border chunks
	for (int z = 0; z < 8; z++)
	{
		StandardPaintableChunks[7][z] = 127;
	}

	// South flags, not border chunks
	for (int x = 0; x < 8; x++)
	{
		StandardPaintableChunks[x][7] = 63;
	}


}

int EnclaveCollectionBlueprint::BlockKeyToSingle(int in_x, int in_y, int in_z)
{
	int x = in_x * 16;
	int y = in_y * 4;
	int z = in_z;
	return x + y + z;
}

EnclaveUnveilMeta EnclaveCollectionBlueprint::SetupCarvePlan(EnclaveKeyDef::EnclaveKey tempKey)
{
	if (CarveMode == 1)		// 1 = slope mode
	{
		int CPXcoord = tempKey.x;
		int CPZcoord = tempKey.z;
		ECBXAxisCarvePlan* planRef = &XAxisCPVector.front();		// get the plan from the vector
		EnclaveUnveilMeta returnMeta;
		for (int x = 0; x < 4; x++)
		{
			for (int z = 0; z < 4; z++)
			{
				returnMeta.EnclaveBlockLocation[x][z] = planRef->CPArray[(x + (CPXcoord*4))][(z + (CPZcoord * 4))];
 			}
		}
		return returnMeta;
	}
}