/*------------------------------------------------------------------------------------------

--OrganicVtxColorMetaArray.h (Last Update: 9/4/2017)

Description: contains up to 32000~ different OrganicTextureMeta entries. 


------------------------------------------------------------------------------------------*/


#pragma once

#ifndef ORGANICVTXCOLORMETAARRAY_H
#define ORGANICVTXCOLORMETAARRAY_H

#include "OrganicVtxColorMeta.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicVtxColorMetaArray
{
	private:
		friend class OrganicSystem;
		friend class EnclaveManifestFactoryT1;
		friend class EnclaveManifest;
		OrganicVtxColorMeta Index[10000];
};

#endif

