#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "EnclaveBlockRayTracker.h"

EnclaveBlockRayTracker::EnclaveBlockRayTracker(CursorPathTraceContainer xContainer, CursorPathTraceContainer yContainer, CursorPathTraceContainer zContainer, EnclaveCollection* collectionPtr)
{
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

	// pointer to collection
	currentCollectionPtr = collectionPtr;
	enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];

	cout << "Chunk: (" << enclaveKey.x << ", " << enclaveKey.y << ", " << enclaveKey.z << ") " << endl;
	cout << "Chunk unique key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")" << endl;

}

int EnclaveBlockRayTracker::MoveEast()
{
	if (blockKey.x < 3)
	{
		blockKey.x += 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.x == 3)
	{
		if (enclaveKey.x <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.x += 1;		// increment the key by 1
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
			blockKey.x = 0;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveWest()
{
	if (blockKey.x > 0)
	{
		blockKey.x -= 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.x == 0)
	{
		if (enclaveKey.x >= 1)
		{
			enclaveKey.x -= 1;
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
			blockKey.x = 3;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveNorth()
{
	if (blockKey.z > 0)
	{
		blockKey.z -= 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.z == 0)
	{
		if (enclaveKey.z >= 1)
		{
			enclaveKey.z -= 1;
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
			blockKey.z = 3;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveSouth()
{
	if (blockKey.z < 3)
	{
		blockKey.z += 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.z == 3)
	{
		if (enclaveKey.z <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.z += 1;		// increment the key by 1
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
			blockKey.z = 0;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;			// return 0 if no block found
}

int EnclaveBlockRayTracker::MoveAbove()
{
	if (blockKey.y < 3)
	{
		blockKey.y += 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.y == 3)
	{
		if (enclaveKey.y <= 6)		// move over one enclave to the east, as long as we aren't at the Enclave where x = 7
		{
			enclaveKey.y += 1;		// increment the key by 1
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z]; // set enclavePtr to the new value
			blockKey.y = 0;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int EnclaveBlockRayTracker::MoveBelow()
{
	if (blockKey.y > 0)
	{
		blockKey.y -= 1;
		int numBlocks = enclavePtr->TotalRenderable;
		int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
		cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
		for (int xx = 0; xx < numBlocks; xx++)
		{
			if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
			{
				return 1;
			}
		}
	}
	else if (blockKey.y == 0)
	{
		if (enclaveKey.y >= 1)
		{
			enclaveKey.y -= 1;
			enclavePtr = &currentCollectionPtr->EnclaveArray[enclaveKey.x][enclaveKey.y][enclaveKey.z];
			blockKey.y = 3;
			int numBlocks = enclavePtr->TotalRenderable;
			int blockSingularValue = enclavePtr->EnclaveCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
			cout << "current location; Enclave key: (" << enclavePtr->UniqueKey.x << ", " << enclavePtr->UniqueKey.y << ", " << enclavePtr->UniqueKey.z << ")  || Block key: (" << blockKey.x << ", " << blockKey.y << ", " << blockKey.z << ")" << endl;
			for (int xx = 0; xx < numBlocks; xx++)
			{
				if (enclavePtr->Sorted.PolyArrayIndex[xx] == blockSingularValue)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}