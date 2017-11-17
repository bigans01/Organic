/*------------------------------------------------------------------------------------------

--OGLMBufferManager.h		(Last update 9/15/2017)

Description: Header file for OGLMBufferManager.cpp

Summary: An OGLMBufferManager is responsible for determining which OpenGL buffers need to be updated for terrain rendering operations, as well as how many buffers need to be udpated for those operations. It contains several 3d matrices that are used in unison to properly 
determine what buffers need to be "morphed" or shifted around in the "mega-buffer"; naming conventions in this class should follow the rules associated with the terrain types (T1, T2, etc)

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
	int T1_cubesize = 0;					// determines the x, y, and z axis lengths of the T1 dynamic array (cube)
	int T2_cubesize = 0;					// determines the x, y, and z axis lengths of the T2 dynamic array (cube)
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

	void SetOGLMPointer(OrganicGLManager* in_OGLMptr);	// sets the pointer to the instance of OrganicGLManager that this instance of OGLMBufferManager will be associated with
	void PopulateOGLMRMCArrays(EnclaveKeyDef::EnclaveKey centerCollectionKey);
	void T1_SetCubeSize(int inT1_CubeSize);
	int T1_translateXYZToSingle(int x, int y, int z);
	void T2_SetCubesize(int inT2_Cubesize);					// sets the buffer cube size, which is used to create the buffers
	int T2_translateXYZToSingle(int x, int y, int z);		// translates XYZ coords for the T2 dynamic array
	void setShiftedCollectionKeys(EnclaveKeyDef::EnclaveKey oldKey, EnclaveKeyDef::EnclaveKey newKey);
	void determineMorphDirections();
	int determineRenderDataSubBufferKey(EnclaveKeyDef::EnclaveKey renderCollectionKey);
	void MorphTerrainBuffers();				
	void MorphTerrainBufferWest();		// morphs the terrain buffer to the west by shifting 1 collection on -x
	void MorphTerrainBufferEast();		// morphs the terrain buffer to the east by shifting 1 collection on +x
	void MorphTerrainBufferBelow();		// morphs the terrain buffer below by shifting 1 collection on -y
	void MorphTerrainBufferAbove();		// morphs the terrain buffer above by shifting 1 collection on +y
	void MorphTerrainBufferNorth();     // morphs the terrain buffer to the north by shifting 1 collection on -z
	void MorphTerrainBufferSouth();		// morphs the terrain buffer to the south by shifting 1 collection on +z
};

#endif
