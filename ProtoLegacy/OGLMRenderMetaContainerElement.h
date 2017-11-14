#pragma once

#ifndef OGLMRENDERMETACONTAINERELEMENT_H
#define OGLMRENDERMETACONTAINERELEMENT_H

#include "EnclaveKeyDef.h"
#include "OrganicGLRenderCollectionMeta.h"

class OGLMBufferManager;
class OrganicGLManager;
class OGLMRenderMetaContainerElement
{
private:
	friend class OGLMBufferManager;
	friend class OrganicGLManager;
	EnclaveKeyDef::EnclaveKey ElementCollectionKey;					// the key of the collection stored in this element
	int ElementSingularXYZValue;									// the singular XYZ value of the buffer number that this element belongs to
	OrganicGLRenderCollectionMeta ElementRenderCollectionMeta;		// the render collection meta info for this element
	int ContainsT1Key;												// determines if this element already contains a collection key (indicating that its being used)
};

#endif


