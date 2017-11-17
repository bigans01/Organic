#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "EnclaveCollection.h"
//#include "Enclave.h"
#include "EnclaveCollectionBlueprint.h"

//class Enclave;
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
			AirtightChunks[x][z] = 63;
		}
	}

	// east chunk carvings...from x =4 to x=7
	for (int x = 4; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SolidChunks[x][z] = 255;		// all chunks painted
			CustomPaintableChunks[x][z] = 128;	// paint only the top chunk (for now)
			AirtightChunks[x][z] = 127;
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
				//std::cout << "test value of block key to single: " << BlockKeyToSingle(0, currentYpos, 0) + 1 << std::endl;
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
		if (x >= 4)								// fill enclaves at x >= 4, and y = 7
		{
			StandardPaintableChunks[x][0] = StandardPaintableChunks[x][0] | 64;
		}
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
		if (x >= 4)								// fill enclaves at x >= 4, and y = 7
		{
			StandardPaintableChunks[x][7] = StandardPaintableChunks[x][7] | 64;
		}
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
		int returnMetaCount = 0;
		for (int x = 0; x < 4; x++)
		{
			for (int z = 0; z < 4; z++)
			{
				returnMeta.EnclaveBlockLocation[x][z] = planRef->CPArray[(x + (CPXcoord*4))][(z + (CPZcoord * 4))];
				returnMetaCount++;
 			}
		}
		returnMeta.numberOfBlocks = returnMetaCount;
		return returnMeta;
	}
}

