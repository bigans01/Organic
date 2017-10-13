/*------------------------------------------------------------------------------------------

--Enclave.cpp		(Last Update: 7/5/2017)

Description: Enclave.cpp contains the definitions for the declarations found in Enclave.h.

------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OGLMBufferManager.h"
#include "OrganicGLManager.h"

void OGLMBufferManager::SetCubesize(int inCubesize)
{
	cubesize = inCubesize;
}

void OGLMBufferManager::GenerateArrays()
{
	// all arrays must be initialized with one dimension	(change "bufferable element" term to something more reasonable later (9/8/2017))
	BufferOffsetMatrixArray = new int[cubesize*cubesize*cubesize];		// stores the exact start position in the buffer for each bufferable element
	RenderCollectionMatrixArray = new int[cubesize*cubesize*cubesize];	// stores the individual "locations" ("sub-bufffers") for each bufferable element
	RenderableBufferList = new int[cubesize*cubesize*cubesize];			// a sorted list of renderable bufferable elements

	// initialize arrays; 
	for (int x = 0; x < cubesize; x++)	// x axis
	{
		for (int y = 0; y < cubesize; y++)
		{
			for (int z = 0; z < cubesize; z++)
			{
				int currentBufferElement = translateXYZToSingle(x, y, z);
				RenderCollectionMatrixArray[currentBufferElement] = currentBufferElement;	
				BufferOffsetMatrixArray[currentBufferElement] = (currentBufferElement*OGLMPtr->CollectionBufferSize);		// buffer offset will be equal to the value of the OGLMPtr's collection buffer size
				//std::cout << "test: " << RenderCollectionMatrixArray[currentBufferElement] << " : " << BufferOffsetMatrixArray[currentBufferElement] << std::endl;
			}
		}
	}
	arraysSet = 1;
}

void OGLMBufferManager::PopulateOGLMRMCArrays(EnclaveKeyDef::EnclaveKey centerCollectionKey)
{
	cout << "OGLMRMC key is: " << centerCollectionKey.x << ", " << centerCollectionKey.y << ", " << centerCollectionKey.z << endl;
	int centerCollectionOffset = cubesize / 2;				// if cubesize is 13, this would be equal to 6; element location 6 on any x/y/z access would be the center
	currentCenterCollectionKey = centerCollectionKey;
	EnclaveKeyDef::EnclaveKey lowerNWCornerKey = centerCollectionKey;				// the key of the collection that would be in the lower most corner; it is initiually set to the input value
	lowerNWCornerKey.x -= centerCollectionOffset;			// subtract the key's x value by the offset
	lowerNWCornerKey.y -= centerCollectionOffset;			// subtract the key's y value by the offset
	lowerNWCornerKey.z -= centerCollectionOffset;			// subtract the key's z value by the offset
	for (int x = 0; x < cubesize; x++)	// x axis
	{
		for (int y = 0; y < cubesize; y++)
		{
			for (int z = 0; z < cubesize; z++)
			{
				int currentBufferElement = translateXYZToSingle(x, y, z);
				EnclaveKeyDef::EnclaveKey elementCollectionKey = lowerNWCornerKey;
				elementCollectionKey.x += x;
				elementCollectionKey.y += y;
				elementCollectionKey.z += z;
				OGLMRMC.renderMetaContainerArray[currentBufferElement].ElementCollectionKey = elementCollectionKey;
			}
		}
	}
	EnclaveKeyDef::EnclaveKey testKey = OGLMRMC.renderMetaContainerArray[translateXYZToSingle(centerCollectionOffset, centerCollectionOffset, centerCollectionOffset)].ElementCollectionKey;
	cout << "center key after work is: " << testKey.x << ", " << testKey.y << ", " << testKey.z << endl;
}

OGLMBufferManager::~OGLMBufferManager()
{
	if (arraysSet == 1)
	{
		delete[] BufferOffsetMatrixArray;
		delete[] RenderCollectionMatrixArray;
		delete[] RenderableBufferList;
	}
}

int OGLMBufferManager::translateXYZToSingle(int x, int y, int z)
{
	//int x_axis = x * (cubesize*cubesize);	// example, for a cube of length 8, this would be 64.
	//int y_axis = y * (cubesize);			// ...this would be 8
	/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	//return x_axis + y_axis + z;

	int x_axis = z * (cubesize*cubesize);	// example, for a cube of length 8, this would be 64.
	int y_axis = y * (cubesize);			// ...this would be 8
											/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	return x_axis + y_axis + x;

}

void OGLMBufferManager::SetOGLMPointer(OrganicGLManager* in_OGLMptr)
{
	OGLMPtr = in_OGLMptr;
}

void OGLMBufferManager::ShiftMatricesForNW()
{
	/* Summmary: shifts the RenderCollectionMatrixArray (using variables prefixed by R_)
	   and the BufferOffsetMatrixarray (using variables prefixed by B_) to the right, and then down. 
	   The result will give the IDs of the buffers that can be updated. */

	// currently used as a speed test only; currently shifts one layer on between y = 0 and y = 1 for NW movement
	auto carvestart = std::chrono::high_resolution_clock::now();


	// First, shift matrices for West
	for (int y = 0; y < cubesize; y++)
	{
		for (int z = 0; z < cubesize; z++)
		{
			int R_lastValueAtEndOfLine = RenderCollectionMatrixArray[translateXYZToSingle(cubesize - 1, y, z)];			// (RenderCollection, "R_"): get the value of the last element in this "line"
			int B_lastValueAtEndOfLine = BufferOffsetMatrixArray[translateXYZToSingle(cubesize - 1, y, z)];				// (BufferOffset, "B_"): get the value of the last element in this "line"
			int R_coutFirst = RenderCollectionMatrixArray[translateXYZToSingle(0, y, z)];								// for output test only
			int B_coutFirst = BufferOffsetMatrixArray[translateXYZToSingle(0, y, z)];

			for (int x = 0; x < cubesize - 1; x++)
			{
				int R_currentValue = RenderCollectionMatrixArray[translateXYZToSingle(x, y, z)];						// (RenderCollection, "R_"): get the value of the remaining elements
				RenderCollectionMatrixArray[translateXYZToSingle(x + 1, y, z)] = R_currentValue;						// (RenderCollection, "R_"): ...and replace the element at x+1 with that value.

				int B_currentValue = BufferOffsetMatrixArray[translateXYZToSingle(x, y, z)];							// (BufferOffset, "B_"): get the value of the remaining elements
				BufferOffsetMatrixArray[translateXYZToSingle(x + 1, y, z)] = B_currentValue;							// (BufferOffset, "B_"): ...and replace the element at x+1 with that value.
			}

			RenderCollectionMatrixArray[translateXYZToSingle(0, y, z)] = R_lastValueAtEndOfLine;				// (RenderCollection, "R_"): set the first element to the last value
			BufferOffsetMatrixArray[translateXYZToSingle(0, y, z)] = B_lastValueAtEndOfLine;					// (BufferOffset, "B_"): set the first element to the last value

			int R_coutSecond = RenderCollectionMatrixArray[translateXYZToSingle(0, y, z)];
			int B_coutSecond = BufferOffsetMatrixArray[translateXYZToSingle(0, y, z)];

			//std::cout << "Element before move: " << R_coutFirst << " | " << B_coutFirst << "        After move: " << R_coutSecond << " | " << B_coutSecond << endl;
			//std::cout << "Element after move: " << coutSecond << endl;
		}
	}


	//Second, shift matrices for north


	//int coutSecond = RenderCollectionMatrixArray[translateXYZToSingle(0, 0, z)];




	auto carveend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> carveelapsed = carveend - carvestart;
	std::cout << "Elapsed time: (matrix shift for NW, all lines (" << cubesize*cubesize << "): " << carveelapsed.count() << endl;
	//std::cout << "Element before move: " << coutFirst << endl;
	//std::cout << "Element after move: " << coutSecond << endl;

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

int OGLMBufferManager::determineRenderDataSubBufferKey(EnclaveKeyDef::EnclaveKey renderCollectionKey)
{
	/*Summary:
	  compares the input value to the currentCenterCollectionKey
	*/

	// set the initial offsets to be equal to the center
	int x_offset = (cubesize / 2);
	int y_offset = (cubesize / 2);
	int z_offset = (cubesize / 2);
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
	int XYZtoSingleResult = translateXYZToSingle(x_offset, y_offset, z_offset);
	cout << "XYZ to single: " << translateXYZToSingle(x_offset, y_offset, z_offset) << endl;
	return XYZtoSingleResult;
}