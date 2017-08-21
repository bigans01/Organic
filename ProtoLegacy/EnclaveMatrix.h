#pragma once

/*------------------------------------------------------------------------------------------

--EnclaveMatrix.h (Last update 8/16/2017)

Description: Header file for EnclaveMatrix.cpp. EnclaveMatrix.cpp is a currently deprecated class.


------------------------------------------------------------------------------------------*/

// activate guards
#ifndef ENCLAVEMATRIX_H
#define ENCLAVEMATRIX_H
#include <future>
#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "Enclave.h"
#include "EnclaveMultiJob.h"

class EnclaveMatrix
{

public:
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> PrimeMatrix;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_iter;
	EnclaveMultiJob testmulti;

	~EnclaveMatrix();
	
	void AddEnclaveToMatrix(int x, int y, int z);
	void SetRenderFlag(int x, int y, int z);
	void SetRenderFlag(EnclaveKeyDef::EnclaveKey inkey);
	void MultiSetRenderFlag(
		int start,
		int end, 
		std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &m_itermap1
	);
	void MultiSetRenderFlag2(
		int start, 
		int end, 
		std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> *m_itermap1, 
		std::promise<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> &promiseref,
		std::future<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> &future1
	);
	void AllocateWorldEnclaveMatrixToHeap();
	void DeallocateWorldEnclaveMatrixFromHeap();
	void ClearEnclaveFromMatrix(EnclaveKeyDef::EnclaveKey inkey);
	void ClearAllEnclavesFromMatrix();
	Enclave& GetEnclaveFromMatrix(int x, int y, int z);

	
};

#endif