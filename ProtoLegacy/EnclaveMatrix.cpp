#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <thread>
#include "Enclave.h"
#include "EnclaveMatrix.h"
#include "EnclaveMultiJob.h"



void EnclaveMatrix::AddEnclaveToMatrix(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;

	Enclave tempenclave(x, y, z);
	PrimeMatrix[tempkey] = tempenclave;
	//cout << x << " " << y << " " << z << endl;

	//cout << PrimeMatrix[tempkey].UniqueKey.x << " " << PrimeMatrix[tempkey].UniqueKey.y << " " << PrimeMatrix[tempkey].UniqueKey.z << endl;
	PrimeMatrix[tempkey].InitializeRenderArray();
	//PrimeMatrix[tempkey].TestTopLayer();
	PrimeMatrix[tempkey].SortRenderArray();
}

void EnclaveMatrix::AllocateWorldEnclaveMatrixToHeap()
{

}

void EnclaveMatrix::DeallocateWorldEnclaveMatrixFromHeap()
{

}


void EnclaveMatrix::ClearEnclaveFromMatrix(EnclaveKeyDef::EnclaveKey inkey)
{
	/*for (m_iter = m.begin(); m_iter != m.end(); ++m_iter)
	{
	cout << "value: 5" << endl;
	}*/

	std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_iter = PrimeMatrix.find(inkey);
	cout << "pre-erase" << endl;
	if (m_iter != PrimeMatrix.end())									// safety check
	{
		PrimeMatrix.erase(m_iter++);
		cout << "erased " << endl;
	}

}


void EnclaveMatrix::ClearAllEnclavesFromMatrix()
{
	PrimeMatrix.erase(PrimeMatrix.begin(), PrimeMatrix.end());
}

void EnclaveMatrix::SetRenderFlag(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_iter = PrimeMatrix.find(tempkey);
	m_iter= PrimeMatrix.find(tempkey);
	//PrimeMatrix[tempkey].EnclaveShapeMode=2;
	m_iter->second.EnclaveShapeMode = 2;
}

void EnclaveMatrix::SetRenderFlag(EnclaveKeyDef::EnclaveKey inkey)
{
	m_iter = PrimeMatrix.find(inkey);
	m_iter->second.EnclaveShapeMode = 2;
}

void EnclaveMatrix::MultiSetRenderFlag(int start, int end, std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> &m_itermap1)
{
	//std::thread t1(&EnclaveMultiJob::RunMultiJob, EnclaveMultiJob(), start, end, PrimeMatrix, m_itermap1);
	//t1.join();
	testmulti.RunMultiJob(start, end, std::ref(PrimeMatrix), std::ref(m_itermap1));

}

void EnclaveMatrix::MultiSetRenderFlag2(
	int start, 
	int end, 
	std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> *m_itermap1, 
	std::promise<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> &promiseref,
	std::future<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> &future1)
	{
		// promise testing
		//std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher
		std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> PROMISEitermap;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator PROMISEitermapITER;
		//std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator PROMISEitermapITER2;
		EnclaveKeyDef::EnclaveKey loopkey;
		loopkey.x = 100;
		loopkey.y = 0;
		loopkey.z = 0;
		int testint = 0;
		std::thread t1(&EnclaveMultiJob::RunMultiJob2, EnclaveMultiJob(), start, end, std::ref(PrimeMatrix), std::ref(PROMISEitermap), &promiseref);
		//m_itermap1 = &future1.get();
		//PROMISEitermap = future1.get();
		//m_itermap1 = &PROMISEitermap;

	



		//std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator itermaptest;
		//for (itermaptest = m_itermap1.begin(); itermaptest != m_itermap1.end(); itermaptest++)
		//{
		
		//}


		//int out_x = m_itermap1->find(loopkey)->second->second.UniqueKey.x;
		//int out_y = m_itermap1->find(loopkey)->second->second.UniqueKey.y;
		//int out_z = m_itermap1->find(loopkey)->second->second.UniqueKey.z;

		//cout << "PROMISE KEPT: " << out_x << " " << out_y << " " << out_z << endl;

		//int out_x = PROMISEitermap.at(loopkey)->second->second.UniqueKey.x;
		//out_x = PROMISEitermap.at(loopkey)->second->second.UniqueKey.x;
		//int testx2 = PROMISEitermap.at(loopkey)->second.EnclaveShapeMode;
		//testx2 = PROMISEitermap.at(loopkey)->second.EnclaveShapeMode;
		//int out_x2 = m_itermap1->find(loopkey)->second->second.UniqueKey.x;
		//cout << endl <<  "PROMISE KEPT: " << testx2 << " " << out_x2 << endl;


		t1.join();

		PROMISEitermapITER = PROMISEitermap.begin();
		//int tempcrapint = m_itermap1->begin()->second->second.UniqueKey.x;
		
		//PROMISEitermapITER->second->second.UniqueKey.x;


		//cout << "PROMISE KEPT: " << testx2 << " " << out_x2 << endl;
		int tempcount = 0;
		int tempint = 0;

		auto start4 = std::chrono::high_resolution_clock::now();
		//for (int z = 0; z < 10; z++)
		//{
			for (PROMISEitermapITER = PROMISEitermap.begin(); PROMISEitermapITER != PROMISEitermap.end(); PROMISEitermapITER++)
			{
				PROMISEitermapITER->second->second.EnclaveShapeMode = 2;//
				//PROMISEitermapITER->second->second.TotalRenderable = 3;
				//out_x = PROMISEitermap.find(loopkey)->second->second.UniqueKey.x;
				//tempint = PROMISEitermapITER->second->second.UniqueKey.x;
				//cout << tempcount << endl;
				tempcount++;

			}
		//}

		
		int testvar = 0;
		auto finish4 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed4 = finish4 - start4;
		cout << "Elapsed time:  Multithread result, POST iterator map traversal:"  << elapsed4.count() << "::" << testvar << endl;



		cout << "PROMISE temp count: " << tempcount << endl;



	}

Enclave& EnclaveMatrix::GetEnclaveFromMatrix(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempkey;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	return PrimeMatrix[tempkey];
}

EnclaveMatrix::~EnclaveMatrix()
{
	cout << "EnclaveMatrix destructor called..." << endl;
}