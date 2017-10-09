#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "EnclaveBlockRayTracker.h"


EnclaveBlockRayTracker::EnclaveBlockRayTracker(CursorPathTraceContainer xContainer, CursorPathTraceContainer yContainer, CursorPathTraceContainer zContainer, EnclaveCollectionState* collectionStatePtr, EnclaveCollectionStateArray* collectionStateArrayPtr, int centerIndex)
{
	// set state array
	currentCollectionStateArray = collectionStateArrayPtr;

	// set up coordinates

	// colllection
	collectionKey.x = xContainer.CollectionCoord;
	collectionKey.y = yContainer.CollectionCoord;
	collectionKey.z = zContainer.CollectionCoord;

	// chunk
	enclaveKey.x = xContainer.ChunkCoord;
	enclaveKey.y = yContainer.ChunkCoord;
	enclaveKey.z = zContainer.ChunkCoord;

	// block
	blockKey.x = xContainer.BlockCoord;
	blockKey.y = yContainer.BlockCoord;
	blockKey.z = zContainer.BlockCoord;

	// array key; set the initial values of arrayKey to be the center
	arrayKey.x = currentCollectionStateArray->centerCollectionStateOffset;
	arrayKey.y = currentCollectionStateArray->centerCollectionStateOffset;
	arrayKey.z = currentCollectionStateArray->centerCollectionStateOffset;

	// pointer to collection
	currentCollectionState = &collectionStatePtr[centerIndex];			// get the state at the center
	//	currentCollectionState = &collectionStatePtr[collectionStatePtr->]
	if (currentCollectionState->isActive == 1)							// check if its active
	{
		currentCollectionPtr = currentCollectionState->collectionPtr;	// if it is active, set the pointer
		enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
		isCurrentCollectionActive = 1;
	}

	//cout << "Chunk: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ") " << endl;
	//cout << "Chunk unique key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")" << endl;

}

