#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OGLMRenderMetaContainer.h"

void OGLMRenderMetaContainer::createContainerArray(int inCubesize)
{
	renderMetaContainerArray = new OGLMRenderMetaContainerElement[inCubesize*inCubesize*inCubesize];

	isDynamicArrayCreated = 1;
}

OGLMRenderMetaContainer::~OGLMRenderMetaContainer()
{
	if (isDynamicArrayCreated == 1)
	{
		delete[] renderMetaContainerArray;
	}
}
