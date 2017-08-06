/*------------------------------------------------------------------------------------------

--OrganicTextureDictionary.h (Last Update: 8/6/2017)

Description: contains one or more entries of OrganicTextureMetaArrays. Values must be created on stack due to their length.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICTEXTUREDICTIONARY_H
#define ORGANICTEXTUREDICTIONARY_H

#include "OrganicTextureMetaArray.h"
#include <unordered_map>

class OrganicTextureDictionary
{
public:
	std::unordered_map<std::string, OrganicTextureMetaArray> Dictionary;
	OrganicTextureDictionary();
};

#endif
