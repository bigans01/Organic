/*------------------------------------------------------------------------------------------

--EnclaveManifestFactoryT1.h	(Last Update 8/16/2017)

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
#include "OrganicVtxColorDictionary.h"

class EnclaveManifestFactoryT1
{
public:
	EnclaveCollection *EnclaveCollectionPtr;												// pointer to a valid EnclaveCollection
	Enclave::EnclavePolyArray *EnclavePolyArrayPtr;											// pointer to an EnclavePolyArray within previous EnclaveCollection
	OrganicTextureDictionary *TextureDictionaryRef;											// pointer to a valid OrganicTextureDictionary
	OrganicVtxColorDictionary *VertexColorDictionaryRef;

	EnclaveManifestFactoryT1Storage StorageArray[512];										// an array of 512 storage units 
	int CurrentStorage = 0;																	// a member variable that stores the current number of stored Enclaves
	int StorageArrayCount = 0;																// ""
	int polyfacebitmask = 32;																// polyfacebitmask that is used as storage units are instantiated
	
	struct EnclaveManifestTuple {															// 3 floats per vertex, up to 16 possible points per "block", 64 blocks
		GLfloat x = 0.0f;																	// instantiate
		GLfloat y = 0.0f;
		GLfloat z = 0.0f;
	};
	EnclaveManifestFactoryT1::EnclaveManifestTuple EnclaveManifestOffset, TempTuple;		// two EnclaveManifestTuples that are used during the iteration through the Factory's storage units

	void AttachManifestToEnclave(Enclave *in_ptr);											// attaches a manifest (contained within this Factory) to a valid Enclave
	void SetEnclaveCollectionPtr(EnclaveCollection *InPtr);									// sets the pointer to a valid EnclaveCollection
	EnclaveManifestTuple SingleToMulti(int input);											// EnclaveManifestTuple
};

#endif