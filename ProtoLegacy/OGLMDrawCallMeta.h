#pragma once

#ifndef OGLMDRAWCALLMETA_H
#define OGLMDRAWCALLMETA_H

#include "EnclaveKeyDef.h"
#include <GL/glew.h>

class OrganicGLManager;
class OrganicSystem;
class OGLMDrawCallMeta
{
	private:
		friend class OrganicGLManager;
		friend class OrganicSystem;
		// DCM delegator (DCMD) array pointers
		int* DCMD_SubBufferContents = NULL;
		int* DCMD_SubBufferLocation = NULL;
		int* DCMD_RemainingVertexSpace = NULL;
		int* DCMD_TT1_GL_BufferOffset = NULL;
		int* DCMD_TT2_GL_BufferOffset = NULL;

		// DCM primary terrain type 1 (TT1) array pointers
		EnclaveKeyDef::EnclaveKey* TT1_CollectionKeys = NULL;
		int* TT1_SubBufferContents = NULL;
		int* TT1_SubBufferLocation = NULL;
		GLint* TT1_GL_BufferOffset = NULL;
		GLint* TT1_GL_VertexArraySize = NULL;
		int indexValueOfFirstEmpty = 0;				// indicates which index in the arrays contains the first available empty buffer


		
		int numberOfRenderableCollections = 0;


		int array_cubesize;
		int array_length;
		int subBufferByteSize;
		int areDynamicArraysSet = 0;

		~OGLMDrawCallMeta();

		void setGLintDynamicArrays(int in_cubeSize, int in_subBufferSize);
		void addToListAndSort(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize);
		void sendTerrainT1RequestToDelegator(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize);
		void setInitialDynamicArrayData(EnclaveKeyDef::EnclaveKey centerCollectionKey);
};

#endif