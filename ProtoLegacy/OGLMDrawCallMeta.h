#pragma once

#ifndef OGLMDRAWCALLMETA_H
#define OGLMDRAWCALLMETA_H

#include "EnclaveKeyDef.h"
#include <GL/glew.h>

class OGLMDrawCallMeta
{
	public:
		EnclaveKeyDef::EnclaveKey* DCM_CollectionKeys;
		int* DCM_SubBufferContents;
		int* DCM_SubBufferLocation;
		GLint* DCM_GL_BufferOffset;
		GLint* DCM_GL_VertexArraySize;
		int areDynamicArraysSet = 0;
		int numberOfRenderableCollections = 0;
		int array_cubesize;
		int subBufferByteSize;

		~OGLMDrawCallMeta();

		void setGLintDynamicArrays(int in_cubeSize, int in_subBufferSize);
		void addToListAndSort(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize, int in_subBufferByteSize);
		void setInitialDynamicArrayData(EnclaveKeyDef::EnclaveKey centerCollectionKey);
};

#endif