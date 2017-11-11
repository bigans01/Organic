/*------------------------------------------------------------------------------------------

--OrganicTextureCoords.h	(Last Update: 8/6/2017)

Description: contains a single pair of UV coordinates used in finding a coordinate on a texture map.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTURECOORDS_H
#define ORGANICTEXTURECOORDS_H

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicTextureCoords
{
private:
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	float U;
	float V;
};

#endif