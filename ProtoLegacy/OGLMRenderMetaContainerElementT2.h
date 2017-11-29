#pragma once

#ifndef OGLMRENDERMETACONTAINERELEMENTT2_H
#define OGLMRENDERMETACONTAINERELEMENTT2_H

#include "EnclaveKeyDef.h"
#include "OrganicGLRenderCollectionMeta.h"

class OGLMBufferManager;
class OrganicGLManager;
class OrganicSystem;
class OGLMRenderMetaContainerElementT2
{
private:
	friend class OGLMBufferManager;
	friend class OrganicGLManager;
	friend class OrganicSystem;
	EnclaveKeyDef::EnclaveKey ElementCollectionKey;					// the key of the collection stored in this element
	int ElementSingularXYZValue = 0;									// the singular XYZ value of the buffer number that this element belongs to
	OrganicGLRenderCollectionMeta ElementRenderCollectionMeta;		// the render collection meta info for this element
	int ContainsUsedT2Key = 0;												// determines if this element already contains a used collection key (indicating that the collection is actively being displayed in OpenGL)
};

#endif


