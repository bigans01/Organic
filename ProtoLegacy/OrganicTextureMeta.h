/*------------------------------------------------------------------------------------------

--OrganicTextureMeta.h (Last Update: 8/6/2017)

Description: Contains meta data for a cube-based texture that will be used in the Organic system.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTUREMETA_H
#define ORGANICTEXTUREMETA_H

#include "OrganicBlockMeta.h"

class OrganicTextureMeta {
public:
	OrganicBlockMeta BlockData;			// contains all texture UV point data for a single block.
	OrganicTextureMeta(int dummy);		// used to instantiate 
	OrganicTextureMeta();				// used for map compatibility
};

#endif
