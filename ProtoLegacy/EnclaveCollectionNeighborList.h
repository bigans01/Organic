/*-----------------------------------------------------------------

--EnclaveCollectionNeighborList.h: (Last update 9/15/2017)

Summary: EnclaveCollectionNeighborList is an object that stores up to 6 pointers, with a flag that is to be set whenever the pointer is valid.

-----------------------------------------------------------------*/
#pragma once

#ifndef ENCLAVECOLLECTIONNEIGHBORLIST_H
#define ENCLAVECOLLECTIONNEIGHBORLIST_H

#include "EnclaveCollectionBlueprint.h"


class EnclaveCollectionNeighborList
{
public:
	int WestNeighborExists = 0;			// determines if there is a neighboring collection west of this one; set to 1 if this is true
	int NorthNeighborExists = 0;
	int EastNeighborExists = 0;
	int SouthNeighborExists = 0;
	int TopNeighborExists = 0;
	int BottomNeighborExists = 0;

	EnclaveCollectionBlueprint* originPtr;		// the origin blueprint
	EnclaveCollectionBlueprint* westPtr;		// blueprint west of the origin 
	EnclaveCollectionBlueprint* northPtr;		// blueprint north of the origin
	EnclaveCollectionBlueprint* eastPtr;		// blueprint east of the origin
	EnclaveCollectionBlueprint* southPtr;		// blueprint south of the origin
	EnclaveCollectionBlueprint* topPtr;			// blueprint above the origin
	EnclaveCollectionBlueprint* bottomPtr;		// blueprint below the origin


};

#endif