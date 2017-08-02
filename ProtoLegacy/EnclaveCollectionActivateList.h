#pragma once

// activate guards
#ifndef ENCLAVECOLLECTIONACTIVATELIST_H
#define ENCLAVECOLLECTIONACTIVATELIST_H

#include "EnclaveKeyDef.h"

class EnclaveCollectionActivateList {
public:
	/*
	struct S {
			double operator()(char, int&);
			float operator()(int) { return 1.0; }
		};
	*/
	//EnclaveCollectionActivateList operator()();
	int count = 0;
	EnclaveKeyDef::EnclaveKey activatables[512];
	char flagArray[8][8];
};

#endif