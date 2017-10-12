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

	// loop through each of the 27 elements; the beginning key to search for will be equal to the centerKey minus 1 on all axis. 
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

void EnclaveCollectionStateArray::SetCenterCollectionDynamic(EnclaveKeyDef::EnclaveKey centerKey, EnclaveCollectionMatrix* collectionMatrixPtr)
{
	// set the location of the center of this matrix, for later use
	centerXYZsingle = translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset);	// converts to a single value

	// 0-initialize all elements of the dynamic array (should only need to be done once, but may change later (10/1/2017) )
	for (int x = 0; x < cubesize; x++)
	{
		for (int y = 0; y < cubesize; y++)
		{
			for (int z = 0; z < cubesize; z++)
			{
				EnclaveCollectionState tempState;
				tempState.isActive = 0;
				int currentIndex = translateXYZToSingle(x, y, z);
				StateMatrixPtr[currentIndex] = tempState;
				//StateMatrixPtr[x][y][z] = tempState;	// set them  all initially to 0 
			}
		}
	}

	// set the center collection
	EnclaveCollectionState centralCollectionMeta;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
	collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(centerKey);
	if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
	{
		EnclaveCollectionState* centerState = &StateMatrixPtr[centerXYZsingle];
		centerState->isActive = 1;										// indicate central is active
		centerState->collectionPtr = &collectionIterator->second;		// set collection pointer

	}

	// loop through all elements of the dynamic array; the beginning key to search for will be equal to the centerKey minus centerCollectionStateOffset on all axis. 
	EnclaveKeyDef::EnclaveKey beginSearchKey;
	beginSearchKey.x = centerKey.x - centerCollectionStateOffset;
	beginSearchKey.y = centerKey.y - centerCollectionStateOffset;
	beginSearchKey.z = centerKey.z - centerCollectionStateOffset;
	for (int xx = 0; xx < cubesize; xx++)
	{
		for (int yy = 0; yy < cubesize; yy++)
		{
			for (int zz = 0; zz < cubesize; zz++)
			{
				EnclaveKeyDef::EnclaveKey currentSearchKey = beginSearchKey;
				currentSearchKey.x += xx;
				currentSearchKey.y += yy;
				currentSearchKey.z += zz;
				collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(currentSearchKey);
				if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
				{
					EnclaveCollectionState* centerState = &StateMatrixPtr[translateXYZToSingle(xx, yy, zz)];
					centerState->ActualCollectionKey = currentSearchKey;
					centerState->isActive = 1;										// indicate central is active
					centerState->collectionPtr = &collectionIterator->second;		// set collection pointer
					//cout << "Neighboring collection found: (" << currentSearchKey.x << ", " << currentSearchKey.y << ", " << currentSearchKey.z << ")" << endl;
				}
				else
				{
					
					EnclaveCollectionState* centerState = &StateMatrixPtr[translateXYZToSingle(xx, yy, zz)];
					centerState->ActualCollectionKey = currentSearchKey;
					//cout << "array location = (" << xx << ", " << yy << ", " << zz << ") this key is: (" << centerState->ActualCollectionKey.x << ", " << centerState->ActualCollectionKey.y << ", " << centerState->ActualCollectionKey.z << ") " << endl;
					centerState->isActive = 0;
				}
			}
		}
	}
}


void EnclaveCollectionStateArray::CreateStateMatrix(int inCubesize)	// note: input value must be a value of 3,5,7,9, etc
{
	StateMatrixPtr = new EnclaveCollectionState[inCubesize*inCubesize*inCubesize];		// generate the dynamic array
	cubesize = inCubesize;																// set cubesize
	isArraySet = 1;																		// set isArraySet indicator flag
	centerCollectionStateOffset = inCubesize / 2;												// set the center location of this dynamic array

}

int EnclaveCollectionStateArray::translateXYZToSingle(int x, int y, int z)
{

	/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	//return x_axis + y_axis + z;

	int x_axis = z * (cubesize*cubesize);	// example, for a cube of length 8, this would be 64.
	int y_axis = y * (cubesize);			// ...this would be 8
											/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	return x_axis + y_axis + x;
}

