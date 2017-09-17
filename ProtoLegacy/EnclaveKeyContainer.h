/*------------------------------------------------------------------------------------------

--RenderCollection.h		(Last update 9/15/2017)

Description: Header file for EnclaveKeyContainer data structure.

Summary: an EnclaveKeyContainer is an object which contains a vector, "KeyVector" which is a list of EnclaveKeys that is used to determine what to send to OpenGL for rendering.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEKEYCONTAINER_H
#define ENCLAVEKEYCONTAINER_H

#include <vector>
#include "EnclaveKeyDef.h"

class EnclaveKeyContainer
{
public:	
	std::vector<EnclaveKeyDef::EnclaveKey> KeyVector;	// the vector that the keys will be stored in
};

#endif