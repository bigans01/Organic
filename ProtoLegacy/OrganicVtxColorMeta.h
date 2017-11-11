/*------------------------------------------------------------------------------------------

--OrganicVtxColorMeta.h (Last Update: 9/4/2017)

Description: contains up to 32000~ different OrganicTextureMeta entries.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORMETA_H
#define ORGANICVTXCOlORMETA_H

#include "OrganicVtxColorBlockMeta.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicVtxColorMetaArray;
class OrganicVtxColorMeta
{
private:
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	friend class OrganicVtxColorMetaArray;
	OrganicVtxColorBlockMeta BlockData;
	OrganicVtxColorMeta(int dummy);			// dummy constructor, needs rework at a later date (possibly)
	OrganicVtxColorMeta();					// dummy constructor, needs rework at a later date (possibly); required so that an instance of this object can be added to an unordered map.
};

#endif