/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 9/15/2017)

Description: Header file for EnclaveCollectionBlueprintMatrix.cpp

Summary: An EnclaveCollectionBlueprintMatrix will contain one or more EnclaveCollectionBlueprint in an unordered_map, with the key being a valid EnclaveKey. This class
will be used by the OrganicSystem to determine what to do with a collection of enclaves that has valid key. 


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBLUEPRINTMATRIX_H
#define ENCLAVECOLLECTIONBLUEPRINTMATRIX_H

#include "EnclaveKeyDef.h"
#include "EnclaveCollectionBlueprint.h"
#include "EnclaveCollectionBorderFlags.h"
//#include "EnclaveCollectionNeighborList.h"
#include <unordered_map>

class EnclaveCollectionNeighborList;
//class OrganicSystem;
class OGLMBufferManager;
//class EnclaveCollectionMatrix;
//class EnclaveCollection;
class EnclaveCollectionBlueprintMatrix
{
	private:
		friend OrganicSystem;
		friend OGLMBufferManager;
		friend EnclaveCollectionMatrix;
		friend EnclaveCollection;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> BlueprintMap;
		EnclaveCollectionNeighborList DetermineBlueprintBordersToRender(EnclaveKeyDef::EnclaveKey blueprintKey,				// this function determines which blueprints exist next to this one.
																	    EnclaveCollectionBlueprint* originBlueprint, 
																		EnclaveCollectionBorderFlags* borderFlagsPtr, int mode);
};

#endif
