/*------------------------------------------------------------------------------------------

--OrganicBlockMeta.h (Last Update: 8/6/2017)

Description: Contains 6 instances of OrganicFaceMeta; this all comes together as the texture data for once block.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICBLOCKMETA_H
#define ORGANICBLOCKMETA_H

#include "OrganicFaceMeta.h"

class OrganicBlockMeta
{
public:
	OrganicFaceMeta FaceIndex[6];	// one face for each direction: west (32), north (16), east (8), south (4), top (2), bottom (1)
};

#endif
