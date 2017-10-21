/*------------------------------------------------------------------------------------------

--OrganicMorphMeta.h	(Last Update 10/21/2017)

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
	EnclaveKeyDef::EnclaveKey collectionKey;
};

#endif