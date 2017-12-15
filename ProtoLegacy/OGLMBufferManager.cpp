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


void OGLMBufferManager::T2_SetCubesize(int inT2_cubesize)
{
	T2_cubesize = inT2_cubesize;
}

void OGLMBufferManager::T1_SetCubeSize(int inT1_cubesize)
{
	T1_cubesize = inT1_cubesize;
}



void OGLMBufferManager::PopulateOGLMRMCArrays(EnclaveKeyDef::EnclaveKey centerCollectionKey)
{
	cout << "OGLMRMC key is: " << centerCollectionKey.x << ", " << centerCollectionKey.y << ", " << centerCollectionKey.z << endl;



	// T2 dynamic array setup -- T2 array must be set up first, because it is a superset of the T1 array.
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
				OGLMRMC.T2_renderMetaContainerArray[currentBufferElement].ContainsUsedT2Key = 0;								// set the used key flag
			}
		}
	}
	EnclaveKeyDef::EnclaveKey testKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(centerCollectionOffset, centerCollectionOffset, centerCollectionOffset)].ElementCollectionKey;
	cout << "center key after work is: " << testKey.x << ", " << testKey.y << ", " << testKey.z << endl;
	EnclaveKeyDef::EnclaveKey testKey2 = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;
	cout << "test of lower corner key: " << testKey2.x << ", " << testKey2.y << ", " << testKey2.z << endl;
	cout << "test of SingularXYZ of 12, 0, 0: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(12, 0, 0)].ElementSingularXYZValue << endl;





	// T1 dynamic array setup -- contents must be populated as a subset of the T1 superset (populated above)
	centerCollectionOffset = T1_cubesize / 2;			// if T1_cubesize is 13, this would be equal to 6; element location 6 on any x/y/z access would be the center
	currentCenterCollectionKey = centerCollectionKey;
	lowerNWCornerKey = centerCollectionKey;				// reset the center collection key
	lowerNWCornerKey.x -= centerCollectionOffset;			// subtract the key's x value by the offset
	lowerNWCornerKey.y -= centerCollectionOffset;			// subtract the key's y value by the offset
	lowerNWCornerKey.z -= centerCollectionOffset;			// subtract the key's z value by the offset

	cout << "Begin new T1 population test..." << endl;
	// for the next 3 values, we must get the values of the X, Y, Z coordinate of the lower NW Element in the T1 buffer. 
	int LowerNWLocation = (T2_cubesize / 2) - (T1_cubesize / 2); // I.E: If T2 size is 13, and T1 size is 3; it means T1's distance from center to any perpendicular edge is 1, and T2's distance is 6. Subtracting 1 from 6 will give us the location of the lower NW edge.
	cout << "LowerNWLocation is: " << LowerNWLocation << endl;
	int T2_lowerNWBufferXValue = LowerNWLocation;
	int T2_lowerNWBufferYValue = LowerNWLocation;
	int T2_lowerNWBufferZValue = LowerNWLocation;
	for (int x = 0; x < T1_cubesize; x++)
	{
		for (int y = 0; y < T1_cubesize; y++)
		{
			for (int z = 0; z < T1_cubesize; z++)
			{
				int currentBufferElement = T1_translateXYZToSingle(x, y, z);
				int T2ElementCopy = T2_translateXYZToSingle(T2_lowerNWBufferXValue + x, T2_lowerNWBufferYValue + y, T2_lowerNWBufferZValue + z);
				EnclaveKeyDef::EnclaveKey elementCollectionKey = lowerNWCornerKey;
				elementCollectionKey.x += x;
				elementCollectionKey.y += y;
				elementCollectionKey.z += z;
				OGLMRMC.T1_renderMetaContainerArray[currentBufferElement].ElementCollectionKey = elementCollectionKey;		// set the collection key
				OGLMRMC.T1_renderMetaContainerArray[currentBufferElement].ElementSingularXYZValue = T2ElementCopy;	        // set the singular element value, based on the value from the T2 dynamic array
				OGLMRMC.T1_renderMetaContainerArray[currentBufferElement].ContainsUsedT1Key = 0;							// set the used key flag
				//cout << "T1 element key value: " << elementCollectionKey.x << ", " << elementCollectionKey.y << ", " << elementCollectionKey.z << endl;
			}
		}
	}

	//cout << "Test: T1 lower NW value is: " << OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(0, 0, 0)].ElementSingularXYZValue << endl;
	//cout << "Test: T2 -1 from center value is: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_lowerNWBufferXValue, T2_lowerNWBufferYValue, T2_lowerNWBufferZValue)].ElementSingularXYZValue << endl;

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

