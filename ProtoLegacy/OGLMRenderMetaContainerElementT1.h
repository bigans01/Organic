#pragma once

#ifndef OGLMRENDERMETACONTAINERELEMENTT1_H
#define OGLMRENDERMETACONTAINERELEMENTT1_H

#include "EnclaveKeyDef.h"

class OGLMBufferManager;
class OGLMRenderMetaContainerElementT1
{
private:
	friend class OGLMBufferManager;
	EnclaveKeyDef::EnclaveKey ElementCollectionKey;					// the key of the collection stored in this element
	int ElementSingularXYZValue;									// the singular XYZ value of the buffer number that this element belongs to
};

#endif
