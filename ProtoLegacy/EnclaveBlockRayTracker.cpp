#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "EnclaveBlockRayTracker.h"


EnclaveBlockRayTracker::EnclaveBlockRayTracker(CursorPathTraceContainer xContainer, CursorPathTraceContainer yContainer, CursorPathTraceContainer zContainer, EnclaveCollectionState* collectionStatePtr, EnclaveCollectionStateArray* collectionStateArrayPtr, int centerIndex, OrganicBlockTarget* inTargetPtr)
{
	// set state array
	currentCollectionStateArray = collectionStateArrayPtr;

	// set OrganicBlockTarget pointer
	targetVertexData = inTargetPtr;

	// set up coordinates

	// colllection
	collectionKey.x = int(xContainer.CollectionCoord);
	collectionKey.y = int(yContainer.CollectionCoord);
	collectionKey.z = int(zContainer.CollectionCoord);

	// chunk
	enclaveKey.x = int(xContainer.ChunkCoord);
	enclaveKey.y = int(yContainer.ChunkCoord);
	enclaveKey.z = int(zContainer.ChunkCoord);

	// block
	blockKey.x = int(xContainer.BlockCoord);
	blockKey.y = int(yContainer.BlockCoord);
	blockKey.z = int(zContainer.BlockCoord);

	// array key; set the initial values of arrayKey to be the center
	arrayKey.x = currentCollectionStateArray->centerCollectionStateOffset;
	arrayKey.y = currentCollectionStateArray->centerCollectionStateOffset;
	arrayKey.z = currentCollectionStateArray->centerCollectionStateOffset;

	// pointer to collection
	currentCollectionState = &collectionStatePtr[centerIndex];			// get the state at the center
	//	currentCollectionState = &collectionStatePtr[collectionStatePtr->]
	if (currentCollectionState->isActive == 1)							// check if its active
	{
		//cout << "active!" << endl;
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
			// cout << "east move inner" << endl;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			//cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
					//cout << "fill block found!" << endl;
					return 1;
				}
			}
		}
		else
		{
			//cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
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
				//cout << "Moved EAST: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						//cout << "fill block found! (1) " << endl;
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
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
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];																																									// get the state at x + 1
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
				//cout << "RayTracker has entered a new collection (east); (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						//cout << "Return from east entry..." << endl;
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						//cout << "fill block found! (2) " << endl;
						return 1;
					}
				}
			}
			else
			{
				//cout << "RayTracker has entered a new collection (east, from an empty collection)" << endl;
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
			//cout << "Moved WEST: current location (0); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
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
				//cout << "Moved WEST: current location (1); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
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
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];
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
				//cout << "Moved WEST: current location (2); Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				//cout << "RayTracker has entered a new collection (west)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}
				
			}
			else
			{
				//cout << "RayTracker has entered a new collection (west, from an empty collection)" << endl;
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
			//cout << "Moved NORTH (0): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
					//cout << "north acquired. (1) " << endl;
					return 1;
				}
			}
		}
		else
		{
			//cout << "north NOT acquired (1)" << endl;
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
				//cout << "Moved NORTH (1): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						//cout << "north acquired. (2) " << endl;
						return 1;
					}
				}
			}
			else
			{
				blockKey.z = 3;
				//cout << "north NOT acquired (2)" << endl;
				return 0;
			}
		}
		else
		{
			//cout << "Entering final ELSE, part 1 " << endl;
			// enclave key's y is equal to 0
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset - 1); // determine value for z - 1
			arrayKey.z -= 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];
			//cout << "Entering final ELSE, part 2 " << endl;
			if (currentCollectionState->isActive == 1)															// check if its active
			{
				//cout << "Entering final ELSE, A part 1 " << endl;
				currentCollectionPtr = currentCollectionState->collectionPtr;									// if it is active, set the pointer
				collectionKey.z -= 1;																			// decrement collectionKey.z value by 1
				enclaveKey.z = 7;																				// set enclave key's z value to be 7 (since we entered a new Collection by going north) 
				blockKey.z = 3;																					// set block key's z value to also be 3 (since it entered a new Enclave by going north)
				enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][7];
				//cout << "Entering final ELSE, A part 2 " << endl;
				isCurrentCollectionActive = 1;
				int numBlocks = enclavePtr->TotalRenderable;													// check the newly entered enclave, for the TotalRenderable
				int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);	// get the singular value
				//cout << "Entering final ELSE, A part 3 " << endl;
				//cout << "Moved NORTH (new collection entry): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				//cout << "Number of blocks in newly entered enclave is: " << numBlocks << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						//cout << "final ELSE return 1 BEGIN" << endl;
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						//cout << "final ELSE return 1 END" << endl;
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
				//cout << "final ELSE return, no collection found" << endl;
				isCurrentCollectionActive = 0;
			}

		}
	}
	//cout << "final ELSE return 0" << endl;
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
			//cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
					return 1;
				}
			}
		}
		else
		{
			//cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
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
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}
			}
			else
			{	
				blockKey.z = 0;
				//cout << "Moved SOUTH: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				return 0;
			}
		}
		else
		{
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset + 1); // determine value for z + 1
			arrayKey.z += 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];
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
				//cout << "RayTracker has entered a new collection (south); (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						//cout << "Return from south entry..." << endl;
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}
			}
			else
			{
				//cout << "RayTracker has entered a new collection (south, from an empty collection)" << endl;
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
			//cout << "Moved ABOVE: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			//cout << "pre Move-above" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
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
				//cout << "Moved ABOVE (2): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
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
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];
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
				//cout << "RayTracker has entered a new collection (above)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}
			}
			else
			{
				//cout << "RayTracker has entered a new collection (above, from an empty collection)" << endl;
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
			//cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					targetVertexData->hasAcquiredTarget = 1;
					fillBlockTargetVertexData(blockSingularValue);
					return 1;
				}
			}
		}
		else
		{
			//cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
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
				//cout << "Moved BELOW (y was 0): current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				for (int xx = 0; xx < numBlocks; xx++)
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}
			}
			else
			{
				blockKey.y = 3;
				//cout << "Moved BELOW: current location; Enclave key: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
				return 0;
			}
		}
		else
		{
			//int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(currentCollectionStateArray->centerCollectionStateOffset, currentCollectionStateArray->centerCollectionStateOffset - 1, currentCollectionStateArray->centerCollectionStateOffset); // determine value for y - 1
			arrayKey.y -= 1;
			int tempIndexVal = currentCollectionStateArray->translateXYZToSingle(arrayKey.x, arrayKey.y, arrayKey.z);
			currentCollectionState = &currentCollectionStateArray->StateMtxPtr[tempIndexVal];
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
				//cout << "RayTracker has entered a new collection (below)" << endl;
				for (int xx = 0; xx < numBlocks; xx++)			// check if the enclave contains any blocks
				{
					if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
					{
						targetVertexData->hasAcquiredTarget = 1;
						fillBlockTargetVertexData(blockSingularValue);
						return 1;
					}
				}

			}
			else
			{
				//cout << "RayTracker has entered a new collection (below, from an empty collection)" << endl;
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

			targetVertexData->targetVertexesXYZ[arrayindex].x = GL_x + targetVertexData->vertexOffsets[testval[(xx * 6) + yy]].x;		// set the +/- difference for the target cube outline for x 
			targetVertexData->targetVertexesXYZ[arrayindex].y = GL_y + targetVertexData->vertexOffsets[testval[(xx * 6) + yy]].y;		// set the +/- difference for the target cube outline for y
			targetVertexData->targetVertexesXYZ[arrayindex].z = GL_z + targetVertexData->vertexOffsets[testval[(xx * 6) + yy]].z;		// set the +/- difference for the target cube outline for z
			//cout << "array index is: " << arrayindex << endl;
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

	ReturnTuple.x = GLfloat(x);
	ReturnTuple.y = GLfloat(y);
	ReturnTuple.z = GLfloat(z);

	return ReturnTuple;
}

