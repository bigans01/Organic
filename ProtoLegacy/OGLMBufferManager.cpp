/*------------------------------------------------------------------------------------------

--Enclave.cpp		(Last Update: 7/5/2017)

Description: Enclave.cpp contains the definitions for the declarations found in Enclave.h.

------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include <stdio.h>
#include "OGLMBufferManager.h"

void OGLMBufferManager::SetCubesize(int inCubesize)
{
	cubesize = inCubesize;
}

void OGLMBufferManager::GenerateArrays()
{
	BufferOffsetMatrixArray = new int[cubesize*cubesize*cubesize];			// all arrays must be initialized with one dimension
	RenderCollectionMatrixArray = new int[cubesize*cubesize*cubesize];
	RenderableBufferList = new int[cubesize*cubesize*cubesize];
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