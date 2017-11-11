/*-----------------------------------------------------------------

--EnclaveNeighborMeta.h: header file for EnclaveNeighborMeta (Last update 9/15/2017)

Summary: EnclaveNeighborMeta is a data structure that contains 6 EnclaveUnveilMeta objects; one for each neighboring chunk. Each of these is used to
determine which faces to render on blocks that face other chunks.

-----------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef ENCLAVENEIGHBORMETA_H
#define ENCLAVENEIGHBORMETA_H

#include "EnclaveUnveilMeta.h"

class Enclave;
class EnclaveNeighborMeta
{
private:
	friend class Enclave;
	unsigned char NeighborBlockData[4][4][4];		// stores up to 64 values that are equivalent to an Enclave "flag" (i.e., t1_flag, t2_flag) etc


};

#endif




