/*------------------------------------------------------------------------------------------

--OrganicFaceMeta.h (Last Update: 8/6/2017)

Description: Contains 6 instances of OrganicTextureCoords; this all comes together as the texture data for once face.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICFACEMETA_H
#define ORGANICFACEMETA_H

#include "OrganicTextureCoords.h"

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicFaceMeta
{
	private:
		friend class OrganicSystem;
		friend class EnclaveManifestFactoryT1;
		friend class EnclaveManifest;
		OrganicTextureCoords FaceData[6];	// one pair of texture coords per vertex (6 vertexes per face)
};

#endif

