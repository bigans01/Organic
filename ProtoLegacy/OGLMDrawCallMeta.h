#pragma once

#ifndef OGLMDRAWCALLMETA_H
#define OGLMDRAWCALLMETA_H

#include "EnclaveKeyDef.h"
#include <GL/glew.h>

class OGLMDrawCallMeta
{
	public:
		EnclaveKeyDef::EnclaveKey* DCM_CollectionKeys;
		int* DCM_SubBufferLocation;
		GLint* DCM_GL_BufferOffset;
		GLint* DCM_GL_VertexArraySize;
		int areDynamicArraysSet = 0;
		int numberOfRenderableCollections = 0;

		~OGLMDrawCallMeta();

		void setGLintDynamicArrays(int in_cubeSize);
};

#endif