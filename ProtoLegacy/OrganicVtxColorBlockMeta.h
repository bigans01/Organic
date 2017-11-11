/*------------------------------------------------------------------------------------------

--OrganicVtxColorBlockMeta.h (Last Update: 9/4/2017)

Description: contains up to 32000~ different OrganicTextureMeta entries.


------------------------------------------------------------------------------------------*/


#pragma once

#ifndef ORGANICVTXCOLORBLOCKMETA_H
#define ORGANICVTXCOLORBLOCKMETA_H

#include "OrganicVtxColorFaceMeta.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicVtxColorBlockMeta
{
private:
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	OrganicVtxColorFaceMeta FaceIndex[6];	// contains values for all 6 faces of a block
};

#endif
