#pragma once

#ifndef ENCLAVEKEYCONTAINER_H
#define ENCLAVEKEYCONTAINER_H

#include <vector>
#include "EnclaveKeyDef.h"

class EnclaveKeyContainer
{
public:	
	std::vector<EnclaveKeyDef::EnclaveKey> KeyVector;
};

#endif