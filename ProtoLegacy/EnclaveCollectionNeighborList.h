/*-----------------------------------------------------------------

--EnclaveCollectionNeighborList.h: header file for EnclaveCollectionNeighborList

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

	EnclaveCollectionBlueprint* originPtr;
	EnclaveCollectionBlueprint* westPtr;
	EnclaveCollectionBlueprint* northPtr;
	EnclaveCollectionBlueprint* eastPtr;
	EnclaveCollectionBlueprint* southPtr;
	EnclaveCollectionBlueprint* topPtr;
	EnclaveCollectionBlueprint* bottomPtr;


};

#endif