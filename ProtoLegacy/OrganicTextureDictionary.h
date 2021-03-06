/*------------------------------------------------------------------------------------------

--OrganicTextureDictionary.h (Last Update: 8/16/2017)

Description: contains one or more entries of OrganicTextureMetaArrays. Values must be created on stack due to their length.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTUREDICTIONARY_H
#define ORGANICTEXTUREDICTIONARY_H

#include "OrganicTextureMetaArray.h"
#include <unordered_map>

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class ManifestCollection;
class OrganicTextureDictionary
{
//public:
	//int dumbval;
private:
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	friend class ManifestCollection;
	std::unordered_map<std::string, OrganicTextureMetaArray> Dictionary;				// the member variable within the class that actually contains the OrganicTextureMetaArrays
	int dumbval = 0;																	
	OrganicTextureDictionary();															// default constructor
};

#endif