EnclaveUnveilMeta EnclaveCollectionBlueprint::ReturnBorderChunkFacesToRender(int x, int y, int z, EnclaveCollectionBlueprint* originBlueprint, EnclaveCollectionBlueprint*  comparedBlueprint, int directionOfNeighbor)
{
	EnclaveUnveilMeta returnMeta;	// an instance of EnclaveUnveilMeta that will be returned as a result of this function call
	if (directionOfNeighbor == 32)	// neighbor is to the west
	{
		int begin_y = y * 4;					// begin_y determines the offset inside a slice to begin with. For instance, the chunk with a UniqueKey of 0, 1, 0 begins at bit 5 (4 bits per chunk).
		int begin_z = z * 4;					// begin_z determines the current slice to use 
		int unveil_y_location = 0;				// for location on the y axis inside the unveil meta return value
		int unveil_z_location = 0;				// for location on the z axis inside the unveil meta return value
		int slice_1 = begin_z;
		for (int aa = slice_1; aa < (slice_1 + 4); aa++)	// loop for slice_1 increments
		{
			unveil_y_location = 0;
			for (int bb = begin_y; bb < (begin_y + 4); bb++)	// loop for all 4 bits on the y-axis
			{
				int bitmaskshifter = 1;			// reset the bitmask
				bitmaskshifter <<= bb;			// shifts it to the left, up to 31 times.
				if (
					((originBlueprint->WestBorderBlocks.faceflagarray[aa] & bitmaskshifter) == bitmaskshifter)		// check if the slice at aa (in this case, slices going along the z axis, going south (+z)) 
																													// has a flag set that matches the bitmaskshifter (which represents the y-bit, so bit 32 = 32nd block)
					&&
					((comparedBlueprint->EastBorderBlocks.faceflagarray[aa] & bitmaskshifter) != bitmaskshifter)	// ...now compare it to the east face of the neighboring blueprint at that same point. If it is not equal, than the block is not being rendered at the border, and we 
																													// may now reveal the east face of the block at this location.			
					)
				{
					returnMeta.EnclaveBlockLocation[unveil_y_location][unveil_z_location] = BlockKeyToSingle(0, unveil_y_location, unveil_z_location);	// convert the location at y/z in the array, to a corresponding  single value
					returnMeta.BlockFacesToRender[unveil_y_location][unveil_z_location] = 32;						// set west (32) block faces to render,  									
					returnMeta.UnveilFlag[unveil_y_location][unveil_z_location] = 1;								// set the block for flagging
				}
				unveil_y_location++;	// loop increment
			}
			unveil_z_location++;	// loop increment
		}
	}

	else if (directionOfNeighbor == 16) // neighbor is to the north
	{
		int begin_y = y * 4;					// begin_y determines the offset inside a slice to begin with. For instance, the chunk with a UniqueKey of 0, 1, 0 begins at bit 5 (4 bits per chunk). 
		int begin_x = x * 4;					// begin_x determines the current slice to use 
		int unveil_y_location = 0;				// for location on the y axis inside the unveil meta return value
		int unveil_x_location = 0;				// for location on the z axis inside the unveil meta return value
		int slice_1 = begin_x;
		for (int aa = slice_1; aa < (slice_1 + 4); aa++)	// loop for slice_1 increments
		{
			unveil_y_location = 0;
			for (int bb = begin_y; bb < (begin_y + 4); bb++)	// loop for all 4 bits on the y-axis
			{
				int bitmaskshifter = 1;			// reset the bitmask
				bitmaskshifter <<= bb;			// shifts it to the left, up to 31 times.
				if (
					((originBlueprint->NorthBorderBlocks.faceflagarray[aa] & bitmaskshifter) == bitmaskshifter)		// check if the slice at aa (in this case, slices going along the x axis, going east (+x)) 
																													// has a flag set that matches the bitmaskshifter (which represents the y-bit, so bit 32 = 32nd block)
					&&																								
					((comparedBlueprint->SouthBorderBlocks.faceflagarray[aa] & bitmaskshifter) != bitmaskshifter)	// ...now compare it to the south face of the neighboring blueprint at that same point. If it is not equal, than the block is not being rendered at the border, and we
																													// may now reveal the south face of the block at this location.			
					)
				{
					returnMeta.EnclaveBlockLocation[unveil_y_location][unveil_x_location] = BlockKeyToSingle(unveil_x_location, unveil_y_location, 0);	// convert the location at x/y in the array, to a corresponding  single value
					returnMeta.BlockFacesToRender[unveil_y_location][unveil_x_location] = 16;						// set north (16) block faces to render,  
					returnMeta.UnveilFlag[unveil_y_location][unveil_x_location] = 1;								// set the block for flagging
				}
				unveil_y_location++;	// loop increment
			}
			unveil_x_location++;	// loop increment
		}
	}

	else if (directionOfNeighbor == 8) // neighbor is to the east
	{
		int begin_y = y * 4;					// begin_y determines the offset inside a slice to begin with. For instance, the chunk with a UniqueKey of 0, 1, 0 begins at bit 5 (4 bits per chunk).
		int begin_z = z * 4;					// begin_z determines the current slice to use 
		int unveil_y_location = 0;				// for location on the y axis inside the unveil meta return value// for location on the y axis inside the unveil meta return value
		int unveil_z_location = 0;				// for location on the z axis inside the unveil meta return value
		int slice_1 = begin_z;
		for (int aa = slice_1; aa < (slice_1 + 4); aa++)	// loop for slice_1 increments
		{
			unveil_y_location = 0;
			for (int bb = begin_y; bb < (begin_y + 4); bb++)	// loop for all 4 bits on the y-axis
			{
				int bitmaskshifter = 1;			// reset the bitmask
				bitmaskshifter <<= bb;			// shifts it to the left, up to 31 times.
				if (
					((originBlueprint->EastBorderBlocks.faceflagarray[aa] & bitmaskshifter) == bitmaskshifter)		// check if the slice at aa (in this case, slices going along the z axis, going south (+z)) 
																													// has a flag set that matches the bitmaskshifter (which represents the y-bit, so bit 32 = 32nd block)
					&&
					((comparedBlueprint->WestBorderBlocks.faceflagarray[aa] & bitmaskshifter) != bitmaskshifter)	// ...now compare it to the west face of the neighboring blueprint at that same point. If it is not equal, than the block is not being rendered at the border, and we 
																													// may now reveal the east face of the block at this location.			
					)
				{
					returnMeta.EnclaveBlockLocation[unveil_y_location][unveil_z_location] = BlockKeyToSingle(3, unveil_y_location, unveil_z_location);	// convert the location at y/z in the array, to a corresponding  single value
					returnMeta.BlockFacesToRender[unveil_y_location][unveil_z_location] = 8;						// set east (8) block faces to render,  		
					returnMeta.UnveilFlag[unveil_y_location][unveil_z_location] = 1;								// set the block for flagging
				}
				unveil_y_location++;	// loop increment
			}
			unveil_z_location++;	// loop increment
		}	



	}

	else if (directionOfNeighbor == 4)	// neighbor is to the south
	{
		int begin_y = y * 4;					// begin_y determines the offset inside a slice to begin with. For instance, the chunk with a UniqueKey of 0, 1, 0 begins at bit 5 (4 bits per chunk).
		int begin_x = x * 4;					// begin_x determines the current slice to use 
		int unveil_y_location = 0;				// for location on the y axis inside the unveil meta return value
		int unveil_x_location = 0;				// for location on the z axis inside the unveil meta return value
		int slice_1 = begin_x;
		for (int aa = slice_1; aa < (slice_1 + 4); aa++)	// loop for slice_1 increments
		{
			unveil_y_location = 0;
			for (int bb = begin_y; bb < (begin_y + 4); bb++)	// loop for all 4 bits on the y-axis
			{
				int bitmaskshifter = 1;
				bitmaskshifter <<= bb;
				if (
					((originBlueprint->SouthBorderBlocks.faceflagarray[aa] & bitmaskshifter) == bitmaskshifter)		// check if the slice at aa (in this case, slices going along the x axis, going east (+x)) 
					&&																								// has a flag set that matches the bitmaskshifter (which represents the y-bit, so bit 32 = 32nd block)

					((comparedBlueprint->NorthBorderBlocks.faceflagarray[aa] & bitmaskshifter) != bitmaskshifter)	// ...now compare it to the south face of the neighboring blueprint at that same point. If it is not equal, than the block is not being rendered at the border, and we
																													// may now reveal the south face of the block at this location.			
					)																								
				{
					returnMeta.EnclaveBlockLocation[unveil_y_location][unveil_x_location] = BlockKeyToSingle(unveil_x_location, unveil_y_location, 3);	// convert the location at x/y in the array, to a corresponding  single value
					returnMeta.BlockFacesToRender[unveil_y_location][unveil_x_location] = 4;						// set north (16) block faces to render,  
					returnMeta.UnveilFlag[unveil_y_location][unveil_x_location] = 1;								// set the block for flagging
				}
				unveil_y_location++;	// loop increment
			}
			unveil_x_location++;	// loop increment
		}

	}

	else if (directionOfNeighbor == 2) // neighbor is above
	{

	}
						
	else                         // neighnor is below
	{

	}
	//std::cout << "total count: " << totalrenderedcount << std::endl;
	return returnMeta;
}

