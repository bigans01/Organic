/*------------------------------------------------------------------------------------------

--Enclave.cpp		(Last Update: 7/5/2017)

Description: Enclave.cpp contains the definitions for the declarations found in Enclave.h.

------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OGLMBufferManager.h"
#include "OrganicGLManager.h"

void OGLMBufferManager::SetCubesize(int inCubesize)
{
	cubesize = inCubesize;
}

void OGLMBufferManager::GenerateArrays()
{
	// all arrays must be initialized with one dimension	(change "bufferable element" to something more reasonable later (9/8/2017))
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
				BufferOffsetMatrixArray[currentBufferElement] = (currentBufferElement*OGLMPtr->CollectionBufferSize);
				//std::cout << "test: " << RenderCollectionMatrixArray[currentBufferElement] << std::endl;
			}
		}
	}
	arraysSet = 1;
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
	int x_axis = x * (cubesize*cubesize);	// example, for a cube of length 8, this would be 64.
	int y_axis = y * (cubesize);			// ...this would be 8
	/*Summary: takes in the x/y/z to get the exact location in the buffers for this value */
	return x_axis + y_axis + z;

}

void OGLMBufferManager::SetOGLMPointer(OrganicGLManager* in_OGLMptr)
{
	OGLMPtr = in_OGLMptr;
}