int OGLMBufferManager::T1_translateXYZToSingle(int x, int y, int z)
{
	int x_axis = z * (T1_cubesize*T1_cubesize);
	int y_axis = y * (T1_cubesize);

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
	//cout << "MORPHING CHECK: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsUsedT2Key;
	// check negative x and positive x, respectively
	auto carvestart = std::chrono::high_resolution_clock::now();

	// NEGATIVE X BUFFER MORPH (WEST)
	if (currentCenterCollectionKey.x < oldCenterCollectionKey.x)
	{
		MorphT2TerrainBufferWest();	
		MorphT1TerrainBufferWest();
	}

	// POSITIVE X BUFFER MORPH (EAST)
	if (currentCenterCollectionKey.x > oldCenterCollectionKey.x)
	{
		MorphT2TerrainBufferEast();
		MorphT1TerrainBufferEast();
	}

	// NEGATIVE Y BUFFER MORPH (BELOW)
	if (currentCenterCollectionKey.y < oldCenterCollectionKey.y)
	{
		MorphT2TerrainBufferBelow();
	}

	// POSITIVE Y BUFFER MORPH (ABOVE)
	if (currentCenterCollectionKey.y > oldCenterCollectionKey.y)
	{
		MorphT2TerrainBufferAbove();
	}

	// NEGATIVE Z BUFFER MORPH (NORTH)
	if (currentCenterCollectionKey.z < oldCenterCollectionKey.z)
	{
		MorphT2TerrainBufferNorth();
	}

	// POSITIVE Z BUFFER MORPH (SOUTH)
	if (currentCenterCollectionKey.z > oldCenterCollectionKey.z)
	{
		MorphT2TerrainBufferSouth();
	}


	auto carveend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> carveelapsed = carveend - carvestart;
	std::cout << "Elapsed time: (matrix shift + blueprint search (" << T2_cubesize*T2_cubesize << "): " << carveelapsed.count() << endl;
}

