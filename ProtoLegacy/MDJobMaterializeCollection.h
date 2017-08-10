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

class MDJobMaterializeCollection
{
public:
	EnclaveKeyDef::EnclaveKey MDKey;
	EnclaveCollectionBlueprintMatrix MDBlueprintMatrixRef;
	EnclaveCollectionMatrix MDEnclaveCollectionsRef;
	ManifestCollectionMatrix MDManifestCollectionsRef;
	RenderCollectionMatrix MDRenderCollectionsRef;
	EnclaveCollection *MDEnclaveCollectionPtr;
	ManifestCollection *MDManifestCollectionPtr;
	//reference_wrapper<ManifestCollection> ManiCollectionPtr2;
	MDJobMaterializeCollection(
									EnclaveKeyDef::EnclaveKey p_MDKey,
										EnclaveCollectionBlueprintMatrix p_MDBlueprintMatrixRef,
									EnclaveCollectionMatrix p_MDEnclaveCollectionsRef,
									ManifestCollectionMatrix p_MDManifestCollectionsRef,
									RenderCollectionMatrix p_MDRenderCollectionsRef,
									EnclaveCollection *p_MDEnclaveCollectionPtr,
									ManifestCollection *p_MDManifestCollectionPtr
											);
};

#endif