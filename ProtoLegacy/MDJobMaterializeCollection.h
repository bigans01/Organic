/*------------------------------------------------------------------------------------------

--MDJobMaterializeCollection.h		(Last update 9/4/2017)

Description: Header file for MDJobMaterializeCollection.cpp

Summary: An MDJobMaterializeCollection contains all the data needed to run a collection 
through the standard pipeline (create enclave collection, create manifests, create finalized render collection)

Notes: When adding new member variables, accidentally copying a referenced value instead of using a pointer will cause SEVERE performance loss. USE POINTERS ONLY

------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef MDJOBMATERIALIZECOLLECTION_H
#define MDJOBMATERIALIZECOLLECTION_H

# include <EnclaveKeyDef.h>
#include <EnclaveCollectionBlueprintMatrix.h>
#include <EnclaveCollectionMatrix.h>
#include <ManifestCollectionMatrix.h>
#include <RenderCollectionMatrix.h>
#include <functional>

class OrganicSystem;
class MDJobMaterializeCollection
{
public:
	MDJobMaterializeCollection();									// a dummy constructor
private:
	friend class OrganicSystem;
	EnclaveKeyDef::EnclaveKey MDKey;								// the unique key of the collection that will be processed
	EnclaveCollectionBlueprintMatrix* MDBlueprintMatrixRef = NULL;			// a pointer to the blueprint matrix
	EnclaveCollectionMatrix* MDEnclaveCollectionsRef = NULL;				// a pointer to the enclave collection matrix
	ManifestCollectionMatrix* MDManifestCollectionsRef = NULL;				// a pointer to the manifest collection matrix; this is not used with Factories but is included anyway
	RenderCollectionMatrix* MDRenderCollectionsRef = NULL;					// a pointer to the render collection list
	EnclaveCollection* MDEnclaveCollectionPtr = NULL;						// a pointer to a valid enclave collection
	ManifestCollection* MDManifestCollectionPtr = NULL;					// a pointer to a valid manifest collection; this is not used with factories but is included anyway
	int currentRenderMode = 0;


	MDJobMaterializeCollection(																					/* the only non-constructor function, 
																												   passes all necessary parameters for this 
																												   object to function. */
									EnclaveKeyDef::EnclaveKey p_MDKey,
									EnclaveCollectionBlueprintMatrix* p_MDBlueprintMatrixRef,
									EnclaveCollectionMatrix* p_MDEnclaveCollectionsRef,
									ManifestCollectionMatrix* p_MDManifestCollectionsRef,
									RenderCollectionMatrix* p_MDRenderCollectionsRef,
									EnclaveCollection* p_MDEnclaveCollectionPtr,
									ManifestCollection* p_MDManifestCollectionPtr,
									int in_renderMode
											);

};

#endif