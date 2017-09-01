/*------------------------------------------------------------------------------------------

--EnclaveUnveilMeta.h		(Last update 8/16/2017)

Description: Header file for EnclaveUnveilMeta data structure

Summary: Contains meta data for unveiling an enclave; meant to serve as a temporary object


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEUNVEILMETA_H
#define ENCLAVEUNVEILMETA_H

class EnclaveUnveilMeta
{
public:
	int numberOfBlocks = 0;
	char EnclaveBlockLocation[4][4];
	char BlockFacesToRender[4][4];
};

#endif