void OGLMBufferManager::MorphT2TerrainBufferWest()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsUsedT2Key == 1)	// check to see if there was already an EnclaveCollection stored 
			{
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementCollectionKey;	// set the old key
				tempMorphMeta.containsPreviousKey = 1;		// set the flag indicating that there is an old key
				cout << "previous key was set! (MOVED WEST) attempting removal send..." << endl;
				OrganicMorphMeta removalMeta;
				removalMeta.collectionKey = tempMorphMeta.oldCollectionKey;
				organicSystemPtr->T2CollectionRemovalQueue.push(removalMeta);		// send to be removed
			}

			for (int x = T2_cubesize - 1; x > 0; x--)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;							// replace element at x + 1 with this collection key

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x - 1, y, z)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInRow;		// store the value of currentFirstKeyInRow
			currentFirstKeyInRow.x -= 1;																								// subtract 1 from the currentFirstKeyInRow
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementCollectionKey = currentFirstKeyInRow;				// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementSingularXYZValue = currentLastSingularXYZValueInRow;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;

			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentFirstKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsUsedT2Key = 1;	// indicate that the element at the beginning of this row now contains a T1 key.
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
void OGLMBufferManager::MorphT2TerrainBufferEast()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ContainsUsedT2Key == 1)	// check to see if there was already an EnclaveCollection stored in the T2_renderMetaContainerArrayElement
			{
				//cout << "y value is " << y << endl;
				//cout << "Z value is " << z << endl;
				//cout << "ELEMENT ID IS: " << OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementSingularXYZValue << endl;
				//cout << "SINGULAR XYZ IS: " << currentFirstSingularXYZValueInRow << endl;
				tempMorphMeta.oldCollectionKey = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(0, y, z)].ElementCollectionKey;	// set the old key
				tempMorphMeta.containsPreviousKey = 1;		// set the flag indicating that there is an old key
				//cout << "original key 9: " << currentLastKeyInRow.x << ", " << currentLastKeyInRow.y << ", " << currentLastKeyInRow.z << endl;
				//cout << "previous key was set! (MOVED EAST)" << tempMorphMeta.oldCollectionKey.x << ", " << tempMorphMeta.oldCollectionKey.y << ", " << tempMorphMeta.oldCollectionKey.z << endl;
				OrganicMorphMeta removalMeta;
				removalMeta.collectionKey = tempMorphMeta.oldCollectionKey;
				organicSystemPtr->T2CollectionRemovalQueue.push(removalMeta);		// send to be removed
			}

			for (int x = 0; x < T2_cubesize - 1; x++)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ElementCollectionKey;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x + 1, y, z)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentLastKey = currentLastKeyInRow;
			currentLastKeyInRow.x += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementCollectionKey = currentLastKeyInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ElementSingularXYZValue = currentFirstSingularXYZValueInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;

			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentLastKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_cubesize - 1, y, z)].ContainsUsedT2Key = 1;	// indicate that the element at the beginning of this row now contains a T1 key.
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
void OGLMBufferManager::MorphT2TerrainBufferBelow()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsUsedT2Key == 1)
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

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y - 1, z)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInColumn;		// store the value of currentFirstKeyInRow
			currentFirstKeyInColumn.y -= 1;

			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementCollectionKey = currentFirstKeyInColumn;	// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ElementSingularXYZValue = currentLastSingularXYZValueInColumn;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInColumn);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInColumn;
				tempMorphMeta.collectionKey = currentFirstKeyInColumn;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsUsedT2Key = 1;
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
void OGLMBufferManager::MorphT2TerrainBufferAbove()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, 0, z)].ContainsUsedT2Key == 1)
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

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y + 1, z)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentLastKeyInColumn;		// store the value of currentFirstKeyInRow
			currentLastKeyInColumn.y += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementCollectionKey = currentLastKeyInColumn;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ElementSingularXYZValue = currentFirstSingularXYZValueInColumn;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInColumn);
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInColumn;
				tempMorphMeta.collectionKey = currentLastKeyInColumn;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, T2_cubesize - 1, z)].ContainsUsedT2Key = 1;
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
void OGLMBufferManager::MorphT2TerrainBufferNorth()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsUsedT2Key == 1)
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

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z - 1)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentFirstKey = currentFirstKeyInRow;		// store the value of currentFirstKeyInRow
			currentFirstKeyInRow.z -= 1;																					// subtract 1 from the currentFirstKeyInRow
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementCollectionKey = currentFirstKeyInRow;	// put new value of currentFirstKeyInRow into the new element that is the first key in the row (the new element was previously the last element in this row, prior to the shift)
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ElementSingularXYZValue = currentLastSingularXYZValueInRow;	// store the new singular XYZ value in the new first element
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentFirstKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentLastSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentFirstKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsUsedT2Key = 1;
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
void OGLMBufferManager::MorphT2TerrainBufferSouth()
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
			int currentContainsUsedT2KeyValue = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsUsedT2Key;
			if (OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, 0)].ContainsUsedT2Key == 1)
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

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z + 1)].ContainsUsedT2Key;
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, z)].ContainsUsedT2Key = ContainsUsedT2KeyValueToShift;
			}
			EnclaveKeyDef::EnclaveKey originalCurrentLastKey = currentLastKeyInRow;
			currentLastKeyInRow.z += 1;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementCollectionKey = currentLastKeyInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ElementSingularXYZValue = currentFirstSingularXYZValueInRow;
			OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsUsedT2Key = currentContainsUsedT2KeyValue;
			//cout << "key output: " << currentLastKeyInRow.x << ", " << currentLastKeyInRow.y << ", " << currentLastKeyInRow.z << ", " << endl;
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentLastKeyInRow);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				//OrganicMorphMeta tempMorphMeta;
				tempMorphMeta.subBufferIndex = currentFirstSingularXYZValueInRow;
				tempMorphMeta.collectionKey = currentLastKeyInRow;
				organicSystemPtr->T2CollectionProcessingQueue.push(tempMorphMeta);
				OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(x, y, T2_cubesize - 1)].ContainsUsedT2Key = 1;
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

