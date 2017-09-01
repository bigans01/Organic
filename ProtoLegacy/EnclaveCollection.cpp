#include "stdafx.h"
#include <stdio.h>
#include "EnclaveCollection.h"

typedef unsigned char(&ElevationMapRef)[8][8];
void EnclaveCollection::ActivateEnclaveForRendering(EnclaveKeyDef::EnclaveKey Key)
{
	if (totalRenderableEnclaves == 0)
	{
		RenderableEnclaves[totalRenderableEnclaves] = Key;
		totalRenderableEnclaves++;
	}
	else if (totalRenderableEnclaves > 0)
	{
		int duplicateFlag = 0;
		for (int x = 0; x < totalRenderableEnclaves; x++)
		{
			if (RenderableEnclaves[x] == Key)		// THIS IS WRONG
			{
				duplicateFlag = 1;
			}
		}
		if (duplicateFlag == 0)			// only perform the following operation if there were no duplicates found
		{
			//RenderableEnclaves[x + 1] = Key;		// if no duplicate is found, make the next index equal to key. 
			//totalRenderableEnclaves++;
			RenderableEnclaves[totalRenderableEnclaves] = Key;
			totalRenderableEnclaves++;
		}
	}
}

Enclave& EnclaveCollection::GetEnclaveByKey(EnclaveKeyDef::EnclaveKey InKey)
{
	return EnclaveArray[InKey.x][InKey.y][InKey.z];
}

void EnclaveCollection::SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{
	for (int z = 1; z < 7; z++) // traverse along the z axis, exclude border chunks
	{
		int stdchunkbitmask = 1;
		//cout << "current byte value: " << int(elevationMapCopy[0][z]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[0][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				// set 16 blocks to have their West face flagged (32)
				//EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 0, 0, 1, 0, 32, 0);	// z = 0, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 0, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 0, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 0, 0, 1, 0, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 1, 0, 1, 0, 32, 0);	// z = 1, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 1, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 1, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 1, 0, 1, 0, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 2, 0, 1, 0, 32, 0);	// z = 2, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 2, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 2, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 2, 0, 1, 0, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 3, 0, 1, 0, 32, 0);	// z = 3, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 3, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 3, 0, 1, 0, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 3, 0, 1, 0, 32, 0);

				activateListRef.flagArray[0][z] = activateListRef.flagArray[0][z] | stdchunkbitmask;		// perform bitwise logical append
			}
			
			stdchunkbitmask <<= 1;
		}
	}
}

void EnclaveCollection::SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{
	for (int x = 1; x < 7; x++)
	{
		int stdchunkbitmask = 1;
		//cout << "current byte value: " << int(elevationMapCopy[x][0]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[x][0] & stdchunkbitmask) == stdchunkbitmask)
			{
				// cout << "valid enclave key: " << x << ", " << bitloop << ", " << 0 << endl;
				// set 16 blocks to have their North face flagged (16)
				//EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 0, 0, 1, 0, 16, 0);	// x = 0, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 1, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 2, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 3, 0, 0, 1, 0, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 0, 0, 0, 1, 0, 16, 0);	// x = 1, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 1, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 2, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 3, 0, 0, 1, 0, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 0, 0, 0, 1, 0, 16, 0);	// x = 2, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 1, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 2, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 3, 0, 0, 1, 0, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 0, 0, 0, 1, 0, 16, 0);	// x = 3, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 1, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 2, 0, 0, 1, 0, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 3, 0, 0, 1, 0, 16, 0);

				activateListRef.flagArray[x][0] = activateListRef.flagArray[x][0] | stdchunkbitmask;		// perform bitwise logical append
			}

			stdchunkbitmask <<= 1;
		}
	}
}

void EnclaveCollection::SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{
	for (int z = 1; z < 7; z++) // traverse along the z axis, exclude border chunks
	{
		int stdchunkbitmask = 1;
		//cout << "current byte value: " << int(elevationMapCopy[0][z]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[7][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				// set 16 blocks to have their East face flagged (8)
				//EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 0, 0, 1, 0, 8, 0);	// z = 0, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 0, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 0, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 0, 0, 1, 0, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 1, 0, 1, 0, 8, 0);	// z = 1, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 1, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 1, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 1, 0, 1, 0, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 2, 0, 1, 0, 8, 0);	// z = 2, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 2, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 2, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 2, 0, 1, 0, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 3, 0, 1, 0, 8, 0);	// z = 3, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 3, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 3, 0, 1, 0, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 3, 0, 1, 0, 8, 0);

				activateListRef.flagArray[7][z] = activateListRef.flagArray[7][z] | stdchunkbitmask;		// perform bitwise logical append
			}

			stdchunkbitmask <<= 1;
		}
	}

}

void EnclaveCollection::SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{

}

void EnclaveCollection::SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{

}

void EnclaveCollection::SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{

}