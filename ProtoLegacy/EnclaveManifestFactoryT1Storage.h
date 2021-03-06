/*------------------------------------------------------------------------------------------

--EnclaveManifestFactoryT1Storage.h	(Last Update 9/4/2017)

Description: Header file for EnclaveManifestFactoryT1Storage.h

Summary: A EnclaveManifestFactoryT1Storage is an object that has memory allocated for all possible outcomes of vertex and texture data from a single Enclave's manifest. Although
it is possible not all memory is used, it is designed to be re-used while processing a single EnclaveCollection. The intent is to prevent calls to allocate memory from the heap (a costly operation).

Dependents: a valid instance of ManifestCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEMANIFESTFACTORYT1STORAGE_H
#define ENCLAVEMANIFESTFACTORYT1STORAGE_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>

class EnclaveManifestFactoryT1;
class RenderCollection;
class OrganicSystem;
class EnclaveManifestFactoryT1Storage {
private:
	friend class RenderCollection;
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	EnclaveKeyDef::EnclaveKey StorageKey;		// an EnclaveKey used to represent the proper EnclaveKey value of a stored Enclave
	int TotalEnclaveTriangles = 0;				// total triangles to be rendered, stored within this enclave; equivalent of the same variable in EnclaveManifest
	int VertexArrayCount = 0;					// the count of actual vertexes used in the VertexArray below (increments by one with each element added into VertexArray)
	int currentRenderMode = 0;
	GLfloat VertexArray[13824];					// 108 GLfloats per cube, x 64 = 6912.   (alternate: 13824)		(other alternate: 3456) // 6912 / 8 = 864
	int VertexColorArrayCount = 0;				// the count of actual texture data used in the VertexColorArray below (increments by one with each element added into VertexColorArray)
	GLfloat VertexColorArray[13824];			// 108 GLfloats per cube, x 64 = 6912.
	int TextureArrayCount = 0;					// the count of actual texture data used in the TextureArray below (increments by one with each element added into TextureArray)
	GLfloat TextureArray[9216];					// 72 GLfloats per cube, x 64 = 4608.
	
};

#endif