#pragma once

#ifndef OGLMRENDERMETACONTAINERELEMENT_H
#define OGLMRENDERMETACONTAINERELEMENT_H

#include "EnclaveKeyDef.h"
#include "OrganicGLRenderCollectionMeta.h"

class OGLMRenderMetaContainerElement
{
public:
	EnclaveKeyDef::EnclaveKey ElementCollectionKey;					// the key of the collection stored in this element
	int ElementSingularXYZValue;									// the singular XYZ value of the buffer number that this element belongs to
	OrganicGLRenderCollectionMeta ElementRenderCollectionMeta;		// the render collection meta info for this element
};

#endif


