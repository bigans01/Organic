/*------------------------------------------------------------------------------------------

--OGLMBufferManager.cpp		(Last Update: 10/15/2017)

Description: OGLMBufferManager.cpp contains the definitions for the declarations found in OGLMBufferManager.h.

------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OGLMBufferManager.h"
#include "OrganicGLManager.h"
#include "OrganicSystem.h"


void OGLMBufferManager::SetCubesize(int inT2_cubesize)
{
	T2_cubesize = inT2_cubesize;
}



void OGLMBufferManager::PopulateOGLMRMCArrays(EnclaveKeyDef::EnclaveKey centerCollectionKey)
{
	cout << "OGLMRMC key is: " << centerCollectionKey.x << ", " << centerCollectionKey.y << ", " << centerCollectionKey.z << endl;
	int centerCollectionOffset = T2_cubesize / 2;				// if T2_cubesize is 13, this would be equal to 6; element location 6 on any x/y/z access would be the center
	currentCenterCollectionKey = centerCollectionKey;
	EnclaveKeyDef::EnclaveKey lowerNWCornerKey = centerCollectionKey;				// the key of the collection that would be in the lower most corner; it is initiually set to the input value
	lowerNWCornerKey.x -= centerCollectionOffset;			// subtract the key's x value by the offset
	lowerNWCornerKey.y -= centerCollectionOffset;			// subtract the key's y value by the offset
	lowerNWCornerKey.z -= centerCollectionOffset;			// subtract the key's z value by the offset
	for (int x = 0; x < T2_cubesize; x++)	// x axis
	{
		for (int y = 0; y < T2_cubesize; y++)
		{
			for (int z = 0; z < T2_cubesize; z++)
			{
				int currentBufferElement = T2_translateXYZToSingle(x, y, z);
				EnclaveKeyDef::EnclaveKey elementCollectionKey = lowerNWCornerKey;
				elementCollectionKey.x += x;
				elementCollectionKey.y += y;
				elementCollectionKey.z += z;
				OGLMRMC.T2_renderMetaContainerArray[currentBufferElement].ElementCollectionKey = elementCollectionKey;			// set the collection key
				OGLMRMC.T2_renderMetaContainerArray[currentBufferElement].ElementSingularXYZValue = currentBufferElement;		// set the singular value
			}
		}
	}
	EnclaveKeyDef::EnclaveKey testKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(centerCollectionOffset, centerCollectionOffset, centerCollectionOffset)].ElementCollectionKey;
	cout << "center key after work is: " << testKey.x << ", " << testKey.y << ", " << testKey.z << endl;
	EnclaveKeyDef::EnclaveKey testKey2 = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0,0,0)].ElementCollectionKey;
	cout << "test of lower corner key: " << testKey2.x << ", " << testKey2.y << ", " << testKey2.z << endl;
	cout << "test of SingularXYZ of 12, 0, 0: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(12, 0, 0)].ElementSingularXYZValue << endl;
}

OGLMBufferManager::~OGLMBufferManager()
{
	if (arraysSet == 1)
	{

	}
}

int OGLMBufferManager::T2_translateXYZToSingle(int x, int y, int z)
{
	//int x_axis = x * (T2_cubesize*T2_cubesize);	// example, for a cube of length 8, this would be 64.
	//int y_axis = y * (T2_cubesize);			// ...this would be 8
	/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	//return x_axis + y_axis + z;

	int x_axis = z * (T2_cubesize*T2_cubesize);	// example, for a cube of length 8, this would be 64.
	int y_axis = y * (T2_cubesize);			// ...this would be 8
											/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	return x_axis + y_axis + x;

}

void OGLMBufferManager::SetOGLMPointer(OrganicGLManager* in_OGLMptr)
{
	OGLMPtr = in_OGLMptr;
}



void OGLMBufferManager::setShiftedCollectionKeys(EnclaveKeyDef::EnclaveKey oldKey, EnclaveKeyDef::EnclaveKey newKey)
{
	oldCenterCollectionKey = oldKey;
	currentCenterCollectionKey = newKey;
	shiftFlag = 1;		// set the shift flag indicator
}
void OGLMBufferManager::determineMorphDirections()
{

}

void OGLMBufferManager::MorphTerrainBuffers()
{
	// check negative x and positive x, respectively
	auto carvestart = std::chrono::high_resolution_clock::now();

	// NEGATIVE X BUFFER MORPH (WEST)
	if (currentCenterCollectionKey.x < oldCenterCollectionKey.x)
	{
		MorphTerrainBufferWest();		
	}

	// POSITIVE X BUFFER MORPH (EAST)
	if (currentCenterCollectionKey.x > oldCenterCollectionKey.x)
	{
		MorphTerrainBufferEast();
	}

	// NEGATIVE Y BUFFER MORPH (BELOW)
	if (currentCenterCollectionKey.y < oldCenterCollectionKey.y)
	{
		MorphTerrainBufferBelow();
	}

	// POSITIVE Y BUFFER MORPH (ABOVE)
	if (currentCenterCollectionKey.y > oldCenterCollectionKey.y)
	{
		MorphTerrainBufferAbove();
	}

	// NEGATIVE Z BUFFER MORPH (NORTH)
	if (currentCenterCollectionKey.z < oldCenterCollectionKey.z)
	{
		MorphTerrainBufferNorth();
	}

	// POSITIVE Z BUFFER MORPH (SOUTH)
	if (currentCenterCollectionKey.z > oldCenterCollectionKey.z)
	{
		MorphTerrainBufferSouth();
	}


	auto carveend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> carveelapsed = carveend - carvestart;
	std::cout << "Elapsed time: (matrix shift + blueprint search (" << T2_cubesize*T2_cubesize << "): " << carveelapsed.count() << endl;
}

void OGLMBufferManager::MorphTerrainBufferWest()
{
	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving WEST) test old key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
	//cout << "(Moving WEST): SingularXYZValue before shift: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	//cout << "(Moving WEST): SingularXYZValue of last element in row: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementSingularXYZValue << endl;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	//cout << "less than x morph!" << endl;
	for (int y = 0; y < T2_cubesize; y++)
	{
		for (int z = 0; z < T2_cubesize; z++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
																																						//cout << "old key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize <<  endl;
			int currentLastSingularXYZValueInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementSingularXYZValue;	// get the last singular XYZ value in the row, to be shifted to the front as the last step of sorting in this row
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsT1Key == 1)	// check to see if there was already an EnclaveCollection stored 
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementCollectionKey;	// set the old key
				tempMorphMeta.containsPreviousKey = 1;		// set the flag indicating that there is an old key
				cout << "previous key was set!" << endl;
			}

			for (int x = T2_cubesize - 1; x > 0; x--)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;							// replace element at x + 1 with this collection key

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInRow;		// store the value of currentFirstKeyInRow
			currentFirstKeyInRow.x -= 1;																								// subtract 1 from the currentFirstKeyInRow
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementCollectionKey = currentFirstKeyInRow;				// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementSingularXYZValue = currentLastSingularXYZValueInRow;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsT1Key = currentContainsT1KeyValue;

			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentFirstKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsT1Key = 1;	// indicate that the element at the beginning of this row now contains a T1 key.
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl;
			}
			//cout << "new key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << endl;

		}
	}

	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving WEST) test old key value for -x POST shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
	//cout << "(Moving WEST): SingularXYZValue after shift: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	//cout << "(Moving WEST): SingularXYZValue after shift of last element in row: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementSingularXYZValue << endl;
}
void OGLMBufferManager::MorphTerrainBufferEast()
{
	//cout << "current key: " << currentCenterCollectionKey.x << ", " << currentCenterCollectionKey.y << ", " << currentCenterCollectionKey.z << endl;
	//cout << "old key: " << oldCenterCollectionKey.x << ", " << oldCenterCollectionKey.y << ", " << oldCenterCollectionKey.z << endl;

	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving EAST) test 1 old key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
	//cout << "(Moving EAST): SingularXYZValue before shift: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	//cout << "(Moving EAST): SingularXYZValue of last element in row: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementSingularXYZValue << endl;
	//currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	//cout << "test 2 old key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	for (int y = 0; y < T2_cubesize; y++)
	{
		for (int z = 0; z < T2_cubesize; z++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentLastKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementCollectionKey;
			int currentFirstSingularXYZValueInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementSingularXYZValue;
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsT1Key == 1)	// check to see if there was already an EnclaveCollection stored in the T2_renderMetaContainerArrayElement
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementCollectionKey;	// set the old key
				tempMorphMeta.containsPreviousKey = 1;		// set the flag indicating that there is an old key
				cout << "previous key was set!" << endl;
			}

			for (int x = 0; x < T2_cubesize - 1; x++)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ElementCollectionKey;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentLastKey = currentLastKeyInRow;
			currentLastKeyInRow.x += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementCollectionKey = currentLastKeyInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementSingularXYZValue = currentFirstSingularXYZValueInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsT1Key = currentContainsT1KeyValue;

			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentLastKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsT1Key = 1;	// indicate that the element at the beginning of this row now contains a T1 key.
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl;
			}
			//cout << "new key value for -x shift is: " << currentLastKeyInRow.x << ", " << currentLastKeyInRow.y << ", " << currentLastKeyInRow.z << endl;

		}
	}

	//currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	//EnclaveKeyDef::EnclaveKey 
	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;
	cout << "(Moving EAST) test 2 old key value for +x POST shift is:" << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
	//cout << "(Moving EAST): SingularXYZValue after shift: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	//cout << "(Moving EAST): SingularXYZValue after shift of last element in row: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementSingularXYZValue << endl;
}
void OGLMBufferManager::MorphTerrainBufferBelow()
{
	//cout << "current key: " << currentCenterCollectionKey.x << ", " << currentCenterCollectionKey.y << ", " << currentCenterCollectionKey.z << endl;
	//cout << "old key: " << oldCenterCollectionKey.x << ", " << oldCenterCollectionKey.y << ", " << oldCenterCollectionKey.z << endl;

	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving BELOW) test old key value for -y shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	//currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	//cout << "test 2 old key value for -x shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	for (int x = 0; x < T2_cubesize; x++)
	{
		for (int z = 0; z < T2_cubesize; z++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentFirstKeyInColumn = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementCollectionKey; // get the collection key for the first element in the column
			int currentLastSingularXYZValueInColumn = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementSingularXYZValue;	// get the last singular XYZ value in the row, to be shifted to the front as the last step of sorting in this column
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsT1Key == 1)
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementCollectionKey;
				tempMorphMeta.containsPreviousKey = 1;
				cout << "previous key was set!" << endl;
			}
			for (int y = T2_cubesize - 1; y > 0; y--)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y - 1, z)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y - 1, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y - 1, z)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInColumn;		// store the value of currentFirstKeyInRow
			currentFirstKeyInColumn.y -= 1;

			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementCollectionKey = currentFirstKeyInColumn;	// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementSingularXYZValue = currentLastSingularXYZValueInColumn;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsT1Key = currentContainsT1KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInColumn);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInColumn;
				tempMorphMeta.collectionKey = currentFirstKeyInColumn;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsT1Key = 1;
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl;
			}
		}

	}
	cout << "(Moving BELOW): SingularXYZValue after shift: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	cout << "(Moving BELOW): SingularXYZValue after shift of last element in column: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, T2_cubesize - 1, 0)].ElementSingularXYZValue << endl;

	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving BELOW) test NEW key value for -y shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
}
void OGLMBufferManager::MorphTerrainBufferAbove()
{
	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, T2_cubesize - 1, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving ABOVE) test old key value for -y shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	for (int x = 0; x < T2_cubesize; x++)
	{
		for (int z = 0; z < T2_cubesize; z++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentLastKeyInColumn = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementCollectionKey; // get the collection key for the first element in the column
			int currentFirstSingularXYZValueInColumn = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementSingularXYZValue;
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsT1Key == 1)
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementCollectionKey;
				tempMorphMeta.containsPreviousKey = 1;
				cout << "previous key was set!" << endl;
			}
			for (int y = 0; y < T2_cubesize - 1; y++)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y + 1, z)].ElementCollectionKey;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y + 1, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y + 1, z)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentLastKeyInColumn;		// store the value of currentFirstKeyInRow
			currentLastKeyInColumn.y += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementCollectionKey = currentLastKeyInColumn;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementSingularXYZValue = currentFirstSingularXYZValueInColumn;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsT1Key = currentContainsT1KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInColumn);
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInColumn;
				tempMorphMeta.collectionKey = currentLastKeyInColumn;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsT1Key = 1;
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl;
			}

		}
	}

	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, T2_cubesize - 1, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving ABOVE) test NEW key value for +y shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
}
void OGLMBufferManager::MorphTerrainBufferNorth()
{
	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving NORTH) test old key value for -z shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	for (int y = 0; y < T2_cubesize; y++)
	{
		for (int x = 0; x < T2_cubesize; x++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
			int currentLastSingularXYZValueInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementSingularXYZValue;	// get the last singular XYZ value in the row, to be shifted to the front as the last step of sorting in this row
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsT1Key == 1)
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementCollectionKey;
				tempMorphMeta.containsPreviousKey = 1;
				cout << "previous key was set!" << endl;
			}
			for (int z = T2_cubesize - 1; z > 0; z--)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z - 1)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z - 1)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z - 1)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInRow;		// store the value of currentFirstKeyInRow
			currentFirstKeyInRow.z -= 1;																					// subtract 1 from the currentFirstKeyInRow
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementCollectionKey = currentFirstKeyInRow;	// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementSingularXYZValue = currentLastSingularXYZValueInRow;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsT1Key = currentContainsT1KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentFirstKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsT1Key = 1;
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl;
			}
		}
	}

	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving NORTH) test NEW key value for -z POST shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
}
void OGLMBufferManager::MorphTerrainBufferSouth()
{
	EnclaveKeyDef::EnclaveKey currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
																																				//EnclaveKeyDef::EnclaveKey testKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(6, 6, 0)].ElementCollectionKey;
	cout << "(Moving SOUTH) test old key value for -z shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator2;
	//blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(testKey);		// attempt to find the blueprint
	//if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
	//{
	//cout << "key was found as blueprint!!!" << endl;
	//}
	for (int y = 0; y < T2_cubesize; y++)
	{
		for (int x = 0; x < T2_cubesize; x++)
		{
			OrganicMorphMeta tempMorphMeta;
			EnclaveKeyDef::EnclaveKey currentLastKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementCollectionKey;
			int currentFirstSingularXYZValueInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementSingularXYZValue;
			int currentContainsT1KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsT1Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsT1Key == 1)
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementCollectionKey;
				tempMorphMeta.containsPreviousKey = 1;
				cout << "previous key was set!" << endl;
			}
			for (int z = 0; z < T2_cubesize - 1; z++)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z + 1)].ElementCollectionKey;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z + 1)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int containsT1KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z + 1)].ContainsT1Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsT1Key = containsT1KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentLastKey = currentLastKeyInRow;
			currentLastKeyInRow.z += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementCollectionKey = currentLastKeyInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementSingularXYZValue = currentFirstSingularXYZValueInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsT1Key = currentContainsT1KeyValue;
			//cout << "key output: " << currentLastKeyInRow.x << ", " << currentLastKeyInRow.y << ", " << currentLastKeyInRow.z << ", " << endl;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentLastKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsT1Key = 1;
				cout << ">>>>>>>>>>>Blueprint was found!! adding to processing queue....." << tempMorphMeta.collectionKey.x << ", " << tempMorphMeta.collectionKey.y << ", " << tempMorphMeta.collectionKey.z << endl;
			}
			else
			{
				//cout << "blueprint not found in OrganicSystem!" << endl; 
			}
		}
	}

	currentFirstKeyInRow = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;		// get the enclave collection key of the first element in the row
	cout << "(Moving SOUTH) test NEW key value for +z POST shift is: " << currentFirstKeyInRow.x << ", " << currentFirstKeyInRow.y << ", " << currentFirstKeyInRow.z << " T2_cubesize: " << T2_cubesize << endl;
}


int OGLMBufferManager::determineRenderDataSubBufferKey(EnclaveKeyDef::EnclaveKey renderCollectionKey)
{
	/*Summary:
	  compares the input value to the currentCenterCollectionKey
	*/

	// set the initial offsets to be equal to the center
	int x_offset = (T2_cubesize / 2);
	int y_offset = (T2_cubesize / 2);
	int z_offset = (T2_cubesize / 2);
	// if the call is mode 1, (on world load) do this
	EnclaveKeyDef::EnclaveKey keyToCompareTo = currentCenterCollectionKey;
	cout << "test keyToCompareTo: (" << keyToCompareTo.x << ", " << keyToCompareTo.y << ", " << keyToCompareTo.z << ") || input collection key: (" << renderCollectionKey.x << ", " << renderCollectionKey.y << ", " << renderCollectionKey.z << ") " << endl;

	// determine x offset 
	if (renderCollectionKey.x != keyToCompareTo.x)
	{
		int offset_value = renderCollectionKey.x - keyToCompareTo.x;	// determine the difference in x, between the renderCollectionKey and the currently compared to key
		x_offset += offset_value;
	}

	// determine y offset
	if (renderCollectionKey.y != keyToCompareTo.y)
	{
		int offset_value = renderCollectionKey.y - keyToCompareTo.y;
		y_offset += offset_value;
	}

	// determine z offset
	if (renderCollectionKey.z != keyToCompareTo.z)
	{
		int offset_value = renderCollectionKey.z - keyToCompareTo.z;
		z_offset += offset_value;
	}
	int XYZtoSingleResult = T2_translateXYZToSingle(x_offset, y_offset, z_offset);
	cout << "XYZ to single: " << T2_translateXYZToSingle(x_offset, y_offset, z_offset) << endl;
	return XYZtoSingleResult;
}