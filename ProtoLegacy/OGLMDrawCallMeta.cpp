#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "OGLMDrawCallMeta.h"

void OGLMDrawCallMeta::setGLintDynamicArrays(int in_cubeSize, int in_subBufferSize)
{


	//EnclaveKeyDef::EnclaveKey* DCM_CollectionKeys;
	//int* DCM_SubBufferLocation;
	//DCMD (delegator) arrays
	DCMD_SubBufferContents = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCMD_SubBufferLocation = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCMD_RemainingVertexSpace = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCMD_TT1_GL_BufferOffset = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCMD_TT2_GL_BufferOffset = new int[in_cubeSize*in_cubeSize*in_cubeSize];

	//TT1 arrays
	TT1_SubBufferContents = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	TT1_CollectionKeys = new EnclaveKeyDef::EnclaveKey[in_cubeSize*in_cubeSize*in_cubeSize];
	TT1_SubBufferLocation = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	TT1_GL_BufferOffset = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];
	TT1_GL_VertexArraySize = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];

	array_cubesize = in_cubeSize;
	array_length = in_cubeSize*in_cubeSize*in_cubeSize;
	subBufferByteSize = in_subBufferSize;
	areDynamicArraysSet = 1;
}

OGLMDrawCallMeta::~OGLMDrawCallMeta()
{
	if (areDynamicArraysSet == 1)
	{
		// delegator buffers (DCMD)
		delete[] DCMD_SubBufferContents;
		delete[] DCMD_SubBufferLocation;
		delete[] DCMD_RemainingVertexSpace;
		delete[] DCMD_TT1_GL_BufferOffset;
		delete[] DCMD_TT2_GL_BufferOffset;

		// terrain type 1 buffers (TT1)
		delete[] TT1_SubBufferContents;
		delete[] TT1_CollectionKeys;
		delete[] TT1_SubBufferLocation;
		delete[] TT1_GL_BufferOffset;
		delete[] TT1_GL_VertexArraySize;
	}
}

void OGLMDrawCallMeta::addToListAndSort(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize)
{
	// search for an element in the list
	int doesElementExist = 0;
	
	for (int x = 0; x < array_length; x++)
	{
		// find the corresponding location of the sub-buffer's ID (with a range of 0 to array_length)
		if (TT1_SubBufferLocation[x] == in_subBufferIndex)
		{
			// check to see if the sub buffer is completely empty.
			if (TT1_SubBufferContents[x] == 0)
			{
				// set up temp swap variables
				int temp_TT1_SubBufferContents = TT1_SubBufferContents[indexValueOfFirstEmpty];
				EnclaveKeyDef::EnclaveKey temp_TT1_CollectionKey = TT1_CollectionKeys[indexValueOfFirstEmpty];
				int temp_TT1_SubBufferLocation = TT1_SubBufferLocation[indexValueOfFirstEmpty];
				int temp_TT1_GL_BufferOffset = TT1_GL_BufferOffset[indexValueOfFirstEmpty];
				int temp_TT1_GL_VertexArraySize = TT1_GL_VertexArraySize[indexValueOfFirstEmpty];

				// set the values at the first empty index
				TT1_SubBufferContents[indexValueOfFirstEmpty] = 1;
				TT1_CollectionKeys[indexValueOfFirstEmpty] = in_key;								// set the key, in the corresponding current element of dynamic array
				TT1_SubBufferLocation[indexValueOfFirstEmpty] = in_subBufferIndex;					// set the location, in the corresponding current element of dynamic array
				TT1_GL_BufferOffset[indexValueOfFirstEmpty] = in_subBufferIndex * (in_subBufferByteSize / 12);				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
				TT1_GL_VertexArraySize[indexValueOfFirstEmpty] = in_vertexArrayByteSize / 12;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array

				// set the values at x
				TT1_SubBufferContents[x] = temp_TT1_SubBufferContents;
				TT1_CollectionKeys[x] = temp_TT1_CollectionKey;														// set the key, in the corresponding current element of dynamic array
				TT1_SubBufferLocation[x] = temp_TT1_SubBufferLocation;											// set the location, in the corresponding current element of dynamic array
				TT1_GL_BufferOffset[x] = temp_TT1_GL_BufferOffset;				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
				TT1_GL_VertexArraySize[x] = temp_TT1_GL_VertexArraySize;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array

				indexValueOfFirstEmpty++;
				numberOfRenderableCollections++;
			}

			// check to see if the sub buffer contains terrain data
			else if (TT1_SubBufferContents[x] == 1)
			{
				// will just replace the data in this buffer
			}

			// check to see if the sub buffer contains cursor (targeting block) data
			else if (TT1_SubBufferContents[x] == 2)
			{
				// will move the existing data to the next empty buffer, and will replace this buffer with the data specified by the input parameter
			}

			// check to see if the sub buffer contains type 2 terrain data
			else if (TT1_SubBufferContents[x] == 3)
			{
				// will move the existing data to the next empty buffer, and replace this buffer with the data specified by the  input parameter
			}
		}
	}
	//std::cout << "Index value of First empty: " << DCM_CollectionKeys[indexValueOfFirstEmpty].x << ", " << DCM_CollectionKeys[indexValueOfFirstEmpty].y << ", " << DCM_CollectionKeys[indexValueOfFirstEmpty].z << std::endl;
	/*
	if (doesElementExist == 0)
	{
		DCM_SubBufferContents[numberOfRenderableCollections] = 1;
		DCM_CollectionKeys[numberOfRenderableCollections] = in_key;														// set the key, in the corresponding current element of dynamic array
		DCM_SubBufferLocation[numberOfRenderableCollections] = in_subBufferIndex;											// set the location, in the corresponding current element of dynamic array
		DCM_GL_BufferOffset[numberOfRenderableCollections] = in_subBufferIndex * (in_subBufferByteSize / 12);				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
		DCM_GL_VertexArraySize[numberOfRenderableCollections] = in_vertexArrayByteSize / 12;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array
		numberOfRenderableCollections++;		// increment the number of renderable collections
	}
	*/
}

