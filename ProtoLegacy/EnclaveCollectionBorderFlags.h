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
	int West = 1;
	int North = 1;
	int East = 1;
	int South = 1;
	int Top = 1;
	int Bottom = 1;
};

#endif