void OGLMBufferManager::MorphT1TerrainBufferWest()
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;			// iterator for blueprint checking
	int T2_LowerNWLocation = (T2_cubesize / 2) - (T1_cubesize / 2); // get the T2 lower NW location, to begin appropriate incrementing of index to find below (going negative x)
	for (int y = 0; y < T1_cubesize; y++)
	{
		for (int z = 0; z < T1_cubesize; z++)
		{
			for (int x = T1_cubesize - 1; x > 0; x--)
			{
				// part 1 -- shift to the east
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x - 1, y, z)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;							// replace element at x + 1 with this collection key

				int valueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x - 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int ContainsUsedT1KeyValueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x - 1, y, z)].ContainsUsedT1Key;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ContainsUsedT1Key = ContainsUsedT1KeyValueToShift;
			}
			// part 2 -- replace element that wasn't shifted, with the appropriate values from the T1 array
			OGLMRenderMetaContainerElementT2 currentT2Element = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_LowerNWLocation, T2_LowerNWLocation + y, T2_LowerNWLocation + z)];
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(0, y, z)].ElementCollectionKey = currentT2Element.ElementCollectionKey;			// transfer copies from  T2 to T1
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(0, y, z)].ElementSingularXYZValue = currentT2Element.ElementSingularXYZValue;	// ""
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(0, y, z)].ContainsUsedT1Key = currentT2Element.ContainsUsedT2Key;				// ""

			// part 3 -- check blueprint map
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentT2Element.ElementCollectionKey);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				cout << "blueprint found! (T1 shift) " << endl;
				EnclaveKeyDef::EnclaveKey tempKey = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(0, 0, 0)].ElementCollectionKey;
				EnclaveKeyDef::EnclaveKey tempKey2 = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_LowerNWLocation, T2_LowerNWLocation, T2_LowerNWLocation)].ElementCollectionKey;
				cout << "(WEST SHIFT) New value of element at " << T2_cubesize - 1 << " , " << 0 << ", " << 0 << " in T1 array is:" << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << endl;
				cout << "(WEST SHIFT) New value of element at " << T2_LowerNWLocation << " , " << T2_LowerNWLocation << ", " << T2_LowerNWLocation << " in T2 array is:" << tempKey2.x << ", " << tempKey2.y << ", " << tempKey2.z << endl;

			}
			else
			{

			}
		}
	}

	


}

