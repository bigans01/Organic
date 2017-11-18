#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OGLMRenderMetaContainer.h"

void OGLMRenderMetaContainer::createContainerArrays(int T1_inCubesize, int T2_inCubesize)
{
	/* Summary: generates two dynamic arrays, one for T1 terrain and one for T2 terrain */
	T1_renderMetaContainerArray = new OGLMRenderMetaContainerElementT1[T1_inCubesize*T1_inCubesize*T1_inCubesize];	// T1 arrays
	T2_renderMetaContainerArray = new OGLMRenderMetaContainerElementT2[T2_inCubesize*T2_inCubesize*T2_inCubesize];	// T2 arrays

	isDynamicArrayCreated = 1;
}

OGLMRenderMetaContainer::~OGLMRenderMetaContainer()
{
	if (isDynamicArrayCreated == 1)
	{
		delete[] T1_renderMetaContainerArray;
		delete[] T2_renderMetaContainerArray;
	}
}
