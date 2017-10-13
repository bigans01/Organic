#pragma once

#ifndef OGLMRENDERMETACONTAINER_H
#define OGLMRENDERMETACONTAINER_H

#include "OGLMRenderMetaContainerElement.h"

class OGLMRenderMetaContainer
{
public:
	OGLMRenderMetaContainerElement* renderMetaContainerArray;
	int isDynamicArrayCreated = 0;								// determines if the current dynamic array is being used
	void createContainerArray(int inCubesize);
	~OGLMRenderMetaContainer();
};

#endif
