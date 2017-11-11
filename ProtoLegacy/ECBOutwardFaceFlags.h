/*------------------------------------------------------------------------------------------

--ECBOutwardFaceFlags.h		(Last update 9/15/2017)

Description: Header file for ECBOutwardFaceFlags.h

Summary: An ECBOutwardFaceFlags object contains 32 ints, one for each block that is on an x,y or z axis of an EnclaveCollection (an EnclaveCollection is 32 blocks wide)

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ECBOUTWARDFACEFLAGS_H
#define ECBOUTWARDFACEFLAGS_H

//class EnclaveCollectionBlueprint;
class ECBOutwardFaceFlags
{
private:
	friend class EnclaveCollectionBlueprint;
	int faceflagarray[32] = { 0 };		// the array of 32 ints, initialized for safety/best practice.
};

#endif
