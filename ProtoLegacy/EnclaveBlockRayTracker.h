/*------------------------------------------------------------------------------------------

--EnclaveBlockRayTracker.h		(Last update 8/14/2017)

Description: Header file for EnclaveBlockRayTracker.cpp

Summary: An EnclaveBlockRayTracker keeps track of a ray as it traverses throguh a collection. It's job is to return the first block it finds that is renderable.


------------------------------------------------------------------------------------------*/


#pragma once

#ifndef ENCLAVEBLOCKRAYTRACKER_H
#define ENCLAVEBLOCKRAYTRACKER_H

#include "EnclaveCollection.h"
#include "EnclaveKeyDef.h"
#include "CursorPathTraceContainer.h"
#include "EnclaveCollectionState.h"
#include "EnclaveCollectionStateArray.h"
#include "OrganicBlockTarget.h"
#include "FloatTupleXYZ.h"
#include "OrganicBlockTarget.h"

class EnclaveBlockRayTracker
{
public:
	EnclaveCollection* currentCollectionPtr;	// pointer to the current collection that the ray is traversing across; the ray can potentially traverse across multiple collections
	Enclave* enclavePtr;
	EnclaveCollectionState* currentCollectionState;
	EnclaveCollectionStateArray* currentCollectionStateArray;
	OrganicBlockTarget* targetVertexData;
	int isCurrentCollectionActive = 0;
	EnclaveKeyDef::EnclaveKey collectionKey;	// actual key of the current collection being traversed by the ray
	EnclaveKeyDef::EnclaveKey enclaveKey;		// the current enclave the ray is traversing
	EnclaveKeyDef::EnclaveKey blockKey;			// the current block the ray is traversing
	EnclaveKeyDef::EnclaveKey arrayKey;

	FloatTupleXYZ EnclaveXYZOffset;	// EnclaveXYZOffset: a tuple of x/y/z values that determines the block's offset from the beginning of the Enclave's 0,0 point.

	EnclaveBlockRayTracker(CursorPathTraceContainer xContainer, CursorPathTraceContainer yContainer, CursorPathTraceContainer zContainer, EnclaveCollectionState* collectionStatePtr, EnclaveCollectionStateArray* collectionStateArrayPtr, int centerIndex, OrganicBlockTarget* inTargetPtr);

	int MoveEast();			// moves the tracker one block to the east
	int MoveWest();			// moves the tracker one block to the west
	int MoveNorth();		// moves the tracker one block to the north
	int MoveSouth();		// moves the tracker one block to the south
	int MoveAbove();		// moves the tracker one block above
	int MoveBelow();		// moves the tracker one block below
	void fillBlockTargetVertexData(int indexValue);
	void printOutTargetBlockHighlightData();			// testing only, used to output the x/y/z points of the highlight block
	FloatTupleXYZ SingleToMulti(int input);

};

#endif

