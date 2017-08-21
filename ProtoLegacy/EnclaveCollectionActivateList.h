#pragma once

// activate guards
#ifndef ENCLAVECOLLECTIONACTIVATELIST_H
#define ENCLAVECOLLECTIONACTIVATELIST_H

#include "EnclaveKeyDef.h"

class EnclaveCollectionActivateList {
public:
	int count = 0;
	EnclaveKeyDef::EnclaveKey activatables[512];
	char flagArray[8][8];
};

#endif