void EnclaveCollectionBlueprint::SetBorderBlockFlags(int direction, int slice, int slice_offset)
{
	
	int initial_offset = 1;

	// do modifications for west
	if (direction == 32)
	{
		initial_offset <<= slice_offset;
		WestBorderBlocks.faceflagarray[slice] = WestBorderBlocks.faceflagarray[slice] | initial_offset;
		//std::cout << "west blocks set: " << WestBorderBlocks.faceflagarray[slice];
	}

	// do modifications for north
	if (direction == 16)
	{
		initial_offset <<= slice_offset;
		NorthBorderBlocks.faceflagarray[slice] = NorthBorderBlocks.faceflagarray[slice] | initial_offset;
	}

	// do modifications for east
	if (direction == 8)
	{
		initial_offset <<= slice_offset;
		EastBorderBlocks.faceflagarray[slice] = EastBorderBlocks.faceflagarray[slice] | initial_offset;
	}

	// do modifications for south
	if (direction == 4)
	{
		initial_offset <<= slice_offset;
		SouthBorderBlocks.faceflagarray[slice] = SouthBorderBlocks.faceflagarray[slice] | initial_offset;
	}
}

void EnclaveCollectionBlueprint::DetermineBorderWall(int direction, int valuearray[32])
{
	if (direction == 32) // west
	{
		for (int x = 0; x < 32; x++)
		{
			WestBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}

	else if (direction == 16) // north
	{
		for (int x = 0; x < 32; x++)
		{
			NorthBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}

	else if (direction == 8) // east
	{
		for (int x = 0; x < 32; x++)
		{
			EastBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}

	else if (direction == 4) // south
	{
		for (int x = 0; x < 32; x++)
		{
			SouthBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}

	else if (direction == 2) // top
	{
		for (int x = 0; x < 32; x++)
		{
			TopBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}

	else if (direction == 1) // bottom
	{
		for (int x = 0; x < 32; x++)
		{
			BottomBorderBlocks.faceflagarray[x] = valuearray[x];
		}
	}
}

void EnclaveCollectionBlueprint::FlattenToElevation()
{
	ECBXAxisCarvePlan tempPlan;
	ECBCollectionPainter tempCollectionPainter;
	tempCollectionPainter.blockID = 2; // dirt?

	// all chunk carvings
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			SolidChunks[x][z] = 1;		// all chunks except top chunk will be solid
			CustomPaintableChunks[x][z] = 1;		// paint only the top chunk (for now)
			AirtightChunks[x][z] = 1;
		}
	}

	int currentYpos = 3;
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
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
	}
	XAxisCPVector.push_back(tempPlan);
}

void EnclaveCollectionBlueprint::CalibrateBlueprintBorders(EnclaveCollection* collectionPtr)
{
	//cout << "calibrate call!" << endl;
	//cout << "before any editing done: " << WestBorderBlocks.faceflagarray[0] << endl;
	
	int current_x_slice;
	int current_z_slice;
	int current_y_offset;

	// begin with west faces (32)	-------------------------------------------------------------------------------------------------------------------
	for (int z = 0; z < 8; z++)			// start with z 
	{
		for (int y = 0; y < 8; y++)		// iterate through y for each z
		{
			Enclave* enclavePtr = &collectionPtr->EnclaveArray[0][y][z];
			current_z_slice = 4;					// always reset to 4
			current_z_slice = current_z_slice * z;	// will be 4 * 0 (for slices for z = 0), 4 * 1 (for slices for z = 1), etc...

			current_y_offset = 4;
			current_y_offset = current_y_offset * y;
			int beginbit = 1;
			int bitshift = current_y_offset;
			//for (int zz = 0; zz < 4; zz++)
			//{
			//	for (int yy = 0; yy < 4; yy++)
			//	{

				//}
			//	current_z_slice++;
			//}
			int totalPolys = enclavePtr->TotalRenderable;
			int testcount = 0;
			if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
			{
				//cout << "total polys: " << totalPolys << endl;
			}
			for (int aa = 0; aa < totalPolys; aa++)
			{
				EnclaveKeyDef::EnclaveKey resultKey = enclavePtr->SingleToEnclaveKey(enclavePtr->Sorted.PolyArrayIndex[aa]);
				//if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
				//{
					if (resultKey.x == 0)	// x == 0 means all blocks on the western face of the chunk
					{
						beginbit = 1;
						bitshift = current_y_offset;					// reset bitshift value
						bitshift = current_y_offset + resultKey.y;		// append y to bitshift value
						beginbit <<= bitshift;							// shift beginbit to the left by bitshift times
						//cout << "begin bit is: " << beginbit << endl;
						WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] = WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] | beginbit;
						//cout << "poly render: " << resultKey.x << ", " << resultKey.y << ", " << resultKey.z << "(shifted by " << bitshift << " || " << current_z_slice + resultKey.z << " || " << WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] <<" | " << bitshift << ")" << endl;

					}
				//}
			}

		}
	}

	// north faces (16)		---------------------------------------------------------------------------------------------------------------------------
	
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Enclave* enclavePtr = &collectionPtr->EnclaveArray[x][y][0];
			current_x_slice = 4;					// always reset to 4
			current_x_slice = current_x_slice * x;	// will be 4 * 0 (for slices for z = 0), 4 * 1 (for slices for z = 1), etc...

			current_y_offset = 4;
			current_y_offset = current_y_offset * y;

			int beginbit = 1;
			int bitshift = current_y_offset;

			int totalPolys = enclavePtr->TotalRenderable;
			int testcount = 0;

			for (int aa = 0; aa < totalPolys; aa++)
			{
				EnclaveKeyDef::EnclaveKey resultKey = enclavePtr->SingleToEnclaveKey(enclavePtr->Sorted.PolyArrayIndex[aa]);
				//if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
				//{
					if (resultKey.z == 0)		// z == 0 means all blocks on the northern face of the chunk
					{
						beginbit = 1;
						bitshift = current_y_offset;					// reset bitshift value
						bitshift = current_y_offset + resultKey.y;		// append y to bitshift value
						beginbit <<= bitshift;							// shift beginbit to the left by bitshift times
																		//cout << "begin bit is: " << beginbit << endl;
						NorthBorderBlocks.faceflagarray[current_x_slice + resultKey.x] = NorthBorderBlocks.faceflagarray[current_x_slice + resultKey.x] | beginbit;
						//cout << "poly render: " << resultKey.x << ", " << resultKey.y << ", " << resultKey.z << "(shifted by " << bitshift << " || " << current_z_slice + resultKey.z << " || " << WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] <<" | " << bitshift << ")" << endl;

					}
				//}
			}
		}
	}

	//  east faces (8)		---------------------------------------------------------------------------------------------------------------------------
	for (int z = 0; z < 8; z++)			// start with z 
	{
		for (int y = 0; y < 8; y++)		// iterate through y for each z
		{
			Enclave* enclavePtr = &collectionPtr->EnclaveArray[7][y][z];
			current_z_slice = 4;					// always reset to 4
			current_z_slice = current_z_slice * z;	// will be 4 * 0 (for slices for z = 0), 4 * 1 (for slices for z = 1), etc...

			current_y_offset = 4;
			current_y_offset = current_y_offset * y;
			int beginbit = 1;
			int bitshift = current_y_offset;
			//for (int zz = 0; zz < 4; zz++)
			//{
			//	for (int yy = 0; yy < 4; yy++)
			//	{

			//}
			//	current_z_slice++;
			//}
			int totalPolys = enclavePtr->TotalRenderable;
			int testcount = 0;
			if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
			{
				//cout << "total polys: " << totalPolys << endl;
			}
			for (int aa = 0; aa < totalPolys; aa++)
			{
				EnclaveKeyDef::EnclaveKey resultKey = enclavePtr->SingleToEnclaveKey(enclavePtr->Sorted.PolyArrayIndex[aa]);
				//if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
				//{
				if (resultKey.x == 3)	// x == 3 means all blocks on the eastern face of the chunk
				{
					beginbit = 1;
					bitshift = current_y_offset;					// reset bitshift value
					bitshift = current_y_offset + resultKey.y;		// append y to bitshift value
					beginbit <<= bitshift;							// shift beginbit to the left by bitshift times
																	//cout << "begin bit is: " << beginbit << endl;
					EastBorderBlocks.faceflagarray[current_z_slice + resultKey.z] = EastBorderBlocks.faceflagarray[current_z_slice + resultKey.z] | beginbit;
					if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 6 && enclavePtr->UniqueKey.z == 0)
					{
						cout << "poly render: " << resultKey.x << ", " << resultKey.y << ", " << resultKey.z << "(shifted by " << bitshift << " || " << current_z_slice + resultKey.z << " || " << WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] <<" | " << bitshift << ")" << endl;
					}
				}
				//}
			}
		}

	}


	//	south faces (4)		---------------------------------------------------------------------------------------------------------------------------
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Enclave* enclavePtr = &collectionPtr->EnclaveArray[x][y][7];
			current_x_slice = 4;					// always reset to 4
			current_x_slice = current_x_slice * x;	// will be 4 * 0 (for slices for z = 0), 4 * 1 (for slices for z = 1), etc...

			current_y_offset = 4;
			current_y_offset = current_y_offset * y;

			int beginbit = 1;
			int bitshift = current_y_offset;

			int totalPolys = enclavePtr->TotalRenderable;
			int testcount = 0;

			for (int aa = 0; aa < totalPolys; aa++)
			{
				EnclaveKeyDef::EnclaveKey resultKey = enclavePtr->SingleToEnclaveKey(enclavePtr->Sorted.PolyArrayIndex[aa]);
				//if (enclavePtr->UniqueKey.x == 0 && enclavePtr->UniqueKey.y == 2 && enclavePtr->UniqueKey.z == 0)
				//{
				if (resultKey.z == 3)	// z == 0 means all blocks on the southern face of the chunk
				{
					beginbit = 1;
					bitshift = current_y_offset;					// reset bitshift value
					bitshift = current_y_offset + resultKey.y;		// append y to bitshift value
					beginbit <<= bitshift;							// shift beginbit to the left by bitshift times
																	//cout << "begin bit is: " << beginbit << endl;
					SouthBorderBlocks.faceflagarray[current_x_slice + resultKey.x] = SouthBorderBlocks.faceflagarray[current_x_slice + resultKey.x] | beginbit;
					//cout << "poly render: " << resultKey.x << ", " << resultKey.y << ", " << resultKey.z << "(shifted by " << bitshift << " || " << current_z_slice + resultKey.z << " || " << WestBorderBlocks.faceflagarray[current_z_slice + resultKey.z] <<" | " << bitshift << ")" << endl;

				}
				//}
			}
		}

	}

	

}