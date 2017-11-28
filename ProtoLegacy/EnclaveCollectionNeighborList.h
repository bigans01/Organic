/*-----------------------------------------------------------------

--EnclaveCollectionNeighborList.h: (Last update 9/15/2017)

Summary: EnclaveCollectionNeighborList is an object that stores up to 6 pointers, with a flag that is to be set whenever the pointer is valid.

-----------------------------------------------------------------*/
#pragma once

#ifndef ENCLAVECOLLECTIONNEIGHBORLIST_H
#define ENCLAVECOLLECTIONNEIGHBORLIST_H

#include "EnclaveCollectionBlueprint.h"

class EnclaveCollectionBlueprintMatrix;
class Enclave;
class EnclaveCollection;
class EnclaveCollectionNeighborList
{
private:
	friend class EnclaveCollectionBlueprintMatrix;
	friend class Enclave;
	friend class EnclaveCollection;
	int WestNeighborExists = 0;			// determines if there is a neighboring collection west of this one; set to 1 if this is true
	int NorthNeighborExists = 0;
	int EastNeighborExists = 0;
	int SouthNeighborExists = 0;
	int TopNeighborExists = 0;
	int BottomNeighborExists = 0;

	EnclaveCollectionBlueprint* originPtr = NULL;		// the origin blueprint
	EnclaveCollectionBlueprint* westPtr = NULL;		// blueprint west of the origin 
	EnclaveCollectionBlueprint* northPtr = NULL;		// blueprint north of the origin
	EnclaveCollectionBlueprint* eastPtr = NULL;		// blueprint east of the origin
	EnclaveCollectionBlueprint* southPtr = NULL;		// blueprint south of the origin
	EnclaveCollectionBlueprint* topPtr = NULL;			// blueprint above the origin
	EnclaveCollectionBlueprint* bottomPtr = NULL;		// blueprint below the origin


};

#endif