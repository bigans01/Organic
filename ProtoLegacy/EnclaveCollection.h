/*------------------------------------------------------------------------------------------

--EnclaveCollection.h		(Last update 9/15/2017)

Description: Header file for EnclaveCollectionMatrix.cpp.

Summary: An EnclaveCollection is a collection of 512 enclaves. It is responsible for maintaining meta-data about the collection as a whole; 
this includes but is not limited to: determining which 

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
	EnclaveKeyDef::EnclaveKey RenderableEnclaves[512];																		// stores all renderable enclave keys (up to 512)
	int totalRenderableEnclaves = 0;																						// the total number of Enclaves that are set for rendering in this Collection.
	void ActivateEnclaveForRendering(EnclaveKeyDef::EnclaveKey Key);														// activates an Enclave in this Collection for rendering, by putting the Enclave's unique key into RenderableEnclaves
	Enclave& GetEnclaveByKey(EnclaveKeyDef::EnclaveKey InKey);																// returns a direct reference to an Enclave in this collection
	void ResetTotalRenderableEnclaves();																					// resets the total number of renderable enclaves (designed for Factory usage)
	void SetWestBorder(ElevationMapRef standardMapCopy,  EnclaveCollectionActivateListT2& activateListRef);														// sets all standard chunks on the west border, if there is no neighbor to the west.
	void SetWestBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);		// sets all standard chunks on the west border, if there is a neighbor to the west.
	void SetNorthBorder(ElevationMapRef standardMapCopy, EnclaveCollectionActivateListT2& activateListRef);														// North (16)
	void SetNorthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);	// 
	void SetEastBorder(ElevationMapRef standardMapCopy,  EnclaveCollectionActivateListT2& activateListRef);														// East (8)
	void SetEastBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);		//
	void SetSouthBorder(ElevationMapRef standardMapCopy,  EnclaveCollectionActivateListT2& activateListRef);													// South (4)
	void SetSouthBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);	//
	void SetTopBorder(ElevationMapRef standardMapCopy,  EnclaveCollectionActivateListT2& activateListRef);														// Top (2)
	void SetTopBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);		// 
	void SetBottomBorder(ElevationMapRef standardMapCopy, EnclaveCollectionActivateListT2& activateListRef);													// Bottom (1)
	void SetBottomBorder(ElevationMapRef elevationMapCopy, EnclaveCollectionActivateListT2& activateListRef, EnclaveCollectionNeighborList* neighborListPtr);	// 
	void RunCollectionPainters(EnclaveCollectionBlueprint* blueprintRef);																						// Runs collection wide painters, based on the passed in blueprint value.

};
#endif