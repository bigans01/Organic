/*------------------------------------------------------------------------------------------

--OGLMBufferManager.h		(Last update 9/15/2017)

Description: Header file for OGLMBufferManager.cpp

Summary: An OGLMBufferManager is responsible for determining which OpenGL buffers need to be updated for terrain rendering operations, as well as how many buffers need to be udpated for those operations. It contains several 3d matrices that are used in unison to properly 
determine what buffers need to be "morphed" or shifted around in the "mega-buffer"

Dependents: -a valid instance of an OrganicSystem
			-enough RAM on MemoryCard to support the desired buffer size (could possibly result in undefined behavior or worse, a crash)

Notes:	this will very likely be modified continuously throughout development.

------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef OGLMBUFFERMANAGER_H
#define OGLMBUFFERMANAGER_H

#include "EnclaveKeyDef.h"
#include "OGLMDrawCallMeta.h"
#include "OGLMRenderMetaContainer.h"
#include "OrganicMorphMeta.h"
#include "EnclaveCollectionBlueprintMatrix.h"

class OrganicGLManager;					// required forward declaration
class OrganicSystem;
class OGLMBufferManager {
private:
	friend class OrganicGLManager;
	friend class OrganicSystem;
	int* BufferOffsetMatrixArray;		// a pointer to a dynamic array which will contain memory offsets measured in bytes. these offsets represent the exact beginning location of each "sub-buffer" that a collection has within the mega buffer
	int* RenderCollectionMatrixArray;	// a pointer to a dynamic array which will contain pointers to the render collections that will need to be rendered.
	int* RenderableBufferList;			// a pointer to a sorted list that will determine how many buffers to render. This will be resorted every time a new buffer needs to be executed by OpenGL.
	int cubesize = 0;					// determines the x, y, and z lengths of the cube. 
	int arraysSet = 0;
	OrganicGLManager* OGLMPtr;
	OGLMRenderMetaContainer OGLMRMC;		// RMC = render meta container
	OGLMDrawCallMeta* DCMPtr;
	OrganicSystem* organicSystemPtr;
	EnclaveCollectionBlueprintMatrix* blueprintMatrixPtr;						// pointer to the OrganicSystem's blueprint matrix
	EnclaveKeyDef::EnclaveKey oldCenterCollectionKey;							// sets the key before the array morph
	EnclaveKeyDef::EnclaveKey currentCenterCollectionKey;						// sets the current key after morph
	int shiftFlag = 0;															// determines if a morph needs to occur; 0 = false, 1 = true

	//OGLMBufferManager();	
	~OGLMBufferManager();				// deletes all dynamically allocated objects created using the "new" operator

	void SetCubesize(int inCubesize);					// sets the buffer cube size, which is used to create the buffers
	void SetOGLMPointer(OrganicGLManager* in_OGLMptr);	// sets the pointer to the instance of OrganicGLManager that this instance of OGLMBufferManager will be associated with
	void GenerateArrays();								// generates a "cube" of arrays, based on the value of cubesize.
	void PopulateOGLMRMCArrays(EnclaveKeyDef::EnclaveKey centerCollectionKey);
	void ShiftMatricesForNW();
	int translateXYZToSingle(int x, int y, int z);

	void setShiftedCollectionKeys(EnclaveKeyDef::EnclaveKey oldKey, EnclaveKeyDef::EnclaveKey newKey);
	void determineMorphDirections();
	int determineRenderDataSubBufferKey(EnclaveKeyDef::EnclaveKey renderCollectionKey);
	void MorphTerrainBuffers();
};

#endif
