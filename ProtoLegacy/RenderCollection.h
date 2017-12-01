/*------------------------------------------------------------------------------------------

--RenderCollection.h		(Last update 8/14/2017)

Description: Header file for RenderCollection.cpp.

Summary: A RenderCollection is a object that contains the dynamically-created GLfloat arrays that will be rendered by OpenGL during draw calls. The RenderCollection interfaces with a corresponding ManifestCollection;
the RenderCollection uses the ManifestCollection's prepared 3d data (from all its contained EnclaveManifests) to create one massive array for rendering. It also supports operations for changing one Enclave's manifest, versus
rebuilding the entire array from scratch; the intent of this is to edit only a small portion of the array to save compute times. The RenderCollection also contains a list of EnclaveKeys that determine the order of EnclaveManifests to be read
in the RenderCollection's related ManifestCollection; This is done due to the fact that the order of elements in the unordered map contained within the ManifestCollection may be different than the order of the elements found in the RenderCollection,
which would lead to strange behavior when rendering texture data.

Dependents: -a valid instance of a ManifestCollection (preferably with the same EnclaveKey)

------------------------------------------------------------------------------------------*/


#pragma once


// activate guards
#ifndef RENDERCOLLECTION_H
#define RENDERCOLLECTION_H

#include "ManifestCollection.h"
#include "EnclaveManifestFactoryT1.h"
#include "EnclaveDataFinder.h"
#include <memory>
#include <mutex>

class RenderCollection {
public:
	RenderCollection();																					// default constructor?
	RenderCollection(int dummy);																		// potentially unused?
	~RenderCollection();																				// required for manual memory management; deletes dynamic array pointed to by GLFloatPtr (if it exists) 
private:
	friend class RenderCollectionMatrix;
	friend class OrganicSystem;
	friend class OrganicGLManager;
	friend class EnclaveManifest;
	EnclaveDataFinder enclaveDataStart[64];
	ManifestCollection *ManifestCollectionPtr;															// the pointer to the related ManifestCollection.
	EnclaveCollection *EnclaveCollectionPtr;															// pointer to the associated EnclaveCollection.
	//GLfloat *GLFloatPtr;																				// the pointer to this RenderCollection's dynamically created 3d array.
	//GLfloat *VertexColorArrayPtr;
	unique_ptr<GLfloat[]> GLFloatPtr;																	// Smart pointer rework(s), of above two comments (old dynamic arrays)
	unique_ptr<GLfloat[]> VertexColorArrayPtr;															// ""
	int IsGLFloatPtrLoaded = 0;																			// indicates whether or not there is a valid dynamic array pointed to by GLFloatPtr.
	int LastCollectionTriangleCount = 0;																// potentially unused?
	int RenderCollectionArraySize = 0;																	// indicates the size of the dynamic array in bytes (required for OpenGL glBufferData)
	int instantiatorFlag = 0;																			// has no purpose other than to instantiate this render collection on the heap (used with an OrganicSystem's heapMutex)
	struct EnclaveManifestMeta {																		// stores meta data about a single EnclaveManifest
		EnclaveKeyDef::EnclaveKey EnclaveManifestKey;
		int currentTriangleCount = 0;
		int changeFlag = 0;
	};
	struct CollectionMeta {																				// stores up to 512 instances of EnclaveManifestMeta.
		int CollectionTriangleCount = 0;
		int EnclaveManifestCount = 0;
		EnclaveManifestMeta MetaArray[512];
	};
	mutex* heapmutexref;																				// required for clean deletes
	CollectionMeta RenderableManifestMeta;
	void CombineManifestArrays();																		// iterates through all elements found in the ManifestCollection, and generates a dynamic array that is pointed to by GLFloatPtr.
	void CombineManifestArrays(mutex& mutexval);														// iterates through all elements found in the ManifestCollection, and generates a dynamic array that is pointed to by GLFloatPtr. (uses a mutex)
	void CombineManifestArraysFromT1Factory(EnclaveManifestFactoryT1 *factoryRef, mutex& mutexval);		// iterates through all of the current elements found in a T1 factory.
	void UpdateManifestArray(EnclaveKeyDef::EnclaveKey Key);											// rebuilds the dynamic array pointed to by GLFloatPtr by updating a single enclave manifest; designed for single enclave operations.
	void SetManifestCollectionPtr(ManifestCollection *manifestcollectionref);							// sets the pointer to the corresponding ManifestCollection that this RenderCollection will be associated with. 
	void SetEnclaveCollectionPtr(EnclaveCollection *enclavecollectionref);								// sets the pointer to the corresponding EnclaveCollection that this RenderCollection will be associated with.
	void AllocateNewArraysViaLockGuard(int in_numberOfFloats, mutex& mutexval);


};
#endif