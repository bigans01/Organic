#pragma once

// define guards
#ifndef ENCLAVEDATAFINDER_H
#define ENCLAVEDATAFINDER_H

#include <EnclaveKeyDef.h>

class EnclaveDataFinder
{
private:
	EnclaveKeyDef::EnclaveKey DFKey;
	int indexStart;
	int totalTriangles;
};

#endif