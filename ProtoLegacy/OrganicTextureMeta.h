/*------------------------------------------------------------------------------------------

--OrganicTextureMeta.h (Last Update: 8/6/2017)

Description: Contains meta data for a cube-based texture that will be used in the Organic system.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTUREMETA_H
#define ORGANICTEXTUREMETA_H

#include "OrganicBlockMeta.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicTextureMetaArray;
class OrganicTextureMeta {
private:
	friend class OrganicTextureMetaArray;
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	OrganicBlockMeta BlockData;			// contains all texture UV point data for a single block.
	OrganicTextureMeta(int dummy);		// used to instantiate 
	OrganicTextureMeta();				// used for map compatibility
};

#endif
