/*------------------------------------------------------------------------------------------

--EnclaveCollectionBorderFlags.h		(Last update 8/31/2017)

Description: Header file for EnclaveCollectionBorderFlags

Summary: Contains meta data for determining which borders of an EnclaveCollection (west, north, east, south, top, bottom) to render.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBORDERFLAGS_H
#define ENCLAVECOLLECTIONBORDERFLAGS_H

class EnclaveCollectionBorderFlags
{
public:
	int West = 0;
	int North = 0;
	int East = 0;
	int South = 0;
	int Top = 0;
	int Bottom = 0;
};

#endif