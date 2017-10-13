#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "OGLMDrawCallMeta.h"

void OGLMDrawCallMeta::setGLintDynamicArrays(int in_cubeSize)
{
	//EnclaveKeyDef::EnclaveKey* DCM_CollectionKeys;
	//int* DCM_SubBufferLocation;
	DCM_CollectionKeys = new EnclaveKeyDef::EnclaveKey[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_SubBufferLocation = new int[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_GL_BufferOffset = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];
	DCM_GL_VertexArraySize = new GLint[in_cubeSize*in_cubeSize*in_cubeSize];
	areDynamicArraysSet = 1;
}

OGLMDrawCallMeta::~OGLMDrawCallMeta()
{
	if (areDynamicArraysSet == 1)
	{
		delete[] DCM_CollectionKeys;
		delete[] DCM_SubBufferLocation;
		delete[] DCM_GL_BufferOffset;
		delete[] DCM_GL_VertexArraySize;
	}
}