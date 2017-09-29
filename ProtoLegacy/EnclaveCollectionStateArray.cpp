#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "EnclaveCollectionStateArray.h"

void EnclaveCollectionStateArray::SetCenterCollection(EnclaveKeyDef::EnclaveKey centerKey, EnclaveCollectionMatrix* collectionMatrixPtr)
{
	// 0-initialize all 27 elements in the array (should only need to be done once, but may change later (9/29/2017) )
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				EnclaveCollectionState tempState;
				tempState.isActive = 0;
				StateMatrix[x][y][z] = tempState;	// set them  all initially to 0 
			}
		}
	}

	// set the center collection
	EnclaveCollectionState centralCollectionMeta;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
	collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(centerKey);
	if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
	{
		EnclaveCollectionState* centerState = &StateMatrix[1][1][1];
		centerState->isActive = 1;										// indicate central is active
		centerState->collectionPtr = &collectionIterator->second;		// set collection pointer

	}

	// loop through each of the 27 elements, except the center one; the beginning key to search for will be equal to the centerKey minus 1 on all axis. 
	EnclaveKeyDef::EnclaveKey beginSearchKey;
	beginSearchKey.x = centerKey.x - cubesize;
	beginSearchKey.y = centerKey.y - cubesize;
	beginSearchKey.z = centerKey.z - cubesize;
	for (int xx = 0; xx < 3; xx++)
	{
		for (int yy = 0; yy < 3; yy++)
		{
			for (int zz = 0; zz < 3; zz++)
			{
				EnclaveKeyDef::EnclaveKey currentSearchKey = beginSearchKey;
				currentSearchKey.x += xx;
				currentSearchKey.y += yy;
				currentSearchKey.z += zz;
				collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(currentSearchKey);
				if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
				{
					EnclaveCollectionState* centerState = &StateMatrix[xx][yy][zz];
					centerState->isActive = 1;										// indicate central is active
					centerState->collectionPtr = &collectionIterator->second;		// set collection pointer
					//cout << "Neighboring collection found: (" << currentSearchKey.x << ", " << currentSearchKey.y << ", " << currentSearchKey.z << ")" << endl;
				}
			}
		}
	}
	
}