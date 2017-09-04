/*------------------------------------------------------------------------------------------

--Enclave.h		(Last update 8/16/2017)

Description: Header file for Enclave.cpp.

Summary: An Enclave stores meta data about 64 blocks, as well as several Enclave-level flags . It is the second smallest unit of storage in the Organic system, second only to an individual Polygon.
Meta-data includes information such as the Enclave's unique key, the total number of renderable blocks, and the total number of renderable triangles. All per-block operations are performed through methods
defined in the class' public body; this includes for instance, hiding/unhiding a block, changing the block type, and updating renderable faces. The realm of operation for this class' methods is limited 
to the Enclave itself and its constituent blocks.

Dependents: -a valid instance of EnclaveCollectionMatrix (if adding to a collection)

Notes:	A valid EnclaveCollectionMatrix must exist before the Enclave is added to an EnclaveCollectionMatrix, but the instance of the Enclave may be created separately.

------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef ENCLAVE_H
#define ENCLAVE_H
using namespace std;

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "EnclaveKeyDef.h"

class Enclave {

public:
	int total_triangles = 0;												// the sum of all triangles that will be rendered from the currently unveiled polygons.
	EnclaveKeyDef::EnclaveKey UniqueKey, CollectionKey;						// the unique Key identifying the Enclave, and the key of the Collection that this enclave belongs to; 
																			// CollectionKey is used when an EnclaveManifest is attached to an enclave, in order to get the exact x/y/z coords of the polygon in a chunk.
	struct EnclaveVertex {													// this struct stores "compressed" values of the xyz.
		char x, y, z;														// for use in compressed data below...values will be multiplied by a matrix of floats, in order to produce appropriate values for OpenGL. This is to save data.
	};
	struct EnclavePolyArray {						/* ----Total size: 52 bytes---- */
		int readorder = 0;								// unused, reserverd for future use
		int otherflags = 0;								// is block air, is block solid, is block set to render, etc
		short blockid = 0;								// the type of block 
		char t1_flags = 0;								// flags for t1 type objects
		char t2_flags = 0;								// flags for t2 type objects
		EnclaveVertex structarray[16];					// 12 bytes per vertex, (16 vertexes = 192)
														/* First 8 vertexes [indexes 0 to 7] = for T1 types
														Next 8 vertexes [indexes 8 to 15] = for T2 types (reserved for later use) */
	};

	EnclavePolyArray StorageArray[4][4][4];				// will store all block data, for 64 blocks

	struct EnclaveRenderable {
		int PolyArrayIndex[64] = { 0 };					// contents must be initialzied to 0 when created on heap, for safety reasons (and also best practice)
		EnclavePolyArray *RenderArray[64];				// an array of up to 64 pointers. Utilized by SortRenderArray(), and to minimize find time of renderable polygons.
	};

	EnclaveRenderable Sorted;							// creates an instance of the previously defined struct. 
	short EnclaveShapeMode = 1;							// currently unused but reserved for future use (Chunk level flag)
	short TotalRenderable = 0;							// the total number of renderable polygons that will be rendered when an EnclaveManifest attaches to this. (see EnclaveManifest.cpp/h)



	Enclave();																// default constructor.
	Enclave(EnclaveKeyDef::EnclaveKey collectkey, int x, int y, int z);		// Required constructor for adding to a valid EnclaveCollectionMatrix
	Enclave(int x, int y, int z);											// Required constructor for creating an Enclave in a case other than adding it to a collection
	
	

	int testsize();																																	// can be removed at a later point in time; returns size of enclave
	void InitializeRenderArray();																													/* Initializes the contents of the array, when the object is created on the heap. NOTE: This should always be called immediately after the 
																																					 enclave is added to the heap (for instance, a vector).*/
	void InitializeRenderArray(int blockid);																										// initializes arrays in the enclave, and sets all blocks to be the value of the input parameter 
	void SortRenderArray();																															// this function performs sorting on RenderArray -- will be used after InitializeRenderArray() is called, to sort for the first time (Insertion sort?)
	void UnveilSinglePolyWithMtrl(int x, int y, int z, int in_readorder, int in_otherflags, short in_blockid, char in_t1, char in_t2);				// Reveals one block at xyz and sets its material, so that it may be rendered.
	void UnveilSinglePoly(int x, int y, int z, int in_readorder, int in_otherflags, char in_t1, char in_t2);
	void VeilSinglePoly(int x, int y, int z, int in_readorder, int in_otherflags, short in_blockid, char in_t1, char in_t2);	// Hides one block at xyz, so that it will no longer be rendered.
	void ChangePolyMaterial(int x, int y, int z, int newmaterial);	// changes the material of a block at the x/y/z value within the chunk
	void TestTopLayer();								// testing purpose only
	void ViewOtherFlags();								// testing purpose only
	int	GetTotalTrianglesInBlock(char in_char);		// Gets that current number of trinagles at a block.
	int GetTotalTrianglesInEnclave();					// Returns the total number of triangles that exist in the enclave. 
	EnclaveKeyDef::EnclaveKey SingleToEnclaveKey(int input);
	
};

#endif