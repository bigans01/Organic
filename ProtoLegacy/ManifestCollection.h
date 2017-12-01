/*------------------------------------------------------------------------------------------

--ManifestCollection.h	(Last Update 8/16/2017)

Description: Header file for ManifestCollection cpp.

Summary: A ManifestCollection is a group of up to 512 enclave manifests. The ManifestCollection contains methods pertaining to the addition/removal of its contained manifests. 

Dependents: a valid instance of ManifestCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once


// activate guards
#ifndef MANIFESTCOLLECTION_H
#define MANIFESTCOLLECTION_H
#include "EnclaveMatrix.h"
#include "EnclaveManifest.h"
#include "EnclaveCollectionMatrix.h"
#include "OrganicTextureDictionary.h"
#include "OrganicVtxColorDictionary.h"
#include <functional>
#include <mutex>
#include "dummyManifest.h"

class OrganicSystem;
class ManifestCollectionMatrix;
class EnclaveCollectionMesh;
class RenderCollection;
class ManifestCollection {
public:
	ManifestCollection();																									// default constructor
	~ManifestCollection();																									// destructor called for manual memory cleanup 
private:
	friend class OrganicSystem;
	friend class ManifestCollectionMatrix;
	friend class EnclaveCollectionMesh;
	friend class RenderCollection;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher> ManMatrix;						// unordered map which stores the EnclaveManifests
	std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<EnclaveManifest>, EnclaveKeyDef::KeyHasher> testmap2;
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, dummyManifest, EnclaveKeyDef::KeyHasher> dummyMatrix;
	std::vector<dummyManifest> dummyMatrix;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher>::iterator ManMatrixIter;		// an iterator for the above unordered map
	EnclaveMatrix *EnclaveMatrixRef = NULL;																						// a reference to the Enclave (currently unused, potentially reserverd for later)
	EnclaveCollectionMatrix *CollectionMatrixRef = NULL;																			// a reference to a valid instance of EnclaveCollectionMatrix
	int insertdummy = 0;
	ManifestCollection(EnclaveMatrix *matrixref);																			// unused constructor; reserved for later use
	ManifestCollection(EnclaveMatrix *matrixref, EnclaveCollectionMatrix *collectionref);									// unused constructor; reserved for later use
	ManifestCollection(EnclaveCollectionMatrix *collectionref);																// currently used constructor;  takes in a valid EnclaveCollectionMatrix, to set the corresponding public member for this class


	void AddManifestToMatrix(int x, int y, int z);																			// adds an EnclaveManifest with x/y/z of in the input parameters (unused)
	void AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key);											// adds an EnclaveManifest with x/y/z of in the input parameters; the EnclaveKey input parameter is used to 
	void AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key, int outputdebug);							// debug purposes only.
	void AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key, int outputdebug, mutex& HeapMutex);		// calls a thread-safe version of AddManifestMatrix (thread-safe being only one thread will use heap at a time)																														// find the corresponding Enclave at x/y/z in the enclave collection having a key value of the input "Key"
	void UpdateManifest(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key);												// updates a currently existing enclave manifest (does not add a new instance to the ManifestCollection)
	EnclaveManifest& GetManifestFromMatrix(int x, int y, int z);															// returns a reference to the EnclaveManifest with the input x/y/z coordinates
	
};

#endif

