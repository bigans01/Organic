/*------------------------------------------------------------------------------------------

--EnclaveUnveilMeta.h		(Last update 9/4/2017)

Description: Header file for EnclaveUnveilMeta data structure

Summary: Contains meta data for unveiling an enclave, for up to 16 blocks; meant to serve as a temporary object that is destroyed once it goes out of scope.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEUNVEILMETA_H
#define ENCLAVEUNVEILMETA_H

class EnclaveCollectionBlueprint;
class EnclaveCollection;
class Enclave;
class EnclaveUnveilMeta
{
private:
	friend class EnclaveCollectionBlueprint;
	friend class EnclaveCollection;
	friend class Enclave;
	int numberOfBlocks = 0;						// the number of blocks to be unveiled
	unsigned char EnclaveBlockLocation[4][4];	// the single number location of the block within the chunk, range will be from 0 to 63.
	unsigned char BlockFacesToRender[4][4];		// stores a value that is the equivalent of Enclave::EnclavePolyArray.t1_flags.
	unsigned char UnveilFlag[4][4] = { 0 };				// indicates whether or not to unveil the block in the corresponding location, by setting a flag to 1.
};

#endif
