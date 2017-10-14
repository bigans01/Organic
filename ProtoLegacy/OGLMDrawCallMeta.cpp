#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "OGLMDrawCallMeta.h"

void OGLMDrawCallMeta::setGLintDynamicArrays(int in_cubeSize, int in_subBufferSize)
{
	//EnclaveKeyDef::EnclaveKey* DCM_CollectionKeys;
	//int* DCM_SubBufferLocation;
	DCM_SubBufferContents = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_CollectionKeys = new EnclaveKeyDef::EnclaveKey[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_SubBufferLocation = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_GL_BufferOffset = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_GL_VertexArraySize = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];
	array_cubesize = in_cubeSize;
	subBufferByteSize = in_subBufferSize;
	areDynamicArraysSet = 1;
}

OGLMDrawCallMeta::~OGLMDrawCallMeta()
{
	if (areDynamicArraysSet == 1)
	{
		delete[] DCM_SubBufferContents;
		delete[] DCM_CollectionKeys;
		delete[] DCM_SubBufferLocation;
		delete[] DCM_GL_BufferOffset;
		delete[] DCM_GL_VertexArraySize;
	}
}

void OGLMDrawCallMeta::addToListAndSort(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize)
{
	// search for an element in the list
	int doesElementExist = 0;
	for (int x = 0; x < numberOfRenderableCollections; x++)
	{

	}
	if (doesElementExist == 0)
	{
		DCM_CollectionKeys[numberOfRenderableCollections] = in_key;														// set the key, in the corresponding current element of dynamic array
		DCM_SubBufferLocation[numberOfRenderableCollections] = in_subBufferIndex;											// set the location, in the corresponding current element of dynamic array
		DCM_GL_BufferOffset[numberOfRenderableCollections] = in_subBufferIndex * (in_subBufferByteSize / 12);				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
		DCM_GL_VertexArraySize[numberOfRenderableCollections] = in_vertexArrayByteSize / 12;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array
		numberOfRenderableCollections++;		// increment the number of renderable collections
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
				DCM_CollectionKeys[currentIndexValue] = elementCollectionKey;
				DCM_SubBufferContents[currentIndexValue] = 0;		// 0 indicates that there is currently nothing in this buffer
				DCM_SubBufferLocation[currentIndexValue] = currentIndexValue;
				DCM_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / 12);
				DCM_GL_VertexArraySize[currentIndexValue] = 0;
				currentIndexValue++;
			}
		}
	}
	std::cout << "last current index value is: " << currentIndexValue-1 << std::endl;
}