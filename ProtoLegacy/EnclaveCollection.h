/*------------------------------------------------------------------------------------------

--EnclaveCollection.h		(Last update 7/20/2017)

Description: Header file for EnclaveCollectionMatrix.cpp.

Summary: An EnclaveCollection is a collection of 512 enclaves.

Dependents: none.


------------------------------------------------------------------------------------------*/

#pragma once
#include "Enclave.h"
#include "EnclaveCollectionActivateListT2.h"
#include "EnclaveCollectionBlueprint.h"
#include "EnclaveCollectionNeighborList.h"
#include <mutex>

// activate guards
#ifndef ENCLAVECOLLECTION_H
#define ENCLAVECOLLECTION_H
class EnclaveCollection {
public:
	Enclave EnclaveArray[8][8][8];																							// 8 x 8 x 8 chunks 
	typedef unsigned char(&ElevationMapRef)[8][8];
	unsigned char ElevationMap[8][8] = { 128, 128 };																		// this value is a 64-byte array, that determines which enclaves (on the y-axis) 																														// are the "surface" enclaves (i.e., those exposed to air)
	EnclaveKeyDef::EnclaveKey RenderableEnclaves[512];
	int totalRenderableEnclaves = 0;
	void ActivateEnclaveForRendering(EnclaveKeyDef::EnclaveKey Key);
	Enclave& GetEnclaveByKey(EnclaveKeyDef::EnclaveKey InKey);
	void SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef);
	void SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef);
	void SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef);
	void SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef);
	void SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef);
	void SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2 &activateListRef);
	void SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);
	void RunCollectionPainters(EnclaveCollectionBlueprint* blueprintRef);

};
#endif