#pragma once

#ifndef ENCLAVEMULTIJOB_H
#define ENCLAVEMULTIJOB_H

#include <future>

class EnclaveMatrix;
class EnclaveMultiJob
{
	private:
		friend class EnclaveMatrix;
		void RunMultiJob(
			int x, 
			int y, 
			std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> &matrixref, 
			std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &m_itermap
		);

		void RunMultiJob2(
			int x, 
			int y, 
			std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> &matrixref, 
			std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &itermapref,
			std::promise<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> *promiseref
			//int z
		);
};
#endif