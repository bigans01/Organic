#pragma once

#ifndef ENCLAVECOLLECTIONSTATE_H
#define ENCLAVECOLLECTIONSTATE_H

#include "EnclaveCollection.h"
#include "EnclaveKeyDef.h"

class EnclaveCollectionState
{
public:
	EnclaveKeyDef::EnclaveKey ActualCollectionKey;
	int isActive = 0;
	EnclaveCollection* collectionPtr;
	EnclaveCollectionState();
};

#endif
