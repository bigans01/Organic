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