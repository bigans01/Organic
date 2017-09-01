/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 7/17/2017)

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
#include <unordered_map>

class EnclaveCollectionBlueprintMatrix
{
	public:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> BlueprintMap;
		void DetermineBlueprintBordersToRender(EnclaveKeyDef::EnclaveKey blueprintKey, EnclaveCollectionBlueprint* originBlueprint, EnclaveCollectionBorderFlags* borderFlagsPtr);
};

#endif
