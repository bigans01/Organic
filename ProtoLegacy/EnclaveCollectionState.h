#pragma once

#ifndef ENCLAVECOLLECTIONSTATE_H
#define ENCLAVECOLLECTIONSTATE_H

#include "EnclaveCollection.h"

class EnclaveCollectionState
{
public:
	int isActive = 0;
	EnclaveCollection* collectionPtr;
	EnclaveCollectionState();
};

#endif
