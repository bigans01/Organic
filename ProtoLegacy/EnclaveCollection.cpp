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
		//totalRenderableEnclaves = 1;
		// cout << "ENTRY TEST (enclaves == 0) " << endl;
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

void EnclaveCollection::SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef)
{
	EnclaveCollectionActivateListT2 buglist;
	//cout << "----WEST BORDER ENTRY----" << endl;
	for (int z = 0; z < 8; z++) // traverse along the z axis, exclude border chunks
	{
		int stdchunkbitmask = 1;
		unsigned char actualbitmaskvalue = 128;

		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[0][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				//UnveilSinglePolyWithMtrl(int x, int y, int z, int in_readorder, int in_otherflags, short in_blockid, char in_t1, char in_t2)		 
				//UnveilSinglePoly(int x, int y, int z, int in_readorder, int in_otherflags, char in_t1, char in_t2)
				// set 16 blocks to have their West face flagged (32)
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 0, 0, 1, 32, 0);	// z = 0, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 0, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 0, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 0, 0, 1, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 1, 0, 1, 32, 0);	// z = 1, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 1, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 1, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 1, 0, 1, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 2, 0, 1, 32, 0);	// z = 2, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 2, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 2, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 2, 0, 1, 32, 0);
				
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 0, 3, 0, 1, 32, 0);	// z = 3, all 4 up
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 1, 3, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 2, 3, 0, 1, 32, 0);
				EnclaveArray[0][bitloop][z].UnveilSinglePoly(0, 3, 3, 0, 1, 32, 0);

				activateListRef.flagArray[0][z] = activateListRef.flagArray[0][z] | stdchunkbitmask;

			}

			stdchunkbitmask <<= 1;

		}
	}

}

void EnclaveCollection::SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef)
{
	//cout << "----NORTH BORDER ENTRY----" << endl;
	for (int x = 0; x < 8; x++)
	{
		int stdchunkbitmask = 1;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			

			if ((elevationMapCopy[x][0] & stdchunkbitmask) == stdchunkbitmask)
			{
				// cout << "valid enclave key: " << x << ", " << bitloop << ", " << 0 << endl;
				// set 16 blocks to have their North face flagged (16)
				//cout << "chunkbitmask: " << stdchunkbitmask << endl;
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 0, 0, 0, 1, 16, 0);	// x = 0, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 1, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 2, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 3, 0, 0, 1, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 0, 0, 0, 1, 16, 0);	// x = 1, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 1, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 2, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(1, 3, 0, 0, 1, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 0, 0, 0, 1, 16, 0);	// x = 2, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 1, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 2, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(2, 3, 0, 0, 1, 16, 0);
				
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 0, 0, 0, 1, 16, 0);	// x = 3, all 4 up
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 1, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 2, 0, 0, 1, 16, 0);
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(3, 3, 0, 0, 1, 16, 0);


				activateListRef.flagArray[x][0] = activateListRef.flagArray[x][0] | stdchunkbitmask;

			}
			//cout << "chunkbitmask: " << stdchunkbitmask << endl;

			stdchunkbitmask <<= 1;

		}
	}

}

void EnclaveCollection::SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{
	
	for (int z = 0; z < 8; z++) // traverse along the z axis, exclude border chunks
	{
		int stdchunkbitmask = 1;
		//cout << "current byte value: " << int(elevationMapCopy[0][z]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[7][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				// set 16 blocks to have their East face flagged (8)
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 0, 0, 1, 8, 0);	// z = 0, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 0, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 0, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 0, 0, 1, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 1, 0, 1, 8, 0);	// z = 1, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 1, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 1, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 1, 0, 1, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 2, 0, 1, 8, 0);	// z = 2, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 2, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 2, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 2, 0, 1, 8, 0);

				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 3, 0, 1, 8, 0);	// z = 3, all 4 up
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 1, 3, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 2, 3, 0, 1, 8, 0);
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 3, 3, 0, 1, 8, 0);

				activateListRef.flagArray[7][z] = activateListRef.flagArray[7][z] | stdchunkbitmask;		// perform bitwise logical append
			}

			stdchunkbitmask <<= 1;
		}
	}

}

void EnclaveCollection::SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{
	for (int x = 0; x < 8; x++)
	{
		int stdchunkbitmask = 1;
		//cout << "current byte value: " << int(elevationMapCopy[x][0]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[x][7] & stdchunkbitmask) == stdchunkbitmask)
			{
				// cout << "valid enclave key: " << x << ", " << bitloop << ", " << 0 << endl;
				// set 16 blocks to have their North face flagged (16)
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 0, 3, 0, 1, 4, 0);	// x = 0, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 1, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 2, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 3, 3, 0, 1, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 0, 3, 0, 1, 4, 0);	// x = 1, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 1, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 2, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 3, 3, 0, 1, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 0, 3, 0, 1, 4, 0);	// x = 2, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 1, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 2, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 3, 3, 0, 1, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 0, 3, 0, 1, 4, 0);	// x = 3, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 1, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 2, 3, 0, 1, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 3, 3, 0, 1, 4, 0);

				activateListRef.flagArray[x][7] = activateListRef.flagArray[x][7] | stdchunkbitmask;		// perform bitwise logical append
			}

			stdchunkbitmask <<= 1;
		}
	}
}

void EnclaveCollection::SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{

}

void EnclaveCollection::SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef)
{

}

void EnclaveCollection::RunCollectionPainters(EnclaveCollectionBlueprint* blueprintRef)
{
	std::unordered_map<std::string, ECBCollectionPainter>::iterator PainterIter = blueprintRef->CollectionPainterMap.begin();
	int dumbcount = 0;
	for (PainterIter; PainterIter != blueprintRef->CollectionPainterMap.end(); ++PainterIter)
	{
		//cout << "paint job found!" << endl;
		int chunkbitmask = 1;
		int blockToPaint = PainterIter->second.blockID;
		ElevationMapRef mapRef = PainterIter->second.chunksToPaint;

		for (int x = 0; x < 8; x++)
		{
			chunkbitmask = 1;
			for (int y = 0; y < 8; y++)
			{
				for (int z = 0; z < 8; z++)
				{
					if ((mapRef[x][z] & chunkbitmask) == chunkbitmask)
					{
						//cout << x << " " << z << " " << chunkbitmask  << " HIT 1" << endl;
						//Enclave stackEnclave(Key, x, y, z);
						//collectionRef.EnclaveArray[x][y][z] = stackEnclave;
						//collectionRef.EnclaveArray[x][y][z].InitializeRenderArray(1);				// setup this solid enclave
																									//cout << "Enclave set up complete: " << x << ", " << y << ", " << z << endl;
						Enclave* enclavePtr = &EnclaveArray[x][y][z];
						for (int xx = 0; xx < 4; xx++)
						{
							for (int yy = 0; yy < 4; yy++)
							{
								for (int zz = 0; zz < 4; zz++)
								{
									enclavePtr->StorageArray[xx][yy][zz].blockid = blockToPaint;
								}
							}
						}
						dumbcount++;
					}
				}
				chunkbitmask <<= 1;
			}
		}
	}

	//cout << "total chunks painted: " << dumbcount << endl;
	//char factoryprefix[] = "Factory ";
	//string factorystring(factoryprefix);
	//string factorynumber = to_string(x);
	//string factory = factorystring + factorynumber;
}