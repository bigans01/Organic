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
	DCMD_SubBufferContents.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	DCMD_SubBufferLocation.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	DCMD_RemainingVertexSpace.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	DCMD_TT1_GL_BufferOffset.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	DCMD_TT2_GL_BufferOffset.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);

	//TT1 arrays
	TT1_SubBufferContents.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	TT1_CollectionKeys.reset(new EnclaveKeyDef::EnclaveKey[in_cubeSize*in_cubeSize*in_cubeSize]);
	TT1_SubBufferLocation.reset(new int[in_cubeSize*in_cubeSize*in_cubeSize]);
	TT1_GL_BufferOffset.reset(new GLint[in_cubeSize*in_cubeSize*in_cubeSize]);
	TT1_GL_VertexArraySize.reset(new GLint[in_cubeSize*in_cubeSize*in_cubeSize]);

	array_cubesize = in_cubeSize;
	array_length = in_cubeSize*in_cubeSize*in_cubeSize;
	subBufferByteSize = in_subBufferSize;
}

void OGLMDrawCallMeta::setVertexTupleWidth(int in_width)
{
	vertexTupleByteLength = in_width;
}

void OGLMDrawCallMeta::removeHighLODAndSort(EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	/*
	std::cout << "---------------------------------CURRENT CONTENTS-------------------------------" << std::endl;
	for (int xx = 0; xx < numberOfRenderableCollections; xx++)
	{
		std::cout << "Key: (" << xx << ") " << TT1_CollectionKeys[xx].x << ", " << TT1_CollectionKeys[xx].y << ", " << TT1_CollectionKeys[xx].z << ") | SubBuffer: " << TT1_SubBufferLocation[xx] << " | BufferOffset: " << TT1_GL_BufferOffset[xx] << " Nubmer of Vertexes: " << TT1_GL_VertexArraySize[xx] << std::endl;

	}

	std::cout << "---------------------------------BEGIN REMOVAL-------------------------------" << std::endl;
	*/
	//std::cout << "BEGIN number of renderable collections: " << numberOfRenderableCollections << std::endl;
	int lastElementIndex = (numberOfRenderableCollections - 1);
	for (int x = 0; x < numberOfRenderableCollections; x++)
	{
		if (TT1_CollectionKeys[x] == in_enclaveKey)
		{
			//if (TT1_SubBufferContents[x] == 1)
			//{
				std::cout << "Key found! removing: (" << x << ") "  << TT1_CollectionKeys[x].x << ", " << TT1_CollectionKeys[x].y << ", " << TT1_CollectionKeys[x].z << std::endl;

				int temp_TT1_SubBufferContents = TT1_SubBufferContents[x];
				EnclaveKeyDef::EnclaveKey temp_TT1_CollectionKey = TT1_CollectionKeys[x];
				int temp_TT1_SubBufferLocation = TT1_SubBufferLocation[x];
				int temp_TT1_GL_BufferOffset = TT1_GL_BufferOffset[x];
				int temp_TT1_GL_VertexArraySize = TT1_GL_VertexArraySize[x];

				TT1_SubBufferContents[x] = TT1_SubBufferContents[lastElementIndex]; 
				TT1_CollectionKeys[x] = TT1_CollectionKeys[lastElementIndex];
				TT1_SubBufferLocation[x] = TT1_SubBufferLocation[lastElementIndex];
				TT1_GL_BufferOffset[x] = TT1_GL_BufferOffset[lastElementIndex];
				TT1_GL_VertexArraySize[x] = TT1_GL_VertexArraySize[lastElementIndex];

				//TT1_SubBufferContents[lastElementIndex] = 0;
				//TT1_CollectionKeys[lastElementIndex].x = 0;
				//TT1_CollectionKeys[lastElementIndex].y = 0;
				//TT1_CollectionKeys[lastElementIndex].z = 0;
				//TT1_SubBufferLocation[lastElementIndex] = 0;

				//TT1_GL_VertexArraySize[lastElementIndex] = 0;

				TT1_SubBufferContents[lastElementIndex] = 0;		// temp_TT1_SubBufferContents
				TT1_CollectionKeys[lastElementIndex] = temp_TT1_CollectionKey;
				TT1_SubBufferLocation[lastElementIndex] = temp_TT1_SubBufferLocation;
				TT1_GL_BufferOffset[lastElementIndex] = temp_TT1_GL_BufferOffset;
				TT1_GL_VertexArraySize[lastElementIndex] = temp_TT1_GL_VertexArraySize;

				numberOfRenderableCollections--;	// deduct the number of renderable collections
				indexValueOfFirstEmpty--;			// deduct first empty value
				
				/*
				for (int xx = 0; xx < numberOfRenderableCollections; xx++)
				{
					std::cout << "Key: (" << xx << ") " << TT1_CollectionKeys[xx].x << ", " << TT1_CollectionKeys[xx].y << ", " << TT1_CollectionKeys[xx].z << ") | SubBuffer: " << TT1_SubBufferLocation[xx] << " | BufferOffset: " << TT1_GL_BufferOffset[xx] << " Nubmer of Vertexes: " << TT1_GL_VertexArraySize[xx] << std::endl;
					
				}
				std::cout << "Last element key is now: " << TT1_CollectionKeys[lastElementIndex].x << ", " << TT1_CollectionKeys[lastElementIndex].y << ", " << TT1_CollectionKeys[lastElementIndex].z << ", " << std::endl;
				std::cout << "-----------------------" << std::endl;
				*/
				break;
			//}

				

		}
	}
	//std::cout << "END number of renderable collections: " << numberOfRenderableCollections << std::endl;
}

