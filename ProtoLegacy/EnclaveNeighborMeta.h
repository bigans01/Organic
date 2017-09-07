/*-----------------------------------------------------------------

--EnclaveNeighborMeta.h: header file for EnclaveNeighborMeta

Summary: EnclaveNeighborMeta is a data structure that contains 6 EnclaveUnveilMeta objects; one for each neighboring chunk. Each of these is used to
determine which faces to render on blocks that face other chunks.

-----------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef ENCLAVENEIGHBORMETA_H
#define ENCLAVENEIGHBORMETA_H

#include "EnclaveUnveilMeta.h"

class EnclaveNeighborMeta
{
public:

	int count;
	struct UnveilMetaArray
	{
		int isSolid = 0;
		int neighborsBorderDirection = 0;
		EnclaveUnveilMeta unveilMeta;
	};
	UnveilMetaArray MetaArrays[6];


};

#endif