void EnclaveCollectionStateArray::ShiftCenterCollection(EnclaveKeyDef::EnclaveKey previousKey, EnclaveKeyDef::EnclaveKey currentKey, EnclaveCollectionMatrix* collectionMatrixPtr)
{
	// check for x change
	if (previousKey.x != currentKey.x)
	{
		if (currentKey.x > previousKey.x)		// positive shift (east)
		{
			// begin shift logic, begin with lowest layer at y = 0
			for (int y = 0; y < cubesize; y++)
			{
				for (int z = 0; z < cubesize; z++)	// will start at northern most row of coordinates, going south
				{
					// for each z, iterate along x
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(0, y, z)];					// copy whatever values are at x = 0; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState lastStateInLine = StateMatrixPtr[translateXYZToSingle(cubesize - 1, y, z)];		// copy whatever is at cubesize - 1
					//EnclaveCollectionState* statePtr = &StateMatrixPtr[translateXYZToSingle(cubesize - 1, y, z)];
					//cout << "lastStateInLine pre: " << statePtr->ActualCollectionKey.x << ", " << statePtr->ActualCollectionKey.y << ", " << statePtr->ActualCollectionKey.z << endl;
					//cout << "lastStateInLine pre: " << lastStateInLine.ActualCollectionKey.x << ", " << lastStateInLine.ActualCollectionKey.y << ", " << lastStateInLine.ActualCollectionKey.z << endl;
					for (int x = 1; x < cubesize; x++)				// begin at x = 1; go until cubesize; for every iteration, replace x - 1 with the value at x 
					{
						StateMatrixPtr[translateXYZToSingle(x - 1, y, z)] = StateMatrixPtr[translateXYZToSingle(x, y, z)];
					}
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					EnclaveKeyDef::EnclaveKey tempFindKey; //= lastStateInLine.ActualCollectionKey.x + 1, lastStateInLine.ActualCollectionKey.y, lastStateInLine.ActualCollectionKey.z
					tempFindKey.x = lastStateInLine.ActualCollectionKey.x + 1;		// going towards positive x, by +1
					tempFindKey.y = lastStateInLine.ActualCollectionKey.y;
					tempFindKey.z = lastStateInLine.ActualCollectionKey.z;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(cubesize - 1, y, z)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					else
					{
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						//cout << "cube coord: (" << cubesize - 1 << ", " << y << ", " << z << ") | " << " tempFindKey is: " << tempFindKey.x << ", " << tempFindKey.y << ", " << tempFindKey.z << endl;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(cubesize - 1, y, z)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					
				}
			}
			EnclaveKeyDef::EnclaveKey outputKey = StateMatrixPtr[translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset)].ActualCollectionKey;
			cout << "New collection center (east) is: " << outputKey.x << ", " << outputKey.y << ", " << outputKey.z << endl;

		}
		else if (currentKey.x < previousKey.x)	// negative shift (west)
		{
			for (int y = 0; y < cubesize; y++)
			{
				for (int z = 0; z < cubesize; z++)	// will start at northern most row of coordinates, going south
				{
					// for each z, iterate along x
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(cubesize - 1, y, z)];	// copy whatever values are at x = cubesize - 1; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState firstStateInLine = StateMatrixPtr[translateXYZToSingle(0, y, z)];
					for (int x = cubesize - 1; x > 1; x--)
					{
						StateMatrixPtr[translateXYZToSingle(x, y, z)] = StateMatrixPtr[translateXYZToSingle(x - 1, y, z)];
					}
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					EnclaveKeyDef::EnclaveKey tempFindKey;
					tempFindKey.x = firstStateInLine.ActualCollectionKey.x - 1;		// going towards negative x, by -1
					tempFindKey.y = firstStateInLine.ActualCollectionKey.y;
					tempFindKey.z = firstStateInLine.ActualCollectionKey.z;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(0, y, z)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					else
					{
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(0, y, z)] = newState;

					}

				}
			}
			EnclaveKeyDef::EnclaveKey outputKey = StateMatrixPtr[translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset)].ActualCollectionKey;
			cout << "New collection center (west) is: " << outputKey.x << ", " << outputKey.y << ", " << outputKey.z << endl;
		}
	}

	// check for y change
	if (previousKey.y != currentKey.y)
	{
		if (currentKey.y > previousKey.y)		// positive shift (above)
		{
			// begin shift logic, begin with first layer at x = 0
			for (int x = 0; x < cubesize; x++)
			{
				for (int z = 0; z < cubesize; z++)
				{
					// for each z, iterate along y
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(x, 0, z)];					// copy whatever values are at x = 0; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState lastStateInLine = StateMatrixPtr[translateXYZToSingle(x, cubesize - 1, z)];		// copy whatever is at cubesize - 1
					for (int y = 1; y < cubesize; y++)
					{
						StateMatrixPtr[translateXYZToSingle(x, y - 1, z)] = StateMatrixPtr[translateXYZToSingle(x, y, z)];
					}
					//cout << "pre-iter" << endl;
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					//cout << "post-iter" << endl;
					EnclaveKeyDef::EnclaveKey tempFindKey; //= lastStateInLine.ActualCollectionKey.x + 1, lastStateInLine.ActualCollectionKey.y, lastStateInLine.ActualCollectionKey.z
					tempFindKey.x = lastStateInLine.ActualCollectionKey.x;		
					tempFindKey.y = lastStateInLine.ActualCollectionKey.y + 1;		// going towards positive y, by +1
					tempFindKey.z = lastStateInLine.ActualCollectionKey.z;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						cout << "found!" << endl;
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(x, cubesize - 1, z)] = newState;
					}
					else
					{
						cout << "not found!" << endl;
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(x, cubesize - 1, z)] = newState;
					}
				}
			}
			EnclaveKeyDef::EnclaveKey outputKey = StateMatrixPtr[translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset)].ActualCollectionKey;
			cout << "New collection center (above) is: " << outputKey.x << ", " << outputKey.y << ", " << outputKey.z << endl;
		}
		else if (currentKey.y < previousKey.y)	// negative shift (below)
		{
			for (int x = 0; x < cubesize; x++)
			{
				for (int z = 0; z < cubesize; z++)
				{
					// for each z, iterate along x
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(x, cubesize - 1, z)];	// copy whatever values are at x = cubesize - 1; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState firstStateInLine = StateMatrixPtr[translateXYZToSingle(x, 0, z)];
					for (int y = cubesize - 1; y > 1; y--)
					{
						StateMatrixPtr[translateXYZToSingle(x, y, z)] = StateMatrixPtr[translateXYZToSingle(x, y - 1, z)];
					}
					//cout << "pre-iter" << endl;
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					//cout << "post-iter" << endl;
					EnclaveKeyDef::EnclaveKey tempFindKey; //= lastStateInLine.ActualCollectionKey.x + 1, lastStateInLine.ActualCollectionKey.y, lastStateInLine.ActualCollectionKey.z
					tempFindKey.x = firstStateInLine.ActualCollectionKey.x;
					tempFindKey.y = firstStateInLine.ActualCollectionKey.y - 1;		// going towards positive y, by +1
					tempFindKey.z = firstStateInLine.ActualCollectionKey.z;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(x, 0, z)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					else
					{
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(x, 0, z)] = newState;

					}

				}
			}
		}
	}

	// check for z change
	if (previousKey.z != currentKey.z)
	{
		if (currentKey.z > previousKey.z)		// positive shift (south)
		{
			// begin shift logic, begin with lowest layer at y = 0
			for (int y = 0; y < cubesize; y++)
			{
				for (int x = 0; x < cubesize; x++)
				{
					// for each x, iterate along z
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(x, y, 0)];					// copy whatever values are at x = 0; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState lastStateInLine = StateMatrixPtr[translateXYZToSingle(x, y, cubesize - 1)];		// copy whatever is at cubesize - 1
					for (int z = 1; z < cubesize; z++)				// begin at x = 1; go until cubesize; for every iteration, replace x - 1 with the value at x 
					{
						StateMatrixPtr[translateXYZToSingle(x, y, z - 1)] = StateMatrixPtr[translateXYZToSingle(x, y, z)];
					}
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					EnclaveKeyDef::EnclaveKey tempFindKey; //= lastStateInLine.ActualCollectionKey.x + 1, lastStateInLine.ActualCollectionKey.y, lastStateInLine.ActualCollectionKey.z
					tempFindKey.x = lastStateInLine.ActualCollectionKey.x;		// going towards positive x, by +1
					tempFindKey.y = lastStateInLine.ActualCollectionKey.y;
					tempFindKey.z = lastStateInLine.ActualCollectionKey.z + 1;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(x, y, cubesize - 1)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					else
					{
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						//cout << "cube coord: (" << cubesize - 1 << ", " << y << ", " << z << ") | " << " tempFindKey is: " << tempFindKey.x << ", " << tempFindKey.y << ", " << tempFindKey.z << endl;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(x, y, cubesize - 1)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}

				}
			}
			EnclaveKeyDef::EnclaveKey outputKey = StateMatrixPtr[translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset)].ActualCollectionKey;
			cout << "New collection center (south) is: " << outputKey.x << ", " << outputKey.y << ", " << outputKey.z << endl;

		}
		else if (currentKey.z < previousKey.z)	// negative shift (north)
		{
			for (int y = 0; y < cubesize; y++)
			{
				for (int x = 0; x < cubesize; x++)
				{
					// for each z, iterate along x
					EnclaveCollectionState stateToReplace = StateMatrixPtr[translateXYZToSingle(x, y, cubesize - 1)];	// copy whatever values are at x = cubesize - 1; this will replace whatever x is at the end of this axis (for instance, with a cubesize of 7 this would be 6)
					EnclaveCollectionState firstStateInLine = StateMatrixPtr[translateXYZToSingle(x, y, 0)];
					for (int z = cubesize - 1; z > 1; z--)
					{
						StateMatrixPtr[translateXYZToSingle(x, y, z)] = StateMatrixPtr[translateXYZToSingle(x, y, z - 1)];
					}
					std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionIterator;
					EnclaveKeyDef::EnclaveKey tempFindKey;
					tempFindKey.x = firstStateInLine.ActualCollectionKey.x;		// going towards negative x, by -1
					tempFindKey.y = firstStateInLine.ActualCollectionKey.y;
					tempFindKey.z = firstStateInLine.ActualCollectionKey.z - 1;
					collectionIterator = collectionMatrixPtr->EnclaveCollectionMap.find(tempFindKey);
					if (collectionIterator != collectionMatrixPtr->EnclaveCollectionMap.end())
					{
						EnclaveCollectionState newState;			// the brand new state value, that will replace the end of the row (the index at cubesize - 1)
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 1;
						newState.collectionPtr = &collectionIterator->second;
						StateMatrixPtr[translateXYZToSingle(x, y, 0)] = newState;					// replace the end of the x row with stateToReplace; stateToReplace is x = 0, which is the part of the matrix that has been "recycled"
					}
					else
					{
						EnclaveCollectionState newState;
						newState.ActualCollectionKey = tempFindKey;
						newState.isActive = 0;
						StateMatrixPtr[translateXYZToSingle(x, y, 0)] = newState;

					}

				}
			}
			EnclaveKeyDef::EnclaveKey outputKey = StateMatrixPtr[translateXYZToSingle(centerCollectionStateOffset, centerCollectionStateOffset, centerCollectionStateOffset)].ActualCollectionKey;
			cout << "New collection center (north) is: " << outputKey.x << ", " << outputKey.y << ", " << outputKey.z << endl;

		}
	}
}

EnclaveCollectionStateArray::~EnclaveCollectionStateArray()
{
	if (isArraySet == 1)
	{
		delete[] StateMatrixPtr;
	}
}