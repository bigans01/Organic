/*------------------------------------------------------------------------------------------


--OGLMRenderMetaContainer.h		(Last update 11/17/2017)

Description: Header file for OGLMRenderMetaContainer.h

Summary: An OGLMRenderMetaContainer is a class that contains dynamic arrays for T1 and T2 terrain data, which is used by an instance of OGLMBufferManager. It does not manage its own arrays (the instance of OGLMBufferManager does this), 
but it does instantiate them and properly delete them in the destructor. 

Dependents: -a valid instance of an OGLMBufferManager

Notes:	this will very likely be modified continuously throughout development.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef OGLMRENDERMETACONTAINER_H
#define OGLMRENDERMETACONTAINER_H

#include "OGLMRenderMetaContainerElementT1.h"
#include "OGLMRenderMetaContainerElementT2.h"
#include <memory>

class OrganicGLManager;
class OrganicSystem;
class OrganicBufferManager;
class OGLMBufferManager;
class OGLMRenderMetaContainer
{
public:
	~OGLMRenderMetaContainer();			// destructor; required for deleting dynamically created arrays
private:
	friend class OrganicGLManager;		// friend declarations
	friend class OrganicSystem;
	friend class OrganicBufferManager;
	friend class OGLMBufferManager;
	//OGLMRenderMetaContainerElementT1* T1_renderMetaContainerArray;
	//OGLMRenderMetaContainerElementT2* T2_renderMetaContainerArray;	// a pointer to the T2 dynamic array
	std::unique_ptr<OGLMRenderMetaContainerElementT1[]> T1_renderMetaContainerArray;
	std::unique_ptr<OGLMRenderMetaContainerElementT2[]> T2_renderMetaContainerArray;

	//int isDynamicArrayCreated = 0;									// determines if the current dynamic array is being used
	void createContainerArrays(int T1_inCubesize, int T2_cubesize);	// creates the dynamic arrays for T1 and T2 

};

#endif
