#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include "ManifestCollection.h"
#include "Enclave.h"
#include "EnclaveKeyDef.h"
#include "EnclaveMatrix.h"
#include "EnclaveManifest.h"
#include "OrganicSystem.h"


void ManifestCollection::AddManifestToMatrix(int x, int y, int z)
{
	/* Summary: adds a newly instanced manifest to the matrix. (UNUSED) */
	//cout << "test call" << endl;
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	EnclaveManifest tempmanifest(x, y, z);
	ManMatrix[tempkey] = tempmanifest;
	ManMatrix[tempkey].AttachToEnclave(EnclaveMatrixRef->GetEnclaveFromMatrix(x, y, z));

}

void ManifestCollection::AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key)
{
	/* Summary: adds a newly instanced manifest to the matrix; the manifest will correspond to the Enclave at x/y/z within the EnclaveCollection having a key value of Key */
	//cout << "test call" << endl;
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	OrganicTextureDictionary *tempDictionaryRef = &CollectionMatrixRef->OrganicPointer->TextureDictionary;
	OrganicVtxColorDictionary *tempColorDictionaryRef = &CollectionMatrixRef->OrganicPointer->VertexColorDictionary;
	EnclaveManifest tempmanifest(x, y, z, tempDictionaryRef, tempColorDictionaryRef, 1);		// last argument is NEW PROTOTYPE DUMB VALUE
	ManMatrix[tempkey] = tempmanifest;
	ManMatrix[tempkey].AttachToEnclave(CollectionMatrixRef->GetEnclaveFromCollection(Key, x, y, z));
	//cout << "test value of triangles:: (" << tempkey.x << ", " << tempkey.y << ", " << tempkey.z << ") " << ManMatrix[tempkey].TotalEnclaveTriangles << endl;
}

void ManifestCollection::AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key, int renderMode)
{
	/* Summary: adds a newly instanced manifest to the matrix; the manifest will correspond to the Enclave at x/y/z within the EnclaveCollection having a key value of Key */
	//cout << "test call" << endl;
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	OrganicTextureDictionary *tempDictionaryRef = &CollectionMatrixRef->OrganicPointer->TextureDictionary;
	OrganicVtxColorDictionary *tempColorDictionaryRef = &CollectionMatrixRef->OrganicPointer->VertexColorDictionary;
	EnclaveManifest tempmanifest(x, y, z, tempDictionaryRef, tempColorDictionaryRef, renderMode);
	ManMatrix[tempkey] = tempmanifest;
	ManMatrix[tempkey].AttachToEnclave(CollectionMatrixRef->GetEnclaveFromCollection(Key, x, y, z));
	//cout << "test call: " << outputdebug << endl;

	//cout << "test value of triangles:: (" << tempkey.x << ", " << tempkey.y << ", " << tempkey.z << ") " << ManMatrix[tempkey].TotalEnclaveTriangles << endl;
}

void ManifestCollection::AddManifestToMatrix(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key, int renderMode, mutex& HeapMutex)
{
	/* Summary: adds a newly instanced manifest to the matrix; the manifest will correspond to the Enclave at x/y/z within the EnclaveCollection having a key value of Key */
	//cout << "test call" << endl;
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	OrganicTextureDictionary *tempDictionaryRef = &CollectionMatrixRef->OrganicPointer->TextureDictionary;
	OrganicVtxColorDictionary *tempColorDictionaryRef = &CollectionMatrixRef->OrganicPointer->VertexColorDictionary;
	EnclaveManifest tempmanifest(x, y, z, tempDictionaryRef, tempColorDictionaryRef, renderMode);
	//cout << "|||| LOCK GUARD BEGIN " << endl;
	AllocateManifestViaLockGuard(tempkey, &tempmanifest, HeapMutex);
	//cout << "|||| LOCK GUARD END " << endl;
	ManMatrix[tempkey].AttachToEnclave(CollectionMatrixRef->GetEnclaveFromCollection(Key, x, y, z), HeapMutex);
	//cout << "|||| ATTACH TO ENCLAVE END " << endl;
	//cout << "test value of triangles:: (" << tempkey.x << ", " << tempkey.y << ", " << tempkey.z << ") " << ManMatrix[tempkey].TotalEnclaveTriangles << endl;
}

void ManifestCollection::AllocateManifestViaLockGuard(EnclaveKeyDef::EnclaveKey Key, EnclaveManifest* manifestPtr, mutex& HeapMutex)
{
	/* Summary: thread-safe memory allocation when adding to heap*/
	std::lock_guard<std::mutex> lock(HeapMutex);
	ManMatrix[Key] = *manifestPtr;
	//cout << "Manifest renderMode value is: " << ManMatrix[Key].manifestRenderMode << endl;
}

void ManifestCollection::UpdateManifest(int x, int y, int z, EnclaveKeyDef::EnclaveKey Key)
{
	/* Summary: updates a currently instanced manifest in the matrix. */
	//cout << "test call of update manifest..." << endl;
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	ManMatrix[tempkey].AttachToEnclave(CollectionMatrixRef->GetEnclaveFromCollection(Key, x, y, z));
}

ManifestCollection::ManifestCollection(EnclaveMatrix *matrixref)
{
	/* Summary: unused constructor */
	//cout << "Old manifest collection add 2 ..." << endl;
	EnclaveMatrixRef = matrixref;
}

ManifestCollection::ManifestCollection(EnclaveMatrix *matrixref, EnclaveCollectionMatrix *collectionref)
{
	/* Summary: unused constructor */
	//cout << "Old manifest collection add..." << endl;
	EnclaveMatrixRef = matrixref;
	CollectionMatrixRef = collectionref;
}

ManifestCollection::ManifestCollection(EnclaveCollectionMatrix *collectionref)
{
	/* Summary: currently used non-default constructor; sets the value for public member variable CollectionMatrixRef */
	//cout << "New manifest collection add...." << endl;
	CollectionMatrixRef = collectionref;
	EnclaveCollectionMatrix *tptr = std::ref(collectionref);
}

ManifestCollection::ManifestCollection()
{
	//cout << "Default manifest collection add..." << endl;
	/* Summary: default constructor */
}

ManifestCollection::~ManifestCollection()
{
	/* Summary: unused destructor; potentially used later. */
	//cout << "ManifestMatrix destructor call..." << endl;
	//int count = 0;
	//for (ManMatrixIter = ManMatrix.begin(); ManMatrixIter != ManMatrix.end(); ManMatrixIter++)				// speical case for destructor, set variable to 0 before delete
	//{
	//	ManMatrixIter->second.IsEnclaveGLPtrLoaded = 0;
		//cout << "test count: " << count++ << endl;
	//}

}
EnclaveManifest& ManifestCollection::GetManifestFromMatrix(int x, int y, int z)		// returns pointer to enclave manifest
{
	/* Summary: returns a pointer to an EnclaveManifest contained within this class' public member of ManMatrix. */

	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	//cout << "GetManifest entry..." << endl;
	return std::ref(ManMatrix[tempkey]);
}