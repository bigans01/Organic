/*------------------------------------------------------------------------------------------

--ManifestCollection.h	(Last Update 7/10/2017)

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
#include <functional>


class ManifestCollection {
public:
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<EnclaveManifest>, EnclaveKeyDef::KeyHasher> ManMatrix;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher> ManMatrix;						// unordered map which stores the EnclaveManifests
	//std::unordered_map<int, reference_wrapper<EnclaveManifest>> testmap;
	//std::unordered_map<int, reference_wrapper<EnclaveManifest>> testmap2;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<EnclaveManifest>, EnclaveKeyDef::KeyHasher> testmap2;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher>::iterator ManMatrixIter;		// an iterator for the above unordered map
	EnclaveMatrix *EnclaveMatrixRef;																						// a reference to the Enclave (currently unused, potentially reserverd for later)
	EnclaveCollectionMatrix *CollectionMatrixRef;																			// a reference to a valid instance of EnclaveCollectionMatrix
	reference_wrapper<EnclaveCollectionMatrix*> *WrappedCollectionMatrixRef;

	void AddManifestToMatrix(int x, int y, int z);																			// adds an EnclaveManifest with x/y/z of in the input parameters (unused)
	void AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key);											// adds an EnclaveManifest with x/y/z of in the input parameters; the EnclaveKey input parameter is used to 
																															// find the corresponding Enclave at x/y/z in the enclave collection having a key value of the input "Key"
	void UpdateManifest(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key);												// updates a currently existing enclave manifest (does not add a new instance to the ManifestCollection)
	//void GetManifestFromMatrix(int x, int y, int z);
	

	ManifestCollection(EnclaveMatrix *matrixref);																			// unused constructor; reserved for later use
	ManifestCollection(EnclaveMatrix *matrixref, EnclaveCollectionMatrix *collectionref);									// unused constructor; reserved for later use
	ManifestCollection(EnclaveCollectionMatrix *collectionref);																// currently used constructor;  takes in a valid EnclaveCollectionMatrix, to set the corresponding public member for this class
	EnclaveManifest& GetManifestFromMatrix(int x, int y, int z);															// returns a reference to the EnclaveManifest with the input x/y/z coordinates
	ManifestCollection();																									// default constructor
	~ManifestCollection();																									// destructor called for manual memory cleanup 
};

#endif

