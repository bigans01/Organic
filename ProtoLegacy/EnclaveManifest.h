/*------------------------------------------------------------------------------------------

--EnclaveManifest.h		(Last update 9/15/2017)

Description: Header file for EnclaveManifest.cpp.

Summary: An EnclaveManifest is a class object that stores metadata about an Enclave's 3d data. The EnclaveManifest "attaches" to Enclaves, a process in which the Enclave's 3d point data is read to create an array of 3d points
that represent all visible polygon faces in the enclave. Blocks that are "veiled" (hidden -- see Enclave.cpp) are not rendered, and thus will not have any triangle data put into the array. The EnclaveManfiest is later pointed to by an associated
RenderCollection, which prepares the final array that is to be rendered by OpenGL.

Dependents: a valid instance of Enclave


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVE_MANIFEST_H
#define ENCLAVE_MANIFEST_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <GL/glew.h>
#include "Enclave.h"
#include "EnclaveCollectionMatrix.h"
#include "OrganicTextureDictionary.h"
#include "OrganicVtxColorDictionary.h"
#include "FloatTupleXYZ.h"
//#include "RenderCollection.h"

class ManifestCollection;
class RenderCollection;																		// why did this work? (7/2/2017) (forward declaration solved compiler issue) 
class EnclaveCollectionMesh;
class EnclaveManifest {

public:
	//EnclaveManifest(EnclaveManifest& EMa) : dumbval(5), testint(std::move(EMa.testint)) {};
	//EnclaveManifest(EnclaveManifest& EMa) : dumbval(5) {};
	EnclaveManifest::~EnclaveManifest();									// required destructor for memory management
	EnclaveManifest(int x, int y, int z);													// constructor which sets the value of the EnclaveManifest's UniqueKey
	EnclaveManifest(int x, int y, int z, OrganicTextureDictionary *texturedictionaryptr, OrganicVtxColorDictionary *vertexcolordictionaryptr);
	EnclaveManifest();																		// potentially unused


	
	
	
	EnclaveManifest& operator=(EnclaveManifest& manifest_a)
	{
		//testint = std::move(manifest_a.testint);
		return *this;
	}
	
	
private:
	friend class ManifestCollection;
	friend class RenderCollection;
	friend class EnclaveCollectionMesh;
	int IsEnclaveGLPtrLoaded = 0;															// flag that determines whether or not an array of 3d data currently exists for this manifest									// currently unused
	int RenderablePolyCount = 0;															// the number of currently "unveiled" polygons in the enclave
	int TotalEnclaveTriangles = 0;															// the total number of triangles that will be rendered in this enclave manifest
	GLfloat *EnclaveGLPtr = NULL;																	// pointer to the array of OpenGL floats, which will be used by an instnce of RenderCollection.
	GLfloat *VertexColorGLPtr = NULL;
	GLfloat *TextureGLPtr = NULL;																	// pointer to the array of texture data
	RenderCollection *RenderCollectionRef = NULL;													// pointer to the RenderCollection that this EnclaveManifest belongs to; this is used to signal 
																							// the RenderCollection that this instance of EnclaveManifest has changed
	//unique_ptr<int> testint;
	OrganicTextureDictionary *TextureDictionaryRef = NULL;											// reference to the Organic system's OrganicTextureDictionary
	OrganicVtxColorDictionary *VertexColorDictionaryRef = NULL;									// reference to the Organic system's OrganicVtxColorDictionary
	int dumbval = 0;
	int IsRenderCollectionRefSet = 0;														// determines whether or not the pointer RenderCollectionRef is set
	Enclave EnclavePtr;																		// pointer to the enclave to gain data from
	Enclave::EnclavePolyArray *EnclavePolyArrayPtr = NULL;											// pointer to the sorted array in the enclave. in the Sorted struct. (see Enclave.h)
	int polyfacebitmask = 32;																// bitmask for determining which faces of a polygon will be rendered

	EnclaveKeyDef::EnclaveKey UniqueKey;													// the unique EnclaveKey of this manifest

	FloatTupleXYZ EnclaveManifestOffset, TempTuple;	// EnclaveManifestOffset: a tuple of x/y/z values that determines the block's offset from the beginning of the Enclave's 0,0 point.
	mutex* in_heapmutex;


																			// TempTuple: temporarily stores the x/y/z tuple that contains the final values for a block's representation in 3d space
	void AttachToEnclave(Enclave &in_ptr);									// "Attaches" the enclave manifest to an appropriate instance of Enclave; this action generates the 3d data that will be stored in the EnclaveManifest.
	void AttachToEnclave(Enclave &in_ptr, mutex& heapmutex);				// Same as AttachToEnclave(Enclave &in_ptr), but passes in a mutex for finer-grained control and release of the mutex.
	int ReturnEnclaveTriangles();											// returns the total number of triangles rendered in this manifest. This will be used by a RenderCollection.
	FloatTupleXYZ SingleToMulti(int input);							// takes in a single value, from 0 to 63, and returns a chunk-level x/y/z offset or coordinate
	
};

#endif