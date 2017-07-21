#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <thread>
#include "Enclave.h"
#include "EnclaveMatrix.h"
#include "EnclaveMultiJob.h"
#include <chrono>

void EnclaveMultiJob::RunMultiJob(
	int x, 
	int y, 
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> &matrixref, 
	std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &m_itermapref
)
{
	cout << "thread t1 called" << endl;

	auto start4 = std::chrono::high_resolution_clock::now();
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_itertest;
	EnclaveKeyDef::EnclaveKey tempkey;
	

		for (int cnt = x; cnt < y; cnt++)
		{
			tempkey.x = cnt;
			tempkey.y = 0;
			tempkey.z = 0;
			m_itertest = matrixref.find(tempkey);
			m_itertest->second.EnclaveShapeMode = 2;
			m_itermapref[tempkey] = m_itertest;  // decent cost, 10 milliseconds
		}

	auto finish4 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed4 = finish4 - start4;

	std::cout << "RUN MULTI JOB 1 ELAPSED ITERATOR TIME: " << elapsed4.count() << endl;
}










void EnclaveMultiJob::RunMultiJob2(
	int x, 
	int y, 
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> &matrixref, 
	std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &itermapref,
	std::promise<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> *promiseref
)
{
	cout << "RunMultiJob2 called" << endl;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> temp_itermapref;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator temp_iter;
	auto start4 = std::chrono::high_resolution_clock::now();
	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_itertest;
	//std::unordered_map<EnclaveKeyDef::EnclaveKey,std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> m_itermap;
	EnclaveKeyDef::EnclaveKey tempkey;

	//for (int y = 0; y < 700; y++)
	//{
	for (int cnt = x; cnt < y; cnt++)
	{
		tempkey.x = cnt;
		tempkey.y = 0;
		tempkey.z = 0;
		m_itertest = matrixref.find(tempkey);

		//m_iter = PrimeMatrix.find(tempkey);
		//PrimeMatrix[tempkey].EnclaveShapeMode=2;
		//m_itertest->second.EnclaveShapeMode = 2;
		//std::pair<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator> testpair(tempkey, m_itertest);

		itermapref[tempkey] = m_itertest;
		//temp_itermapref[tempkey] = m_itertest;
		//temp_itermapref.insert(testpair);
		//cout << m_itermapref[tempkey]->second.UniqueKey.x << " " << m_itermapref[tempkey]->second.UniqueKey.y << " " << m_itermapref[tempkey]->second.UniqueKey.z << " " << endl;
	}
	//}
	auto finish4 = std::chrono::high_resolution_clock::now();

	int tempcount = 0;
	//for (temp_iter = temp_itermapref.begin(); temp_iter != temp_itermapref.end(); temp_iter++)
	//{
		//temp_iter->second->second.EnclaveShapeMode = 2;//
		//PROMISEitermapITER->second->second.TotalRenderable = 3;
		//out_x = PROMISEitermap.find(loopkey)->second->second.UniqueKey.x;
		//tempint = PROMISEitermapITER->second->second.UniqueKey.x;
		//cout << "Job: " << tempcount << endl;
		//tempcount++;

	//}

	std::chrono::duration<double> elapsed4 = finish4 - start4;
	//promiseref->set_value(temp_itermapref);																					// sets the promise so that the calling thread has a return value.

	std::cout << "RUN MULTI JOB 2 (PROMISE) ELAPSED ITERATOR TIME2: " << elapsed4.count() << endl;
}