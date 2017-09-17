/*------------------------------------------------------------------------------------------

--ManifestCollectionMatrix.h (Last update 9/15/2017)

Description: Header file for ManifestCollectionMatrix.cpp

Summary: A ManifestCollectionMatrix is a class object that contains an unordered map, which stores various ManifestCollections. It is responsible for the addition/removal of the  ManifestCollections within the
ManiCollectionMap public member variable. 

Dependents: a valid instance of a EnclaveCollectionMatrix



------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef MANIFESTCOLLECTIONMATRIX_H
#define MANIFESTCOLLECTIONMATRIX_H
#include "EnclaveCollectionMatrix.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <functional>
#include "EnclaveKeyDef.h"
#include "ManifestCollection.h"
//#include <mutex>

class ManifestCollectionMatrix
{
public:
	std::unordered_map<EnclaveKeyDef::EnclaveKey, ManifestCollection, EnclaveKeyDef::KeyHasher> ManiCollectionMap;				// un unordered map which stores the ManifestCollections
	std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<ManifestCollection>, EnclaveKeyDef::KeyHasher> ManiCollectionMapRef;
	EnclaveCollectionMatrix *CollectionMatrixRef;																				// a reference to a valid EnclaveCollectionMatrix

	ManifestCollectionMatrix(EnclaveCollectionMatrix *collectionmatrixref);														// constructor which takes in a valid reference to an EnclaveCollectionMatrix
	ManifestCollectionMatrix();																									// constructor used when creating this object in an OrganicSystem.

	void SetCollectionMatrixRef(EnclaveCollectionMatrix *collectionmatrixref);
	void AddNewCollection(EnclaveKeyDef::EnclaveKey Key);																		// adds a new ManifestCollection into ManiCollectionMap, the key in the map will be value of Key
	void AttachManifestToCollectedEnclave(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z);	// attaches a manifest to a collected enclave that is contained within the Enclave collection that has a value of CollectionKey
	void AttachManifestToCollectedEnclave2(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z, ManifestCollection *ManifestCollectionRef);  // testing  only
	void AttachManifestToCollectedEnclave2(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z, ManifestCollection *ManifestCollectionRef, int outputdebug);  // testing  only
	void GetCollectedEnclaveManifestAt(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z);		// reserved for future use (unused)
	void UpdateAttachedManifest(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z);				// updates a specific manifest at x/y/z, which is contained within the manifest collection CollectionKey
	ManifestCollection& GetRefManiCollectionMap(EnclaveKeyDef::EnclaveKey FindKey);												// potentially unused (9/4/2017)

};

#endif