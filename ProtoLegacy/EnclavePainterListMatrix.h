/*------------------------------------------------------------------------------------------

--EnclavePainterListMatrix.h		(Last update 7/21/2017)

Description: Header file for EnclavePainterListMatrix

Summary: An EnclavePainterListMatrix is an object that contains an unordered_map of EnclavePainterLists; this object will be found in the class "EnclaveCollectionBlueprint"


------------------------------------------------------------------------------------------*/
#pragma once

#ifndef ENCLAVEPAINTERLISTMATRIX_H
#define ENCLAVEPAINTERLISTMATRIX_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "EnclavePainterList.h"

class EnclavePainterListMatrix {
public:
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclavePainterList, EnclaveKeyDef::KeyHasher> PainterListMatrix;	// the painter list matrix
};

#endif