void OGLMBufferManager::MorphT1TerrainBufferEast()
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;			// iterator for blueprint checking
	int T2_LowerNELocationX = (T2_cubesize / 2) + (T1_cubesize / 2); // get the T2 lower NE location, to begin appropriate incrementing of index to find below (going positive x)
	int T2_LowerNELocationY = (T2_cubesize / 2) - (T1_cubesize / 2);
	int T2_LowerNELocationZ = (T2_cubesize / 2) - (T1_cubesize / 2);
	for (int y = 0; y < T1_cubesize; y++)
	{
		for (int z = 0; z < T1_cubesize; z++)
		{
			for (int x = 0; x < T1_cubesize - 1; x++)
			{
				// part 1 -- shift to the west
				
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x + 1, y, z)].ElementCollectionKey;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x + 1, y, z)].ElementSingularXYZValue;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int ContainsUsedT2KeyValueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x + 1, y, z)].ContainsUsedT1Key;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ContainsUsedT1Key = ContainsUsedT2KeyValueToShift;
				
			}
			// part 2 -- replace element that wasn't shifted, with the appropriate values from the T1 array

			
			
			OGLMRenderMetaContainerElementT2 currentT2Element = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_LowerNELocationX, T2_LowerNELocationY + y, T2_LowerNELocationZ + z)];
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(T1_cubesize - 1, y, z)].ElementCollectionKey = currentT2Element.ElementCollectionKey;			// transfer copies from  T2 to T1
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(T1_cubesize - 1, y, z)].ElementSingularXYZValue = currentT2Element.ElementSingularXYZValue;	// ""
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(T1_cubesize - 1, y, z)].ContainsUsedT1Key = currentT2Element.ContainsUsedT2Key;				// ""

																																									
																																									// part 3 -- check blueprint map
			blueprintMapIterator = blueprintMatrixPtr->BlueprintMap.find(currentT2Element.ElementCollectionKey);		// attempt to find the blueprint
			if (blueprintMapIterator != blueprintMatrixPtr->BlueprintMap.end())					// if it isn't equal to end, it was found.
			{
				cout << "blueprint found! (T1 shift) " << endl;
				EnclaveKeyDef::EnclaveKey tempKey = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(T1_cubesize - 1, 0, 0)].ElementCollectionKey;
				EnclaveKeyDef::EnclaveKey tempKey2 = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_LowerNELocationX, T2_LowerNELocationY, T2_LowerNELocationZ)].ElementCollectionKey;
				cout << "(EAST SHIFT) New value of element at " << T2_cubesize - 1 << " , " << 0 << ", " << 0 << " in T1 array is:" << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << endl;
				cout << "(EAST SHIFT) New value of element at " << T2_LowerNELocationX << " , " << T2_LowerNELocationY << ", " << T2_LowerNELocationZ << " in T2 array is:" << tempKey2.x << ", " << tempKey2.y << ", " << tempKey2.z << endl;

			}
			else
			{

			}
			

		}
	}

}

void OGLMBufferManager::MorphT1TerrainBufferBelow()
{

}

void OGLMBufferManager::MorphT1TerrainBufferAbove()
{

}

void OGLMBufferManager::MorphT1TerrainBufferNorth()
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;			// iterator for blueprint checking
	int T2_LowerNWLocation = (T2_cubesize / 2) - (T1_cubesize / 2); // get the T2 lower NW location, to begin appropriate incrementing of index to find below (going negative z)
	for (int y = 0; y < T1_cubesize; y++)
	{
		for (int x = 0; x < T1_cubesize; x++)
		{
			for (int z = T1_cubesize - 1; z > 0; z--)
			{
				EnclaveKeyDef::EnclaveKey keyToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z - 1)].ElementCollectionKey;	// get the value of the collection key at x,y,z
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementCollectionKey = keyToShift;

				int valueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z - 1)].ElementSingularXYZValue;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ElementSingularXYZValue = valueToShift;

				int ContainsUsedT1KeyValueToShift = OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z - 1)].ContainsUsedT1Key;
				OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, z)].ContainsUsedT1Key = ContainsUsedT1KeyValueToShift;
			}
			OGLMRenderMetaContainerElementT2 currentT2Element = OGLMRMC.T2_renderMetaContainerArray[T2_translateXYZToSingle(T2_LowerNWLocation + x, T2_LowerNWLocation + y, T2_LowerNWLocation)];
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, 0)].ElementCollectionKey = currentT2Element.ElementCollectionKey;			// transfer copies from  T2 to T1
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, 0)].ElementSingularXYZValue = currentT2Element.ElementSingularXYZValue;	// ""
			OGLMRMC.T1_renderMetaContainerArray[T1_translateXYZToSingle(x, y, 0)].ContainsUsedT1Key = currentT2Element.ContainsUsedT2Key;

		}

	}
}

void OGLMBufferManager::MorphT1TerrainBufferSouth()
{

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

void OGLMBufferManager::setPhaseInDCM()
{
	DCMPtr->isPhaseInitial = 1;
}