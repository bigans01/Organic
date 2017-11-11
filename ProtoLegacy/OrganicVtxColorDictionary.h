/*------------------------------------------------------------------------------------------

--OrganicVtxColorDictionary.h (Last Update: 9/4/2017)

Description: contains one or more entries of OrganicVtxColorMetaArrays (?)

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORDICTIONARY_H
#define ORGANICVTXCOLORDICTIONARY_H

#include "OrganicVtxColorMetaArray.h"
#include <unordered_map>

class OrganicSystem;
class EnclaveManifestFactoryT1;
class EnclaveManifest;
class OrganicVtxColorDictionary
{
private:
	friend class OrganicSystem;
	friend class EnclaveManifestFactoryT1;
	friend class EnclaveManifest;
	std::unordered_map<std::string, OrganicVtxColorMetaArray> Dictionary;	// unordered map that actually contains the Dictionary objects
	OrganicVtxColorDictionary();											// required so that te Dictionary can be added into the unordered map
};

#endif
