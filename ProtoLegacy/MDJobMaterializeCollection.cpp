#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "MDJobMaterializeCollection.h"

MDJobMaterializeCollection::MDJobMaterializeCollection(
	EnclaveKeyDef::EnclaveKey p_MDKey,
	EnclaveCollectionBlueprintMatrix* p_MDBlueprintMatrixRef,
	EnclaveCollectionMatrix* p_MDEnclaveCollectionsRef,
	ManifestCollectionMatrix* p_MDManifestCollectionsRef,
	RenderCollectionMatrix* p_MDRenderCollectionsRef,
	EnclaveCollection *p_MDEnclaveCollectionPtr,
	ManifestCollection *p_MDManifestCollectionPtr,
	int in_renderMode
)
{
	MDKey = p_MDKey;
	MDBlueprintMatrixRef = p_MDBlueprintMatrixRef;
	MDEnclaveCollectionsRef= p_MDEnclaveCollectionsRef;
	MDManifestCollectionsRef = p_MDManifestCollectionsRef;
	MDRenderCollectionsRef = p_MDRenderCollectionsRef;
	MDEnclaveCollectionPtr = p_MDEnclaveCollectionPtr;
	MDManifestCollectionPtr = p_MDManifestCollectionPtr;
	currentRenderMode = in_renderMode;

}

MDJobMaterializeCollection::MDJobMaterializeCollection(																					/* the only non-constructor function,
																											passes all necessary parameters for this
																											object to function. */
	EnclaveKeyDef::EnclaveKey p_MDKey,
	EnclaveCollectionBlueprintMatrix* p_MDBlueprintMatrixRef,
	EnclaveCollectionMatrix* p_MDEnclaveCollectionsRef,
	ManifestCollectionMatrix* p_MDManifestCollectionsRef,
	RenderCollectionMatrix* p_MDRenderCollectionsRef,
	EnclaveCollection* p_MDEnclaveCollectionPtr,
	ManifestCollection* p_MDManifestCollectionPtr,
	int in_renderMode,
	EnclaveCollectionStateArray* p_MDStateArrayPtr
)
{
	MDKey = p_MDKey;
	MDBlueprintMatrixRef = p_MDBlueprintMatrixRef;
	MDEnclaveCollectionsRef = p_MDEnclaveCollectionsRef;
	MDManifestCollectionsRef = p_MDManifestCollectionsRef;
	MDRenderCollectionsRef = p_MDRenderCollectionsRef;
	MDEnclaveCollectionPtr = p_MDEnclaveCollectionPtr;
	MDManifestCollectionPtr = p_MDManifestCollectionPtr;
	currentRenderMode = in_renderMode;
	MDStateArrayPtr = p_MDStateArrayPtr;
}

MDJobMaterializeCollection::MDJobMaterializeCollection()
{

}