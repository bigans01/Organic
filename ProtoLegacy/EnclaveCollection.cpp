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
	for (int z = 0; z < 8; z++) // traverse along the z axis
	{
		int stdchunkbitmask = 1;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[0][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				// set 16 blocks to have their west face flagged
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 0, 0, 1, 0, 32, 0);	// z = 0, all 4 up
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