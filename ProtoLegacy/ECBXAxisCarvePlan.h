/*------------------------------------------------------------------------------------------

--ECBXAxisCarvePlan.h		(Last update 8/29/2017)

Description: Header file for Enclave.cpp.
 
Summary: contains a 32 x 32 array of chars that indicate the "top" block's y coordinate (x and z) are stored in the first and second dimensions of the two-dimensional array, 
respectively. This is designed to be used on elevations meant to be carved along the x-axis.

Dependents: none



------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ECBXAXISCARVEPLAN_H
#define ECBXAXISCARVEPLAN_H

//class EnclaveCollectionBlueprint;
class ECBXAxisCarvePlan
{
private:
	friend class  EnclaveCollectionBlueprint;
	unsigned char CPArray[32][32] = { 0 };			// first dimension is x, second is z

};

#endif