void OGLMDrawCallMeta::addToListAndSort(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize)
{
	// search for an element in the list

	/*
	if (isPhaseInitial == 1)
	{
		std::cout << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>> now attempting add... Key being added is: (" << in_key.x << ", " << in_key.y << ", " << in_key.z << ") | Sub buffer is: " << in_subBufferIndex << std::endl;
		std::cout << "pre-add array contents: " << std::endl;
		for (int xx = 0; xx < numberOfRenderableCollections; xx++)
		{
			std::cout << "Key: (" << xx << ") " << TT1_CollectionKeys[xx].x << ", " << TT1_CollectionKeys[xx].y << ", " << TT1_CollectionKeys[xx].z << ") | SubBuffer: " << TT1_SubBufferLocation[xx] << " | BufferOffset: " << TT1_GL_BufferOffset[xx] << " Nubmer of Vertexes: " << TT1_GL_VertexArraySize[xx] << " | Buffer Contents: " << TT1_SubBufferContents[xx] << std::endl;

		}
		std::cout << std::endl;

		// TRY THIS: if f < indexValueofFirstEmpty

		// scan to see if its in the already renderable list
		for (int f = 0; f < array_length; f++)
		{
			if (in_subBufferIndex == TT1_SubBufferLocation[f])
			{
				if (f <= numberOfRenderableCollections)
				//if (f == indexValueOfFirstEmpty)
				{
					std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Sub buffer Key is within already rendered list!!!! ((3)) (" << f << ") " << " index of first empty is ( " << indexValueOfFirstEmpty << ") " << std::endl;
				}
			}
		}

	}
	*/


	int doesElementExist = 0;
	//std::cout << "test of sub buffer index is: " << in_subBufferIndex << std::endl;
	for (int x = 0; x < array_length; x++)
	{
		
		// find the corresponding location of the sub-buffer's ID (with a range of 0 to array_length)
		if (TT1_SubBufferLocation[x] == in_subBufferIndex )
		{

				if (TT1_SubBufferContents[x] == 0)
				{
					//std::cout << "sub buffer contents pass " << std::endl;
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
					TT1_GL_BufferOffset[indexValueOfFirstEmpty] = in_subBufferIndex * (in_subBufferByteSize / vertexTupleByteLength);				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
					TT1_GL_VertexArraySize[indexValueOfFirstEmpty] = in_vertexArrayByteSize / vertexTupleByteLength;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array

					// set the values at x
					if (indexValueOfFirstEmpty != x)		// <<<< THIS WAS FIX; don't do unnecessary REPLACE!
					{
						TT1_SubBufferContents[x] = temp_TT1_SubBufferContents;
						TT1_CollectionKeys[x] = temp_TT1_CollectionKey;														// set the key, in the corresponding current element of dynamic array
						TT1_SubBufferLocation[x] = temp_TT1_SubBufferLocation;											// set the location, in the corresponding current element of dynamic array
						TT1_GL_BufferOffset[x] = temp_TT1_GL_BufferOffset;				// set the vertex offset of the beginning of the sub buffer, in the corresponding current element of dynamic array
						TT1_GL_VertexArraySize[x] = temp_TT1_GL_VertexArraySize;		// set the number of vertexes in the sub buffer, in the corresponding current element of dynamic array
					}
					//std::cout << "index value of first empty is: " << indexValueOfFirstEmpty << std::endl;
					indexValueOfFirstEmpty++;
					numberOfRenderableCollections++;
					// std::cout << "Added to list: sub buffer: " << in_subBufferIndex << "actual offset: " << in_subBufferIndex * (in_subBufferByteSize / 12) << " vertex count: " << in_vertexArrayByteSize / 12 << std::endl;
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
			//}

		}
		
	}

}

void OGLMDrawCallMeta::sendTerrainT1AddRequestToDelegator(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize)
{
	for (int x = 0; x < array_length; x++)
	{
		if (DCMD_SubBufferLocation[x] == in_subBufferIndex)
		{
			// check to see if contents are empty
			if (DCMD_SubBufferContents[x] == 0)
			{
				//std::cout << "Remaining vertices: " << DCMD_RemainingVertexSpace[x] << std::endl;
				DCMD_RemainingVertexSpace[x] = DCMD_RemainingVertexSpace[x] - (in_vertexArrayByteSize / vertexTupleByteLength);	// subtract the vertexes being added to the array, from the total amount remaining and store the result
				DCMD_TT1_GL_BufferOffset[x] = in_subBufferIndex * (in_subBufferByteSize / vertexTupleByteLength);					// store the starting vertex of the TT1 data in the sub buffer
				//sendTerrainT1AddRequestToDelegator(in_key, in_subBufferIndex, in_vertexArrayByteSize, in_subBufferByteSize);
				addToListAndSort(in_key, in_subBufferIndex, in_vertexArrayByteSize, in_subBufferByteSize);
				DCMD_SubBufferContents[x] = 0;
				//std::cout << "Vertices subtracted: " << (in_vertexArrayByteSize / 12) << std::endl;
				//std::cout << "Remaining verties (2): " << DCMD_RemainingVertexSpace[x] << std::endl;
			}

			// needs new statement for when contents are not empty (i.e., already contains T1 data) (11/13/2017)
		}
	}
	// std::cout << "Delegator inserted values: element number: " << in_subBufferIndex << "   actual offset: " << in_subBufferIndex * (in_subBufferByteSize / 12) << std::endl;
}

void OGLMDrawCallMeta::sendTerrainT1RemoveRequestToDelegator(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex)
{

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
				DCMD_RemainingVertexSpace[currentIndexValue] = (subBufferByteSize / vertexTupleByteLength);	// set the initial amount of vertexes remaining to the buffer size
				DCMD_TT1_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / vertexTupleByteLength);	// set the start location of this sub buffer's first TT1 vertex to be the currentIndexValue * (subBufferByteSize / 12 )
				DCMD_TT2_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / vertexTupleByteLength); // set the start location of this sub buffer's first TT2 vertex to be the currentIndexValue * (subBufferByteSize / 12 ) (will change later when TT2 data is inserted)

				// setup TT1 arrays
				TT1_SubBufferContents[currentIndexValue] = 0;					// 0 indicates that there is currently nothing in this buffer
				TT1_CollectionKeys[currentIndexValue] = elementCollectionKey;	// the collection key
				TT1_SubBufferLocation[currentIndexValue] = currentIndexValue;	// the index value to put into the Location
				TT1_GL_BufferOffset[currentIndexValue] = currentIndexValue * (subBufferByteSize / vertexTupleByteLength);
				TT1_GL_VertexArraySize[currentIndexValue] = 0;



				currentIndexValue++;
			}
		}
	}
	std::cout << "last current index value is: " << currentIndexValue-1 << std::endl;
}