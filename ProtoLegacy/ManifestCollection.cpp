#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include "ManifestCollection.h"
#include "Enclave.h"
#include "EnclaveKeyDef.h"
#include "EnclaveMatrix.h"
#include "EnclaveManifest.h"
#include "OrganicTextureDictionary.h"
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
	//EnclaveManifest *tptr;
	//tptr = &ManMatrix[tempkey];
	//ManMatrix.emplace(tempkey,EnclaveManifest (x, y, z));
	//cout << "test AddManifestTomatrix: " << EnclaveMatrixRef->GetEnclaveFromMatrix(x, y, z).GetTotalTrianglesInEnclave() << endl;
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
	EnclaveManifest tempmanifest(x, y, z, tempDictionaryRef);

	ManMatrix[tempkey] = tempmanifest;
	//EnclaveManifest *tptr;
	//tptr = &ManMatrix[tempkey];
	//ManMatrix.emplace(tempkey,EnclaveManifest (x, y, z));
	//cout << "test AddManifestTomatrix: " << EnclaveMatrixRef->GetEnclaveFromMatrix(x, y, z).GetTotalTrianglesInEnclave() << endl;
	ManMatrix[tempkey].AttachToEnclave(CollectionMatrixRef->GetEnclaveFromCollection(Key, x, y, z));
	//cout << "test call2 " << endl;
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
	EnclaveMatrixRef = matrixref;
}

ManifestCollection::ManifestCollection(EnclaveMatrix *matrixref, EnclaveCollectionMatrix *collectionref)
{
	/* Summary: unused constructor */
	EnclaveMatrixRef = matrixref;
	CollectionMatrixRef = collectionref;
}

ManifestCollection::ManifestCollection(EnclaveCollectionMatrix *collectionref)
{
	/* Summary: currently used non-default constructor; sets the value for public member variable CollectionMatrixRef */
	CollectionMatrixRef = collectionref;
}

ManifestCollection::ManifestCollection()
{
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
	return ManMatrix[tempkey];
}