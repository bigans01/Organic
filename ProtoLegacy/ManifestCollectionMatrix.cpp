#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include "ManifestCollectionMatrix.h"
//#include <mutex>

ManifestCollectionMatrix::ManifestCollectionMatrix(EnclaveCollectionMatrix *collectionmatrixref)
{
	/* Summary: primary constructor which sets the value of CollectionMatrixRef */
	ECMCollectionMatrixRef = collectionmatrixref;
}

ManifestCollectionMatrix::ManifestCollectionMatrix()
{

}

void ManifestCollectionMatrix::SetCollectionMatrixRef(EnclaveCollectionMatrix *collectionmatrixref)
{
	ECMCollectionMatrixRef = collectionmatrixref;
}

void  ManifestCollectionMatrix::AddNewCollection(EnclaveKeyDef::EnclaveKey Key)
{
	/* Summary: adds a new collection that has a value of Key, and has a reference to CollectionMatrixRef */
	//ManifestCollection tempmanifestmatrix(ECMCollectionMatrixRef);
	//ManiCollectionMap[Key] = tempmanifestmatrix;
	//ManiCollectionMap.emplace(Key, ManifestCollection(ECMCollectionMatrixRef));
	//cout << "pre crashy-crash" << endl;
	ManiCollectionMap[Key].CollectionMatrixRef = ECMCollectionMatrixRef;
}

void ManifestCollectionMatrix::AttachManifestToCollectedEnclave(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z)
{
	/* Summary: attaches a manifest to an already collected enclave */
	//EnclaveKeyDef::EnclaveKey tempkey;					// this key is for the exact enclave contained within each collection, not the key of the collections itself
	ManiCollectionMap[CollectionKey].AddManifestToMatrix(target_x, target_y, target_z, CollectionKey);

}

void ManifestCollectionMatrix::AttachManifestToCollectedEnclave2(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z, ManifestCollection *ManifestCollectionRef)
{
	/* Summary: attaches a manifest to an already collected enclave */
	//EnclaveKeyDef::EnclaveKey tempkey;					// this key is for the exact enclave contained within each collection, not the key of the collections itself
	ManifestCollectionRef->AddManifestToMatrix(target_x, target_y, target_z, CollectionKey);

}

void ManifestCollectionMatrix::AttachManifestToCollectedEnclave2(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z, ManifestCollection *ManifestCollectionRef, int outputdebug)
{
	/* Summary: attaches a manifest to an already collected enclave */
	//EnclaveKeyDef::EnclaveKey tempkey;					// this key is for the exact enclave contained within each collection, not the key of the collections itself
	ManifestCollectionRef->AddManifestToMatrix(target_x, target_y, target_z, CollectionKey, outputdebug);

}


void ManifestCollectionMatrix::GetCollectedEnclaveManifestAt(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z)
{
	/* Summary: reserved for future use */
	//cout << "status of IsRenderCollectionRefSet: " << ManiCollectionMap[CollectionKey].GetManifestFromMatrix(target_x, target_y, target_z).IsRenderCollectionRefSet << endl;
}

void ManifestCollectionMatrix::UpdateAttachedManifest(EnclaveKeyDef::EnclaveKey CollectionKey, int target_x, int target_y, int target_z)
{
	/* Summary: updates a currently existing EnclaveManifest */
	ManiCollectionMap[CollectionKey].UpdateManifest(target_x, target_y, target_z, CollectionKey);
}


ManifestCollection& ManifestCollectionMatrix::GetRefManiCollectionMap(EnclaveKeyDef::EnclaveKey FindKey)
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<ManifestCollection>, EnclaveKeyDef::KeyHasher>::iterator temp_iter;
	temp_iter = ManiCollectionMapRef.find(FindKey);
	//ManifestCollection manicollPtr = temp_iter->second;
	//return std::ref(manicollPtr);
	return std::ref(temp_iter->second);
}
