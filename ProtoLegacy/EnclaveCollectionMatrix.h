/*------------------------------------------------------------------------------------------

--EnclaveCollectionMatrix.h		(Last update 8/16/2017)

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
#include "CursorPathTraceContainer.h"
#include "EnclaveCollectionBlueprint.h"
#include "EnclaveCollectionBlueprintMatrix.h"
#include "EnclaveCollectionActivateListT2.h"
#include "EnclaveCollectionBorderFlags.h"
#include "ECBCollectionPainter.h"
#include "TestList.h"
#include <unordered_map>
#include <mutex>

class OrganicSystem;
class RenderCollectionMatrix;
class EnclaveCollectionStateArray;
class ManifestCollection;
class EnclaveCollectionMatrix {
private:
	friend class OrganicSystem;
	friend class RenderCollectionMatrix;
	friend class EnclaveCollectionStateArray;
	friend class ManifestCollection;
	typedef unsigned char(&ElevationMapRef)[8][8];																					// typedef for returning an array of 8x8 chars
	OrganicSystem *OrganicPointer = NULL;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher> EnclaveCollectionMap;				// unordered map which stores the collections

	void AddNewCollection(int x, int y, int z);																						// adds a new collection, with the members of its key value being equivalent to x/y/z
	void AddNewCollection(EnclaveKeyDef::EnclaveKey Key);																			// adds a new collection, with the given key value.
	void AddNewCollectionSkeleton(EnclaveKeyDef::EnclaveKey Key);
	void AddNewCollectionWithBlueprint(EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint);						// single-threaded call for adding a collection with a blueprint.
	void MultiAddNewCollectionWithBlueprint(int numThreads, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprintMatrix* blueprintmatrixptr, EnclaveCollectionBlueprint* blueprint);	// function that will prepare several worker threads for enclave instantiation.

	void JobInstantiateAndPopulateEnclave(	int beginRange,																			// (potentially unused) this function is designed to be used with multi-threaded calls. See definition for more details.
											int endRange,																			
											EnclaveCollection &collectionRef, 
											EnclaveKeyDef::EnclaveKey Key, 
											EnclaveCollectionBlueprint *blueprint);

	void JobInstantiateAndPopulateEnclaveBeta(	int beginRange,																		// (potentially unused) this function is designed for multithreading; it will return a list of enclaves that need to be rendered as a result
																		int endRange,												// of this job being run.			
																		EnclaveCollection &collectionRef,
																		EnclaveKeyDef::EnclaveKey Key,
																		EnclaveCollectionBlueprint *blueprint);
	void JobInstantiateAndPopulateEnclaveAlpha(	int beginRange,																		// this function is designed for multithreading; it will return a list of enclaves that need to be rendered as a result
												int endRange,																		// of this job being run. It is the defacto job to call when dealing with populating/instantiating Enclaves	
												EnclaveCollection &collectionRef,													// as part of a multithreaded operation.
												EnclaveKeyDef::EnclaveKey Key,
												EnclaveCollectionBlueprint* blueprint,
												EnclaveCollectionActivateListT2 &activateListRef);

	void JobInstantiateAndPopulateEnclaveAlpha2(int beginRange,																		// this function is designed for multithreading; it will return a list of enclaves that need to be rendered as a result
		int endRange,																												// of this job being run. It is the defacto job to call when dealing with populating/instantiating Enclaves	
		EnclaveCollection &collectionRef,																							// as part of a multithreaded operation.
		EnclaveKeyDef::EnclaveKey Key,
		EnclaveCollectionBlueprint* blueprint,
		EnclaveCollectionBlueprintMatrix* blueprintmatrix,
		EnclaveCollectionActivateListT2& activateListRef,
		mutex& HeapMutex);



	Enclave& GetEnclaveFromCollection(EnclaveKeyDef::EnclaveKey Key, int x, int y, int z);											// returns a reference to the enclave located at the x/y/z coordinate within the collection that has a key of value Key
	Enclave& GetEnclaveFromXYZ(int x, int y, int z);																				// returns the enlave located at the absolute value of x/y/z
	void TracePathToBlock(int x, int y, int z);																						// testing purposes -- will output the coordinates taken to find a particualr block (i.e., prints x of the collection, then x of the chunk, then x of the block)

	ElevationMapRef& GetElevationMapFromCollection(EnclaveKeyDef::EnclaveKey InKey);												// returns a pointer to an ElevationMap corresponding to a particular EnclaveCollection
	PathTraceContainer GetCoordTrace(float x);																					// returns a PathTraceContainer that contains the x, y, or z value of the collection, chunk, and individual block that would be returned when finding the coordinate. (would be called 3 times																																// to get a true x/y/z coordinate.
	CursorPathTraceContainer GetCursorCoordTrace(float x);
	int KeyToSingle(EnclaveKeyDef::EnclaveKey InKey);																				// returns an integer representing the value of an EnclaveKey transformed to a single value (i.e., 7,7,7 = 512 -- the 8th enclave on x,y, and z)
	void SetOrganicSystem(OrganicSystem *organicRef);																				// sets the OrganicSystem that this instance of EnclaveCollectionMatrix points to



};
#endif