int EnclaveBlockRayTracker::MoveEast()
{
	//cout << "east move " << endl;
	if (blockKey.x < 3)
	{
		// << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << 
		//cout << "east move (1)" << endl;
		blockKey.x += 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{
			cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			return 0;
		}
	}// --------------------------------------------------
	else if (blockKey.x == 3)
	{
		//cout << "east move (2)" << endl;
		if (enclaveKey.x <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.x += 1;		// increment the key by 1
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
				blockKey.x = 0;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				//cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				blockKey.x = 0;
				return 0;
			}
		}
		else      // do this if x = 7
		{
			// enclave key's x is equal to 7
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset + 1, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset); // determine value for x + 1
			arrayKey.x += 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];																																									// get the state at x + 1
			if (currentCollectionState->isActive == 1)													// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;							// if it is active, set the pointer
				collectionKey.x += 1;																	// increment collectionKey value by 1
				enclaveKey.x = 0;																		// set the enclave key's x value to 0 (since we entered a new Collection and went east)
				blockKey.x = 0;																			// set block key's x value to also be 0 (since it entered a new Enclave by going east)
				enclavePtr = &currentCollectionPtr->EnclaveArray[0][enclaveKey.y][enclaveKey.z];		// since we are going east, and crossing into a new collection, set x to be 0.
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;											// check the newly entered enclave, for the TotalRenderable
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "RayTracker has entered a new collection (east); (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						cout << "Return from east entry..." << endl;
						return 1;
					}
				}
			}
			else
			{
				cout << "RayTracker has entered a new collection (east, from an empty collection)" << endl;
				collectionKey.x += 1;																	// increment collectionKey value by 1
				enclaveKey.x = 0;																		// set the x of the enclaveKey value to 0
				blockKey.x = 0;				// set block key's x value to also be 0 (since it entered a new Enclave by going east)
				isCurrentCollectionActive = 0;
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveWest()
{
	//cout << "west move " << endl;
	if (blockKey.x > 0)
	{
		blockKey.x -= 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved WEST: current location (0); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{

			return 0;
		}
	} // --------------------------------------------------
	else if (blockKey.x == 0)
	{
		if (enclaveKey.x >= 1)
		{
			enclaveKey.x -= 1;
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
				blockKey.x = 3;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				cout << "Moved WEST: current location (1); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				blockKey.x = 3;
				return 0;
			}
		}
		else
		{
			// enclave key's x is equal to 0
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset - 1, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset); // determine value for x - 1
			arrayKey.x -= 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];
			if (currentCollectionState->isActive == 1)															// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;									// if it is active, set the pointer
				collectionKey.x -= 1;																			// decrement collectionKey value by 1
				enclaveKey.x = 7;																				// set enclave key's x value to be 7 (since we entered a new Collection by going west)
				blockKey.x = 3;																					// set block key's x value to also be 3 (since it entered a new Enclave by going west)
				enclavePtr = &currentCollectionPtr->EnclaveArray[7][enclaveKey.y][enclaveKey.z];				// since we are going west, and crossing into a new collection, set x to be 7.
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;													// check the newly entered enclave, for the TotalRenderable
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "Moved WEST: current location (2); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				//cout << "RayTracker has entered a new collection (west)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
				
			}
			else
			{
				cout << "RayTracker has entered a new collection (west, from an empty collection)" << endl;
				collectionKey.x -= 1;																	// decrement collectionKey value by 1
				enclaveKey.x = 7;
				blockKey.x = 3;
				isCurrentCollectionActive = 0;
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveNorth()
{
	//cout << "north move " << endl;
	//cout << "BlockKey.z is: " << blockKey.z << endl;
	if (blockKey.z > 0)
	{
		blockKey.z -= 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved NORTH (0): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{
			return 0;
		}
	}// --------------------------------------------------
	else if (blockKey.z == 0)
	{
		if (enclaveKey.z >= 1)
		{
			enclaveKey.z -= 1;
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
				blockKey.z = 3;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				cout << "Moved NORTH (1): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				blockKey.z = 3;
				return 0;
			}
		}
		else
		{
			// enclave key's y is equal to 0
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset - 1); // determine value for z - 1
			arrayKey.z -= 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];
			if (currentCollectionState->isActive == 1)															// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;									// if it is active, set the pointer
				collectionKey.z -= 1;																			// decrement collectionKey.z value by 1
				enclaveKey.z = 7;																				// set enclave key's z value to be 7 (since we entered a new Collection by going north) 
				blockKey.z = 3;																					// set block key's z value to also be 3 (since it entered a new Enclave by going north)
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][7];
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;													// check the newly entered enclave, for the TotalRenderable
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "Moved NORTH (new collection entry): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}

			}
			else
			{
				//cout << "RayTracker has entered a new collection (north, from an empty collection)" << endl;
				collectionKey.z -= 1;																	// decrement collectionKey value by 1
				enclaveKey.z = 7;
				blockKey.z = 3;
				isCurrentCollectionActive = 0;
			}

		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveSouth()
{
	//cout << "south move " << endl;
	if (blockKey.z < 3)
	{
		blockKey.z += 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{
			cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			return 0;
		} // --------------------------------------------------
	}
	else if (blockKey.z == 3)
	{
		if (enclaveKey.z <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.z += 1;		// increment the key by 1
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
				blockKey.z = 0;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				//cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{	
				blockKey.z = 0;
				cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				return 0;
			}
		}
		else
		{
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset + 1); // determine value for z + 1
			arrayKey.z += 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];
			if (currentCollectionState->isActive == 1)													// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;							// if it is active, set the pointer
				collectionKey.z += 1;																	// increment collectionKey.z value by 1
				enclaveKey.z = 0;																		// set the z of the enclaveKey value to 0
				blockKey.z = 0;																			// set block key's z value to also be 0 (since it entered a new Enclave by going south)
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][0];		// since we are going south, and crossing into a new collection, set z to be 0.
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;													// check the newly entered enclave, for the TotalRenderable
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "RayTracker has entered a new collection (south); (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						//cout << "Return from south entry..." << endl;
						return 1;
					}
				}
			}
			else
			{
				cout << "RayTracker has entered a new collection (south, from an empty collection)" << endl;
				collectionKey.z += 1;																	// increment collectionKey.z value by 1
				enclaveKey.z = 0;																		// set the z of the enclaveKey value to 0
				blockKey.z = 0;				// set block key's z value to also be 0 (since it entered a new Enclave by going east)
				isCurrentCollectionActive = 0;
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveAbove()
{
	//cout << "above move " << endl;
	if (blockKey.y < 3)
	{
		
		blockKey.y += 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved ABOVE: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			//cout << "pre Move-above" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{

			return 0;
		} // --------------------------------------------------
		//cout << "post Move-Above" << endl;
	}
	else if (blockKey.y == 3)
	{
		if (enclaveKey.y <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.y += 1;		// increment the key by 1
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
				blockKey.y = 0;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				cout << "Moved ABOVE (2): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				blockKey.y = 0;
				return 0;
			}
		}
		else
		{
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset + 1, currentCollectionStateArray->centerCollectionStateOffset); // determine value for y + 1
			arrayKey.y += 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];
			if (currentCollectionState->isActive == 1)													// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;							// if it is active, set the pointer
				collectionKey.y += 1;																	// increment collectionKey.y value by 1
				enclaveKey.y = 0;																		// set the y of the enclaveKey value to 0
				blockKey.y = 0;																			// set block key's y value to also be 0 (since it entered a new Enclave by going above)
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][0][enclaveKey.z];		// since we are going south, and crossing into a new collection, set z to be 0.
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "RayTracker has entered a new collection (above)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				cout << "RayTracker has entered a new collection (above, from an empty collection)" << endl;
				collectionKey.y += 1;																	// increment collectionKey.y value by 1
				enclaveKey.y = 0;																		// set the y of the enclaveKey value to 0
				blockKey.y = 0;																			// set block key's y value to also be 0 (since it entered a new Enclave by going above)
				isCurrentCollectionActive = 0;
			}

		}
	}
	return 0;
}

