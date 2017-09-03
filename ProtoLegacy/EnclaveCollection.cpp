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
		cout << "ENTRY TEST (enclaves == 0) " << endl;
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

EnclaveCollectionActivateListT2 EnclaveCollection::SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, mutex& HeapMutex)
{
	EnclaveCollectionActivateListT2 buglist;
	cout << "----WEST BORDER ENTRY----" << endl;
	for (int z = 0; z < 8; z++) // traverse along the z axis, exclude border chunks
	{
		//cout << "value of z: " << z << endl;
		int stdchunkbitmask = 1;
		unsigned char actualbitmaskvalue = 128;
		//cout << "current byte value: " << int(elevationMapCopy[0][z]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			if ((elevationMapCopy[0][z] & stdchunkbitmask) == stdchunkbitmask)
			{
				//cout << "chunkbitmask: " << stdchunkbitmask << endl;
				// set 16 blocks to have their West face flagged (32)
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

				//HeapMutex.lock();
				//cout << "Before edit: (West) " << int(activateListRef->flagArray[0][3]) << endl;
				if (z == 6)
				{
					//cout << "dumb test:" << int(activateListRef.flagArray[0][6]) << "; bitmask: " << stdchunkbitmask << endl;
				}
				//buglist.flagArray[0][z] = 128;
				//activateListRef.flagArray[0][z] = activateListRef.flagArray[0][z] = 128;		// perform bitwise logical append
				//buglist.flagArray[0][z] = 128;
				cout << "----------flag set entry" << endl;
				//buglist.flagArray[0][z] = buglist.flagArray[0][z] | stdchunkbitmask;
				activateListRef.flagArray[0][z] = activateListRef.flagArray[0][z] | stdchunkbitmask;
				//activateListRef->flagArray[0][z] = activateListRef->flagArray[0][z] + actualbitmaskvalue;		// perform bitwise logical append
				//activateListRef.flagArray[0][z] = actualbitmaskvalue;
				//cout << "Post edit: " << int(activateListRef->flagArray[0][3]) << endl;
				//activateListRef.flagArray[0][0] | stdchunkbitmask;
				//HeapMutex.unlock();
			}
			//cout << "chunkbitmask: " << stdchunkbitmask << endl;
			stdchunkbitmask <<= 1;
			//actualbitmaskvalue *= 2;
		}
	}
	return buglist;
}

EnclaveCollectionActivateListT2 EnclaveCollection::SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, mutex& HeapMutex)
{
	cout << "----NORTH BORDER ENTRY----" << endl;
	EnclaveCollectionActivateListT2 buglist;
	//HeapMutex.lock();
	int northcount = 0;
	for (int x = 0; x < 8; x++)
	{
		int stdchunkbitmask = 1;
		unsigned char actualbitmaskvalue = 1;
		//cout << "current byte value: " << int(elevationMapCopy[x][0]) << endl;
		for (int bitloop = 0; bitloop < 8; bitloop++)		// bitloop value serves as current y coordinate
		{
			

			if ((elevationMapCopy[x][0] & stdchunkbitmask) == stdchunkbitmask)
			{
				// cout << "valid enclave key: " << x << ", " << bitloop << ", " << 0 << endl;
				// set 16 blocks to have their North face flagged (16)
				//cout << "chunkbitmask: " << stdchunkbitmask << endl;
				EnclaveArray[x][bitloop][0].UnveilSinglePoly(0, 0, 0, 0, 1, 0, 16, 0);	// x = 0, all 4 up
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

				//HeapMutex.lock();
				//cout << "Before edit: (North)" << int(activateListRef.flagArray[0][6]) << endl;
				//cout << "value of x: " << x << endl;
				//cout << "value of bitloop: " << bitloop << endl;

				//activateListRef.flagArray[x][0] = activateListRef.flagArray[x][0] | stdchunkbitmask;		// perform bitwise logical append
				//activateListRef.flagArray[1][0] = activateListRef.flagArray[0][0] | stdchunkbitmask;		// perform bitwise logical append
				//activateListRef.flagArray[x][0] = activateListRef.flagArray[x][0] + actualbitmaskvalue;		// perform bitwise logical append
				unsigned char lolchar = 127;
				//activateListRef.flagArray[x][0] = lolchar;	
				//activateListRef.flagArray[x][0] = 128;		// perform bitwise logical append
				buglist.flagArray[x][0] = buglist.flagArray[x][0] | stdchunkbitmask;
				//buglist.flagArray[x][0] = buglist.flagArray[x][0] = 128;
				//buglist.flagArray[0][z] = buglist.flagArray[0][z] | stdchunkbitmask;
				//HeapMutex.unlock();
				northcount++;
			}
			//cout << "chunkbitmask: " << stdchunkbitmask << endl;

			stdchunkbitmask <<= 1;
			actualbitmaskvalue *= 2;
		}
	}
	for (int x = 0; x < 8; x++)
	{
		cout << "value of x: " << int(buglist.flagArray[x][0]) << endl;
	}
	cout << "final value of the north count: " << northcount << endl;
	//HeapMutex.unlock();
	return buglist;
}

void EnclaveCollection::SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef, mutex& HeapMutex)
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
				EnclaveArray[7][bitloop][z].UnveilSinglePoly(3, 0, 0, 0, 1, 0, 8, 0);	// z = 0, all 4 up
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

void EnclaveCollection::SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef, mutex& HeapMutex)
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
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 0, 3, 0, 1, 0, 4, 0);	// x = 0, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 1, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 2, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(0, 3, 3, 0, 1, 0, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 0, 3, 0, 1, 0, 4, 0);	// x = 1, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 1, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 2, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(1, 3, 3, 0, 1, 0, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 0, 3, 0, 1, 0, 4, 0);	// x = 2, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 1, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 2, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(2, 3, 3, 0, 1, 0, 4, 0);

				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 0, 3, 0, 1, 0, 4, 0);	// x = 3, all 4 up
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 1, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 2, 3, 0, 1, 0, 4, 0);
				EnclaveArray[x][bitloop][7].UnveilSinglePoly(3, 3, 3, 0, 1, 0, 4, 0);

				activateListRef.flagArray[x][7] = activateListRef.flagArray[x][7] | stdchunkbitmask;		// perform bitwise logical append
			}

			stdchunkbitmask <<= 1;
		}
	}
}

void EnclaveCollection::SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef, mutex& HeapMutex)
{

}

void EnclaveCollection::SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef, mutex& HeapMutex)
{

}

void EnclaveCollection::RunCollectionPainters(EnclaveCollectionBlueprint* blueprintRef)
{
	std::unordered_map<std::string, ECBCollectionPainter>::iterator PainterIter = blueprintRef->CollectionPainterMap.begin();
	int dumbcount = 0;
	for (PainterIter; PainterIter != blueprintRef->CollectionPainterMap.end(); ++PainterIter)
	{
		cout << "paint job found!" << endl;
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

	cout << "total chunks painted: " << dumbcount << endl;
	//char factoryprefix[] = "Factory ";
	//string factorystring(factoryprefix);
	//string factorynumber = to_string(x);
	//string factory = factorystring + factorynumber;
}