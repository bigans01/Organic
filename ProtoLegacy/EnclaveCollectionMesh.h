#pragma once

#ifndef ENCLAVECOLLECTIONMESH_H
#define ENCLAVECOLLECTIONMESH_H

#include "EnclaveKeyDef.h"
#include "EnclaveManifest.h"
#include "EnclaveMatrix.h"
#include "ManifestCollection.h"
#include <GL/glew.h>
class EnclaveCollectionMesh
{
private:
	struct EnclaveDict
	{
		EnclaveKeyDef::EnclaveKey UniqueKey;
		EnclaveManifest *EnclaveManifestPtr;
	};


	ManifestCollection *EnclaveManifestRef = NULL;							// points to the passed EnclaveManifest reference
	GLfloat *RenderArrayPtr = NULL;									// points to the render array that is allocated on the heap
	EnclaveDict *EnclaveDictArray = NULL;
	int EnclaveDictArrayElementPointer = 0;						// keeps track of number of elements in EnclaveDictArray
	int TotalMeshTriangles = 0;

	EnclaveCollectionMesh(int arraysize, ManifestCollection *refmatrix);
	~EnclaveCollectionMesh();												// destructor for killing arrays on heap
	void AddManifestPtr(EnclaveKeyDef::EnclaveKey Key);
	void BuildRenderArray();
	void PopulateRenderArray();
	
};

#endif