void EnclaveBlockRayTracker::printOutTargetBlockHighlightData()
{
	cout << "-----------------------------highlight block data---------------------------" << endl;
	cout << "Upper SOUTHWEST (0): " << targetVertexData->targetVertexesXYZ[0].x << ", " << targetVertexData->targetVertexesXYZ[0].y << ", " << targetVertexData->targetVertexesXYZ[0].z << endl;
	cout << "Upper NORTHWEST (1): " << targetVertexData->targetVertexesXYZ[1].x << ", " << targetVertexData->targetVertexesXYZ[1].y << ", " << targetVertexData->targetVertexesXYZ[1].z << endl;
	cout << "Lower SOUTHWEST (2): " << targetVertexData->targetVertexesXYZ[2].x << ", " << targetVertexData->targetVertexesXYZ[2].y << ", " << targetVertexData->targetVertexesXYZ[2].z << endl;
	cout << "Lower NORTHWEST (3): " << targetVertexData->targetVertexesXYZ[5].x << ", " << targetVertexData->targetVertexesXYZ[5].y << ", " << targetVertexData->targetVertexesXYZ[5].z << endl;
	cout << "Upper SOUTHEAST (4): " << targetVertexData->targetVertexesXYZ[13].x << ", " << targetVertexData->targetVertexesXYZ[13].y << ", " << targetVertexData->targetVertexesXYZ[13].z << endl;
	cout << "Upper NORTHEAST (5): " << targetVertexData->targetVertexesXYZ[7].x << ", " << targetVertexData->targetVertexesXYZ[7].y << ", " << targetVertexData->targetVertexesXYZ[7].z << endl;
	cout << "Lower SOUTHEAST (6): " << targetVertexData->targetVertexesXYZ[17].x << ", " << targetVertexData->targetVertexesXYZ[17].y << ", " << targetVertexData->targetVertexesXYZ[17].z << endl;
	cout << "Lower NORTHEAST (7): " << targetVertexData->targetVertexesXYZ[11].x << ", " << targetVertexData->targetVertexesXYZ[11].y << ", " << targetVertexData->targetVertexesXYZ[11].z << endl;
}