void OGLMDrawCallMeta::sendTerrainT1RequestToDelegator(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize)
{
	for (int x = 0; x < array_length; x++)
	{
		if (DCMD_SubBufferLocation[x] == in_subBufferIndex)
		{
			// check to see if contents are empty
			if (DCMD_SubBufferContents[x] == 0)
			{
				//std::cout << "Remaining vertices: " << DCMD_RemainingVertexSpace[x] << std::endl;
				DCMD_RemainingVertexSpace[x] = DCMD_RemainingVertexSpace[x] - (in_vertexArrayByteSize / 12);	// subtract the vertexes being added to the array, from the total amount remaining and store the result
				DCMD_TT1_GL_BufferOffset[x] = in_subBufferIndex * (in_subBufferByteSize / 12);					// store the starting vertex of the TT1 data in the sub buffer
				//sendTerrainT1RequestToDelegator(in_key, in_subBufferIndex, in_vertexArrayByteSize, in_subBufferByteSize);
				addToListAndSort(in_key, in_subBufferIndex, in_vertexArrayByteSize, in_subBufferByteSize);
				DCMD_SubBufferContents[x] = 1;
				//std::cout << "Vertices subtracted: " << (in_vertexArrayByteSize / 12) << std::endl;
				//std::cout << "Remaining verties (2): " << DCMD_RemainingVertexSpace[x] << std::endl;
			}

			// needs new statement for when contents are not empty (i.e., already contains T1 data) (11/13/2017)
		}
	}
}

void OGLMDrawCallMeta::setInitialDynamicArrayData(EnclaveKeyDef::EnclaveKey centerCollectionKey)
{
	int centerCollectionOffset = array_cubesize / 2;				// if cubesize is 13, this would be equal to 6; element location 6 on any x/y/z access would be the center
	EnclaveKeyDef::EnclaveKey lowerNWCornerKey = centerCollectionKey;
	lowerNWCornerKey.x -= centerCollectionOffset;			// subtract the key's x value by the offset
	lowerNWCornerKey.y -= centerCollectionOffset;			// subtract the key's y value by the offset
	lowerNWCornerKey.z -= centerCollectionOffset;			// subtract the key's z value by the offset
	int currentIndexValue = 0;
	for (int x = 0; x < array_cubesize; x++)
	{
		for (int y = 0; y < array_cubesize; y++)
		{
			for (int z = 0; z < array_cubesize; z++)
			{
				EnclaveKeyDef::EnclaveKey elementCollectionKey = lowerNWCornerKey;
				elementCollectionKey.x += x;
				elementCollectionKey.y += y;
				elementCollectionKey.z += z;

				// setup DCMD arrays
				DCMD_SubBufferContents[currentIndexValue] = 0;								// set this flag to 0 
				DCMD_SubBufferLocation[currentIndexValue] = currentIndexValue;				// set the sub buffer location to the current index value
				DCMD_RemainingVertexSpace[currentIndexValue] = (subBufferByteSize / 12);	// set the initial amount of vertexes remaining to the buffer size
				DCMD_TT1_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / 12);	// set the start location of this sub buffer's first TT1 vertex to be the currentIndexValue * (subBufferByteSize / 12 )
				DCMD_TT2_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / 12); // set the start location of this sub buffer's first TT2 vertex to be the currentIndexValue * (subBufferByteSize / 12 ) (will change later when TT2 data is inserted)

				// setup TT1 arrays
				TT1_SubBufferContents[currentIndexValue] = 0;					// 0 indicates that there is currently nothing in this buffer
				TT1_CollectionKeys[currentIndexValue] = elementCollectionKey;	// the collection key
				TT1_SubBufferLocation[currentIndexValue] = currentIndexValue;	// the index value to put into the Location
				TT1_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / 12);
				TT1_GL_VertexArraySize[currentIndexValue] = 0;



				currentIndexValue++;
			}
		}
	}
	std::cout << "last current index value is: " << currentIndexValue-1 << std::endl;
}