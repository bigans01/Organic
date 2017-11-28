#pragma once

#ifndef ENCLAVECOLLECTIONSTATE_H
#define ENCLAVECOLLECTIONSTATE_H

#include "EnclaveCollection.h"
#include "EnclaveKeyDef.h"

class EnclaveCollectionStateArray;
class EnclaveBlockRayTracker;
class EnclaveCollectionState
{
private:
	friend class EnclaveCollectionStateArray;
	friend class EnclaveBlockRayTracker;
	EnclaveKeyDef::EnclaveKey ActualCollectionKey;
	int isActive = 0;
	EnclaveCollection* collectionPtr = NULL;
	EnclaveCollectionState();
};

#endif
