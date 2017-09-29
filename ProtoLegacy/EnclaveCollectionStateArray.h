#pragma once

#include "EnclaveCollectionState.h"
#include "EnclaveCollectionMatrix.h"
#include "EnclaveKeyDef.h"

#ifndef ENCLAVECOLLECTIONSTATEARRAY_H
#define ENCLAVECOLLECTIONSTATEARRAY_H

class EnclaveCollectionStateArray {
public:
	int cubesize = 1;								// determines how many collections in any direction there are around the central collection
	EnclaveCollectionState StateMatrix[3][3][3];	// stores up to 27 EnclaveCollectionStates
	void SetCenterCollection(EnclaveKeyDef::EnclaveKey centerKey, EnclaveCollectionMatrix* collectionMatrixPtr);
};

#endif
