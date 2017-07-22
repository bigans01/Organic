/*------------------------------------------------------------------------------------------

--EnclaveCollectionMatrix.h		(Last update 7/5/2017)

Description: Header file for EnclaveCollectionMatrix.cpp.

Summary: An EnclaveCollectionMatrix is an unordered map that consists of one or more EnclaveCollections. Each EnclaveCollection consists of 512 separate individual enclaves. The EnclaveKey key value in the unordered map
describes the x/y/z location of the collection. For instance, the 9th chunk on the positive x-axis would be found in the collection with a key of 1,0,0 (Chunks 0 through 7 would begin at 0,0,0, 1 through 15 at 1,0,0, and 16 through 23 at 2,0,0, etc). 
Although the number of x/y/z chunks is n cubed, where n is a multiple of 2, this may change in the future. The current value of n is 8. 

Dependents: none.


------------------------------------------------------------------------------------------*/

#pragma once



// activate guards
#ifndef ENCLAVECOLLECTIONMATRIX_H
#define ENCLAVECOLLECTIONMATRIX_H

#include "Enclave.h"
#include "EnclaveKeyDef.h"
#include "EnclaveCollection.h"
#include "PathTraceContainer.h"
#include "EnclaveCollectionBlueprint.h"
#include <unordered_map>

class EnclaveCollectionMatrix {
public:
	/*struct EnclaveCollection {																									// struct for creating groups of 512 chunks (512 chunks = 1 collection)
		Enclave EnclaveArray[8][8][8];																							// 8 x 8 x 8 chunks 
		unsigned char ElevationMap[8][8] = { 128, 128 };																						// this value is a 64-byte array, that determines which enclaves (on the y-axis) 
																																				// are the "surface" enclaves (i.e., those exposed to air)
		EnclaveKeyDef::EnclaveKey RenderableEnclaves[512];																		// stores a sorted list of enclaves that should be rendered

	};*/
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher> EnclaveCollectionMap;			// unordered map which stores the collections
	void AddNewCollection(int x, int y, int z);																					// adds a new collection, with the members of its key value being equivalent to x/y/z
	void AddNewCollection(EnclaveKeyDef::EnclaveKey Key);																		// adds a new collection, with the given key value.
	void AddNewCollectionWithBlueprint(EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint);
	Enclave& GetEnclaveFromCollection(EnclaveKeyDef::EnclaveKey Key, int x, int y, int z);										// returns a reference to the enclave located at the x/y/z coordinate within the collection that has a key of value Key
	Enclave& GetEnclaveFromXYZ(int x, int y, int z);																			// returns the enlave located at the absolute value of x/y/z
	void TracePathToBlock(int x, int y, int z);																					// testing purposes -- will output the coordinates taken to find a particualr block (i.e., prints x of the collection, then x of the chunk, then x of the block)
	typedef unsigned char(&ElevationMapRef)[8][8];																						// typedef for returning an array of 8x8 chars
	ElevationMapRef& GetElevationMapFromCollection(EnclaveKeyDef::EnclaveKey InKey);
	PathTraceContainer GetCoordTrace(int value);																				// returns a PathTraceContainer that contains the x, y, or z value of the collection, chunk, and individual block that would be returned when finding the coordinate. (would be called 3 times
																																// to get a true x/y/z coordinate.
	int KeyToSingle(EnclaveKeyDef::EnclaveKey InKey);

};
#endif