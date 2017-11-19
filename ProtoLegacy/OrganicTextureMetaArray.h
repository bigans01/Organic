/*------------------------------------------------------------------------------------------

--OrganicTextureMetaMap.h (Last Update: 8/6/2017)

Description: contains up to 32000~ different OrganicTextureMeta entries.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTUREMETAMAP_H
#define ORGANICTEXTUREMETAMAP_H

#include "OrganicTextureMeta.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class ManifestCollection;
class OrganicTextureMetaArray
{
private:
	friend class OrganicSystem;
	friend class ManifestColllection;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	OrganicTextureMeta Index[10000];			// can be increased beyond 10000 at a later time.
};

#endif