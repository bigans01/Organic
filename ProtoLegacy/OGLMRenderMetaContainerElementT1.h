#pragma once

#ifndef OGLMRENDERMETACONTAINERELEMENTT1_H
#define OGLMRENDERMETACONTAINERELEMENTT1_H

#include "EnclaveKeyDef.h"

class OGLMBufferManager;
class OrganicSystem;
class OGLMRenderMetaContainerElementT1
{
private:
	friend class OGLMBufferManager;
	friend class OrganicSystem;
	EnclaveKeyDef::EnclaveKey ElementCollectionKey;					// the key of the collection stored in this element
	int ElementSingularXYZValue;									// the singular XYZ value of the buffer number that this element belongs to
	int ContainsUsedT1Key;											// determines if this element already contains a used collection key (indicating that the collection is actively being displayed in OpenGL)
};

#endif
