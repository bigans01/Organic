/*------------------------------------------------------------------------------------------

--OrganicMorphMeta.h	(Last Update 11/12/2017)

Description: Header file for OrganicMorphMeta.h

Summary: contains a sub buffer index, and a collection key attached to it -- this is put into the CollectionProcessingQueue

Dependents: a valid instance of ManifestCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICMORPHMETA_H
#define ORGANICMORPHMETA_H

#include "EnclaveKeyDef.h"

class OrganicMorphMeta
{
public:
	int subBufferIndex;
	EnclaveKeyDef::EnclaveKey collectionKey;		// the collection key to be inserted
	int containsPreviousKey = 0;					// determines if there was a previous key in the sub buffer
	EnclaveKeyDef::EnclaveKey oldCollectionKey;		// the previous collection key (determined by OGLMBufferManager, during a call to MorphTerrainBuffers)
};

#endif