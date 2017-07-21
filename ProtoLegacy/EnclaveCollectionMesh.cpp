#pragma once



#include "stdafx.h"
#include <stdio.h>
#include <iostream>

#include "EnclaveCollectionMesh.h"
//#include "EnclaveMatrix.h"
//#include "EnclaveKeyDef.h"
//#include "ManifestMatrix.h"


void EnclaveCollectionMesh::AddManifestPtr(EnclaveKeyDef::EnclaveKey Key)
{
	//cout << "destructor check test..." << endl;
	EnclaveDict tempDict;
	tempDict.UniqueKey = Key;
	tempDict.EnclaveManifestPtr = &EnclaveManifestRef->GetManifestFromMatrix(Key.x, Key.y, Key.z);
	//cout << "destructor check test...2" << endl;
	TotalMeshTriangles += tempDict.EnclaveManifestPtr->ReturnEnclaveTriangles();
	EnclaveDictArray[EnclaveDictArrayElementPointer++] = tempDict;

	//std::cout << "test value of mesh triangles: " << TotalMeshTriangles << endl;
}

void EnclaveCollectionMesh::BuildRenderArray()
{
	RenderArrayPtr = new GLfloat[TotalMeshTriangles];
}

void EnclaveCollectionMesh::PopulateRenderArray()
{
	for (int x = 0; x < EnclaveDictArrayElementPointer; x++)
	{
		//EnclaveDictArray[x].EnclaveManifestPtr->
	}
}

EnclaveCollectionMesh::EnclaveCollectionMesh(int arraysize, ManifestCollection *refmatrix)
{
	EnclaveManifestRef = refmatrix;
	EnclaveDictArray = new EnclaveDict[arraysize];
}

EnclaveCollectionMesh::~EnclaveCollectionMesh()
{
	//cout << "EnclaveMesh destructor called..." << endl;
	delete[] EnclaveDictArray;							// delete enclave dict array array on destruction
	//cout << "EnclaveMesh destructor called...(EnclaveDictArray delete passed) " << endl;
	delete[] RenderArrayPtr;
	//cout << "EnclaveMesh destructor called...(RenderarrayPtr delete passed) " << endl;
}
