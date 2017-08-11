/*------------------------------------------------------------------------------------------

--EnclaveManifestFactoryT1.h	(Last Update 8/11/2017)

Description: Header file for EnclaveManifestFactoryT1.cpp

Summary: A EnclaveManifestFactoryT1 is a container that allocates memory for 512 EnclaveManifestFactoryT1Storage instances, which equates to roughly 28 megabytes.
The point of the factory is to create a "work area" of memory for each thread to process the vertex and texture data before it is finally 

Dependents: a valid instance of ManifestCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEMANIFESTFACTORYT1_H
#define ENCLAVEMANIFESTFACTORYT1_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "Enclave.h"
#include "EnclaveManifestFactoryT1Storage.h"
#include "EnclaveCollection.h"
#include "EnclaveManifest.h"
#include "OrganicTextureDictionary.h"

class EnclaveManifestFactoryT1
{
public:
	EnclaveCollection *EnclaveCollectionPtr;
	Enclave::EnclavePolyArray *EnclavePolyArrayPtr;
	OrganicTextureDictionary *TextureDictionaryRef;

	EnclaveManifestFactoryT1Storage StorageArray[512];
	int CurrentStorage = 0;		
	int StorageArrayCount = 0;
	int polyfacebitmask = 32;
	
	struct EnclaveManifestTuple {															// 3 floats per vertex, up to 16 possible points per "block", 64 blocks
		GLfloat x = 0.0f;																	// instantiate
		GLfloat y = 0.0f;
		GLfloat z = 0.0f;
	};
	EnclaveManifestFactoryT1::EnclaveManifestTuple EnclaveManifestOffset, TempTuple;

	void AttachManifestToEnclave(Enclave *in_ptr);
	void SetEnclaveCollectionPtr(EnclaveCollection *InPtr);
	EnclaveManifestTuple SingleToMulti(int input);				// EnclaveManifestTuple
};

#endif