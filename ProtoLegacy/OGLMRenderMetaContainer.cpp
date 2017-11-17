#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OGLMRenderMetaContainer.h"

void OGLMRenderMetaContainer::createContainerArrays(int T1_inCubesize, int T2_inCubesize)
{
	/* Summary: generates two dynamic arrays, one for T1 terrain and one for T2 terrain */
																												
	T2_renderMetaContainerArray = new OGLMRenderMetaContainerElement[T2_inCubesize*T2_inCubesize*T2_inCubesize];	// T2 arrays

	isDynamicArrayCreated = 1;
}

OGLMRenderMetaContainer::~OGLMRenderMetaContainer()
{
	if (isDynamicArrayCreated == 1)
	{
		delete[] T2_renderMetaContainerArray;
	}
}
