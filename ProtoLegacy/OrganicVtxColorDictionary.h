/*------------------------------------------------------------------------------------------

--OrganicVtxColorDictionary.h (Last Update: 9/4/2017)

Description: contains one or more entries of OrganicVtxColorMetaArrays (?)

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORDICTIONARY_H
#define ORGANICVTXCOLORDICTIONARY_H

#include "OrganicVtxColorMetaArray.h"
#include <unordered_map>

class OrganicVtxColorDictionary
{
public:
	std::unordered_map<std::string, OrganicVtxColorMetaArray> Dictionary;	// unordered map that actually contains the Dictionary objects
	OrganicVtxColorDictionary();											// required so that te Dictionary can be added into the unordered map
};

#endif
