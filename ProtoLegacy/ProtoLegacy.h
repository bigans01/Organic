/*------------------------------------------------------------------------------------------

--ProtoLegacy.h		(Last update 8/16/2017)

Description: unused header file for ProtoLegacy.h

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef PROTOLEGACY_H
#define PROTOLEGACY_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>

class ProtoLegacy {

public:

	//int main();
	void AllocateWorld();
	void DeallocateWorldEnclaveMatrixFromHeap();
	void AddEnclaveToMatrix(int x, int y, int z);
	void ClearAllEnclavesFromMatrix();

};
#endif