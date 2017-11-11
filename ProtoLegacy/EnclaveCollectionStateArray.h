#pragma once

#include "EnclaveCollectionState.h"
#include "EnclaveCollectionMatrix.h"
#include "EnclaveKeyDef.h"

#ifndef ENCLAVECOLLECTIONSTATEARRAY_H
#define ENCLAVECOLLECTIONSTATEARRAY_H

class EnclaveBlockRayTracker;
class OrganicSystem;
class EnclaveCollectionStateArray {
private:
	friend class EnclaveBlockRayTracker;
	friend class OrganicSystem;
	int cubesize = 1;								// determines how many collections in any direction there are around the central collection
	int isArraySet = 0;								// flag for determining if the arrays were set or not
	int centerCollectionStateOffset = 0;					// the offset needed to get to the center of the dynamicCollectionState array
	int centerXYZsingle = 0;						// the center of the dyanmic collection matrix, converted from x/y/z to a single value
	EnclaveCollectionState StateMatrix[3][3][3];	// stores up to 27 EnclaveCollectionStates
	EnclaveCollectionState* StateMatrixPtr;

	~EnclaveCollectionStateArray();

	void SetCenterCollection(EnclaveKeyDef::EnclaveKey centerKey, EnclaveCollectionMatrix* collectionMatrixPtr);
	void SetCenterCollectionDynamic(EnclaveKeyDef::EnclaveKey centerKey, EnclaveCollectionMatrix* collectionMatrixPtr);
	int translateXYZToSingle(int x, int y, int z);
	void CreateStateMatrix(int inCubesize);
	void ShiftCenterCollection(EnclaveKeyDef::EnclaveKey previousKey, EnclaveKeyDef::EnclaveKey currentKey, EnclaveCollectionMatrix* collectionMatrixPtr);
};

#endif
