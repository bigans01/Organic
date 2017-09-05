/*------------------------------------------------------------------------------------------

--EnclaveCollectionActivateListT2.h		(Last update 9/4/2017)

Description: Header file for EnclaveCollectionActivateListT2 data structure.

Summary: An EnclaveCollectionActivateListT2 object contains one two dimensional array (8 x 8) of 64 bytes; each dimension represents either the x or z axis. 
The first bit in each byte represents y = 0, with the last bit of the byte representing y = 7.


Dependents: none.


------------------------------------------------------------------------------------------*/


#pragma once


// activate guards
#ifndef ENCLAVECOLLECTIONACTIVATELISTT2_H
#define ENCLAVECOLLECTIONACTIVATELISTT2_H


class EnclaveCollectionActivateListT2 {
public:

	int flagArray[8][8] = { 0, 0, 0, 0, 0, 0, 0, 0		// potential unnecessary intialization (review later)
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0
							,0, 0, 0, 0, 0, 0, 0, 0		
							};
};

#endif