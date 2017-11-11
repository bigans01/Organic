/*------------------------------------------------------------------------------------------

--EnclavePainterList.h		(Last update 7/21/2017)

Description: Header file for EnclavePainterList

Summary: An EnclavePainter is an object that contains a list of EnclavePainters.


------------------------------------------------------------------------------------------*/
#pragma once

#ifndef ENCLAVEPAINTERLIST_H
#define ENCLAVEPAINTERLIST_H

#include "EnclavePainter.h"
#include <unordered_map>

class EnclaveCollectionBlueprint;
class EnclaveCollectionMatrix;
class EnclavePainterList {
	private:
		friend class EnclaveCollectionBlueprint;
		friend class EnclaveCollectionMatrix;
		std::unordered_map<int, EnclavePainter> PaintList;	// PaintList consists of a key which serves as the ID of the block to paint; EnclavePainter determines which of the blocks 
															// in the Enclave to paint, where the the value of the block is equivalent to the value of the key in the key/element pair.
};

#endif