#pragma once

#ifndef OGLMRENDERMETACONTAINER_H
#define OGLMRENDERMETACONTAINER_H

#include "OGLMRenderMetaContainerElement.h"

class OrganicGLManager;
class OrganicSystem;
class OrganicBufferManager;
class OGLMBufferManager;
class OGLMRenderMetaContainer
{
public:
	~OGLMRenderMetaContainer();
private:
	friend class OrganicGLManager;
	friend class OrganicSystem;
	friend class OrganicBufferManager;
	friend class OGLMBufferManager;
	OGLMRenderMetaContainerElement* renderMetaContainerArray;
	int isDynamicArrayCreated = 0;								// determines if the current dynamic array is being used
	void createContainerArrays(int T1_inCubesize, int T2_cubesize);	// creates 

};

#endif