int EnclaveBlockRayTracker::MoveBelow()
{
	//cout << "below move " << endl;
	if (blockKey.y > 0)
	{
		blockKey.y -= 1;
		if (isCurrentCollectionActive == 1)
		{
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
		else
		{
			cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			return 0;
		}
	} // --------------------------------------------------
	else if (blockKey.y == 0)
	{
		if (enclaveKey.y >= 1)
		{
			enclaveKey.y -= 1;
			if (isCurrentCollectionActive == 1)
			{
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
				blockKey.y = 3;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				cout << "Moved BELOW (y was 0): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}
			}
			else
			{
				blockKey.y = 3;
				cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				return 0;
			}
		}
		else
		{
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset - 1, currentCollectionStateArray->centerCollectionStateOffset); // determine value for y - 1
			arrayKey.y -= 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMatrixPtr[tempIndexVal];
			if (currentCollectionState->isActive == 1)													// check if its active
			{
				currentCollectionPtr = currentCollectionState->collectionPtr;							// if it is active, set the pointer
				collectionKey.y -= 1;																	// decrement collectionKey.y value by 1
				enclaveKey.y = 7;																		// set the y of the enclaveKey value to 7
				blockKey.y = 3;																			// set block key's y value to also be 3 (since it entered a new Enclave by going below)
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][7][enclaveKey.z];		// since we are going south, and crossing into a new collection, set z to be 0.
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				cout << "RayTracker has entered a new collection (below)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						return 1;
					}
				}

			}
			else
			{
				cout << "RayTracker has entered a new collection (below, from an empty collection)" << endl;
				collectionKey.y -= 1;																	// decrement collectionKey.y value by 1
				enclaveKey.y = 7;																		// set the y of the enclaveKey value to 7
				blockKey.y = 3;																			// set block key's y value to also be 3 (since it entered a new Enclave by going below)
				isCurrentCollectionActive = 0;
			}
		}
	}
	return 0;
}

void EnclaveBlockRayTracker::fillBlockTargetVertexData(int indexValue)
{
	char testval[36] =    { 0, 1, 2, 1, 2, 3,								// negative x			(WEST)		(32)	// this array could be constant?	
							1, 5, 3, 5, 3, 7,								// negative z			(NORTH)		(16)	// OLD:  1, 5, 3, 5, 3, 7,
							5, 4, 7, 4, 7, 6,								// positive x			(EAST)		(8)
							4, 0, 6, 0, 6, 2,								// positive z			(SOUTH)		(4)		// OLD: 0, 4, 6, 0, 6, 2,
							1, 5, 0, 5, 0, 4,								// positive y			(TOP)		(2)
							3, 7, 2, 7, 2, 6								// negative y			(BOTTOM)	(1)
						   };												// also for testing
	
	int arrayindex = 0;	 // set arrayindex to 0; will be incremented 107 times (to get to 108th index)
	//int iteratorval;	 

	GLfloat GL_x = 0.5f;		// instantiate within stack frame
	GLfloat GL_y = 0.5f;
	GLfloat GL_z = 0.5f;

	// begin block face loop
	EnclaveXYZOffset = SingleToMulti(enclavePtr->Sorted.PolyArrayIndex[indexValue]);
	for (int xx = 0; xx < 6; xx++)
	{

		// begin vertex loop (6 vertices per face)
		for (int yy = 0; yy < 6; yy++)
		{
			GL_x = 0.5f;
			GL_y = 0.5f;
			GL_z = 0.5f;
			// below 3 lines: GL_x = (x of vertex at structarray) + (enclave's unique x * 4) + (x offset)
			GL_x = GL_x * (enclavePtr->Sorted.RenderArray[indexValue]->structarray[testval[(xx * 6) + yy]].x) + ((enclavePtr->UniqueKey.x) * 4) + ((enclavePtr->CollectionKey.x) * 32) + EnclaveXYZOffset.x;			// multiply by x coord of vertex at structarray[...]	array index of: [(j*6) + some int] 
			GL_y = GL_y * (enclavePtr->Sorted.RenderArray[indexValue]->structarray[testval[(xx * 6) + yy]].y) + ((enclavePtr->UniqueKey.y) * 4) + ((enclavePtr->CollectionKey.y) * 32) + EnclaveXYZOffset.y;			// multiply by y coord of vertex at structarray[...]	array index of: [(j*6) + some int]
			GL_z = GL_z * (enclavePtr->Sorted.RenderArray[indexValue]->structarray[testval[(xx * 6) + yy]].z) + ((enclavePtr->UniqueKey.z) * 4) + ((enclavePtr->CollectionKey.z) * 32) + EnclaveXYZOffset.z;			// multiply by z coord of vertex at structarray[...]	array index of: [(j*6) + some int]
			arrayindex++;	// increment array index at the end
		}
	}
}

FloatTupleXYZ EnclaveBlockRayTracker::SingleToMulti(int input)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
	FloatTupleXYZ ReturnTuple;

	int x = input / 16;
	int remainder_x = input % 16;

	int y = remainder_x / 4;
	int remainder_y = remainder_x % 4;

	int z = remainder_y;

	//cout << x << " " << y << " " << z << " " << endl;

	ReturnTuple.x = x;
	ReturnTuple.y = y;
	ReturnTuple.z = z;

	return ReturnTuple;
}