#pragma once

// define guards
#ifndef ENCLAVEDATAFINDER_H
#define ENCLAVEDATAFINDER_H

#include <EnclaveKeyDef.h>

class EnclaveDataFinder
{
public:
	EnclaveKeyDef::EnclaveKey DFKey;
	int indexStart;
	int totalTriangles;
};

#endif