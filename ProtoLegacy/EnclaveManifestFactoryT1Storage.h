/*------------------------------------------------------------------------------------------

--EnclaveManifestFactoryT1Storage.h	(Last Update 8/11/2017)

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

class EnclaveManifestFactoryT1Storage {
public:
	EnclaveKeyDef::EnclaveKey StorageKey;
	int TotalEnclaveTriangles = 0;
	int VertexArrayCount = 0;
	GLfloat VertexArray[13824];	// 108 GLfloats per cube, x 64 = 6912.   (alternate: 13824)		(other alternate: 3456) // 6912 / 8 = 864
	int TextureArrayCount = 0;
	GLfloat TextureArray[13824]; // 108 GLfloats per cube, x 64 = 6912.
	
};

#endif