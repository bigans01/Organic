#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "RenderCollectionMatrix.h"
#include "RenderCollection.h"
#include <chrono>

/*void RenderCollectionMatrix::SetManifestCollectionMatrix(ManifestCollectionMatrix *manifestcollectionmatrixref)
{
	ManifestCollectionMatrixPtr = manifestcollectionmatrixref;
}*/
mutex cmutex;
void RenderCollectionMatrix::CreateRenderArrayFromManifestCollection(EnclaveKeyDef::EnclaveKey Key)
{
	/* Summary: creates a new 3d array for the RenderCollection having a key of Key. */
	//cmutex.lock();
	// NOTE: when adding a class to map, need 2 constructors!! (see 3 lines below)
	//int x = 0;
	//RenderCollection tempRenderCollection(x);																// create a render collection that references the proper manifest collection  :: &ManifestCollectionMatrixPtr->ManiCollectionMap[Key]
	//RenderMatrix[Key] = tempRenderCollection;															// set map to the temp value

	//auto start3 = std::chrono::high_resolution_clock::now();
	RenderMatrix[Key].SetManifestCollectionPtr(&ManifestCollectionMatrixPtr->ManiCollectionMap[Key]);	// set the pointer in the temp value
	RenderMatrix[Key].SetEnclaveCollectionPtr(&EnclaveCollectionMatrixPtr->EnclaveCollectionMap[Key]);
	//auto finish3 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed3 = finish3 - start3;
	//cout << "---------------create render array from manifest call time: " << elapsed3.count() << endl;

	RenderMatrix[Key].CombineManifestArrays();															// combine arrays
	//cmutex.unlock();
	
}

void RenderCollectionMatrix::CreateRenderArrayFromManifestCollection(EnclaveKeyDef::EnclaveKey Key, mutex& mutexval)
{
	RenderMatrix[Key].SetManifestCollectionPtr(&ManifestCollectionMatrixPtr->ManiCollectionMap[Key]);	// set the pointer in the temp value
	RenderMatrix[Key].SetEnclaveCollectionPtr(&EnclaveCollectionMatrixPtr->EnclaveCollectionMap[Key]);
	//auto finish3 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed3 = finish3 - start3;
	//cout << "---------------create render array from manifest call time: " << elapsed3.count() << endl;

	RenderMatrix[Key].CombineManifestArrays(std::ref(mutexval));
}
void RenderCollectionMatrix::CreateRenderArrayFromFactory(EnclaveKeyDef::EnclaveKey Key, EnclaveManifestFactoryT1 *factoryRef, mutex& mutexRef)
{
	RenderMatrix[Key].SetManifestCollectionPtr(&ManifestCollectionMatrixPtr->ManiCollectionMap[Key]);	// set the pointer in the temp value
	RenderMatrix[Key].SetEnclaveCollectionPtr(&EnclaveCollectionMatrixPtr->EnclaveCollectionMap[Key]);
	RenderMatrix[Key].CombineManifestArraysFromT1Factory(factoryRef, std::ref(mutexRef));
}

void RenderCollectionMatrix::SetManifestCollectionMatrixPtr(ManifestCollectionMatrix *manifestcollectionmatrixref)
{
	/* Summary: manually sets the ManifestCollectionMatrixPtr; for use with OrganicSystem objects */

	ManifestCollectionMatrixPtr = manifestcollectionmatrixref;
}

void RenderCollectionMatrix::SetEnclaveCollectionMatrixPtr(EnclaveCollectionMatrix *enclavecollectionmatrixref)
{
	EnclaveCollectionMatrixPtr = enclavecollectionmatrixref;
	//cout << "ptr gets called..." << endl;
}

RenderCollectionMatrix::RenderCollectionMatrix(ManifestCollectionMatrix *manifestcollectionmatrixref)
{
	/* Summary: sets the ManifestCollectionMatrixPtr. */
	ManifestCollectionMatrixPtr = manifestcollectionmatrixref;
}

RenderCollectionMatrix::RenderCollectionMatrix()
{
	/* Summary: constructor called so that this object may be utilized in OrganicSystem.h */
}