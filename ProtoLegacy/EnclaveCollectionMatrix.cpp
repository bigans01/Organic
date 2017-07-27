#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "EnclaveCollectionMatrix.h"
#include "PathTraceContainer.h"
#include "EnclaveCollection.h"
#include "EnclavePainterList.h"
#include "OrganicSystem.h"
#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <future>
#include <utility>
#include <vector>

class OrganicSystem;

typedef unsigned char(&ElevationMapRef)[8][8];												// forward declaration for return type below
void EnclaveCollectionMatrix::AddNewCollection(int x, int y, int z)
{
	/* Summary: Adds a new collection into the matrix, with the x/y/z values of the new collection's EnclaveKey being equal to the input parameters x/y/z.*/

	EnclaveKeyDef::EnclaveKey tempkey;			// create the temporary key
	tempkey.x = x;								// assign x to key
	tempkey.y = y;								// assign y to key
	tempkey.z = z;								// assign z to key
	auto start = std::chrono::high_resolution_clock::now();			// optional, for debugging
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int z = 0; z < 8; z++)
			{

				Enclave stackEnclave(tempkey, x, y, z);											// add an enclave, with a collection key value of tempkey.
				EnclaveCollectionMap[tempkey].EnclaveArray[x][y][z] = stackEnclave;				// copy the newly instantiated enclave onto the heap.
				EnclaveCollectionMap[tempkey].EnclaveArray[x][y][z].InitializeRenderArray();	// initialize contents of the newly heaped enclave.
			}
		}
	}
	auto finish = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed = finish - start;																// ""
	//std::cout << "Elapsed time (512 chunk instantiations: , " << elapsed.count() << "): " << elapsed.count() << endl;	// ""
}

void EnclaveCollectionMatrix::AddNewCollection(EnclaveKeyDef::EnclaveKey Key)
{
	/* Summary: Adds a new collection into the matrix, which will have a EnclaveKey value of the input value "Key" */
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int z = 0; z < 8; z++)
			{
				
				Enclave stackEnclave(Key, x, y, z);												// add an enclave, with a collection key of Key
				EnclaveCollectionMap[Key].EnclaveArray[x][y][z] = stackEnclave;					// copy the newly instantiated enclave onto the heap.
				EnclaveCollectionMap[Key].EnclaveArray[x][y][z].InitializeRenderArray();		// initialize contents of the newly heaped enclave.
			}
		}
	}

}

void EnclaveCollectionMatrix::AddNewCollectionWithBlueprint(EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint)
{
	/* Order of operations:
	1. Instantiate 512
	2. determine solids
	3. determine surfaces
	4. painting
	5. unveil polys
	6. smoothing
	7. attach to enclaves
	*/
	int chunkbitmask = 1;																				// set initial value of bitmask to be 128 (which is the top chunk)
	int chunkindex = 7;
	typedef unsigned char(&ElevationMapRef)[8][8];
	//ElevationMapRef solidChunk = blueprint.GetSolidChunkData[x][z];
	ElevationMapRef solidChunk = blueprint->GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef surfaceChunk = blueprint->GetSurfaceChunkData();
	ElevationMapRef paintableChunk = blueprint->GetPaintableChunkData();
	//int testout = 0;
	for (int x = 0; x < 8; x++)
	{
		chunkbitmask = 1;
		for (int y = 0; y < 8; y++)
		{
				
			for (int z = 0; z < 8; z++)
			{

				//ElevationMapRef solidChunk = blueprint->GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
				//ElevationMapRef surfaceChunk = blueprint->GetSurfaceChunkData();
				//ElevationMapRef paintableChunk = blueprint->GetPaintableChunkData();

				// step 2 begins here
				if ((solidChunk[x][z] & chunkbitmask) == chunkbitmask)								
				{

					//cout << x << " " << z << " " << chunkbitmask  << " HIT 1" << endl;
					Enclave stackEnclave(Key, x, y, z);												// add an enclave, with a collection key of Key
					EnclaveCollectionMap[Key].EnclaveArray[x][y][z] = stackEnclave;					// copy the newly instantiated enclave onto the heap.
					EnclaveCollection *collectionPtr = &EnclaveCollectionMap[Key];
					collectionPtr->EnclaveArray[x][y][z].InitializeRenderArray(1);		// 
			
					// step 3 begins here 
					if ((surfaceChunk[x][z] & chunkbitmask) == chunkbitmask)								
					{
						// do stuff
						//testout++;
						//cout << "test entrance: " << testout << endl;
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = y;
						tempKey.z = z;

						collectionPtr->ActivateEnclaveForRendering(tempKey);

																								
						// step 5 will go here ? 
						for (int xx2 = 0; xx2 < 4; xx2++)
						{
							for (int zz2 = 0; zz2 < 4; zz2++)
							{
									//collectionPtr->UnveilSinglePoly(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
								collectionPtr->EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePoly(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
							}
						}																			
						
					}

					// step 4 begins here
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)								
					{
						//cout << "Paintable chunk found! (" << x << ", " << y << ", " << z << ")" << endl;
						EnclavePainterList *painterListRef;
						Enclave *tempEnclave = &collectionPtr->EnclaveArray[x][y][z];				// this line is used to get the current Enclave's unique key
						//int keyToUse = KeyToSingle(tempEnclave->UniqueKey);
						painterListRef = &blueprint->PaintListMatrix.PainterListMatrix[tempEnclave->UniqueKey];		// gets the list of the paint jobs to run for this specific chunk
						std::unordered_map<int, EnclavePainter>::iterator paintListIter;
						paintListIter = painterListRef->PaintList.begin();
						for (paintListIter; paintListIter != painterListRef->PaintList.end(); paintListIter++)
						{
							//cout << "block type to be painted: " << paintListIter->first << endl; // testing
						}


					}

			



				}
				else
				{
					//cout << "HIT 2" << endl;
					Enclave stackEnclave(Key, x, y, z);												// add an enclave, with a collection key of Key
					EnclaveCollectionMap[Key].EnclaveArray[x][y][z] = stackEnclave;					// copy the newly instantiated enclave onto the heap.
					EnclaveCollection *collectionPtr = &EnclaveCollectionMap[Key];
					collectionPtr->EnclaveArray[x][y][z].InitializeRenderArray();		// initialize contents of the newly heaped enclave.
				}
			}
			chunkbitmask <<= 1;
		}
	}
}


void EnclaveCollectionMatrix::MultiAddNewCollectionWithBlueprint(int numThreads, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint)
{
	//EnclaveCollection newCollection;				// set up initial collection by declaring a single enclave
	auto start = std::chrono::high_resolution_clock::now();
	Enclave stackEnclave(Key, 0, 0, 0);												// add an enclave, with a collection key of Key
	EnclaveCollectionMap[Key].EnclaveArray[0][0][0] = stackEnclave;
	//EnclaveCollectionMap[Key] = newCollection;		// map new collection
	auto finish = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed = finish - start;																// ""
	//std::cout << "Elapsed time (multi thread dummy instantiation , " << elapsed.count() << "): " << elapsed.count() << endl;	// ""
	
	if (numThreads == 1)
	{
		thread_pool *tpref = OrganicPointer->getpool();
		std::packaged_task<void()> Job1(std::bind
		(
			&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
			this,
			0,												// 0, 
			7 + 1,												// 1 + 1, 
			std::ref(EnclaveCollectionMap[Key]),												// std::ref(EnclaveCollectionMap[Key])
			Key,												// Key
			std::ref(blueprint)														// std::ref(blueprint)
		)
		);
		std::future<void> testfuture1 = tpref->submit(std::move(Job1));
		testfuture1.get();

		//std::thread t0(std::move(Job1));
		//t0.join();
	}

	if (numThreads == 2)
	{
		// TRUE MULTITHREAD TEST BEGINS HERE.

		//JobInstantiateAndPopulateEnclave(0, 3 + 1,  EnclaveCollectionMap[Key], Key, blueprint);
		//JobInstantiateAndPopulateEnclave(4, 7, EnclaveCollectionMap[Key], Key, blueprint);
		//JobInstantiateAndPopulateEnclave(4, 7, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		auto start = std::chrono::high_resolution_clock::now();
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job1(std::bind
																																(
																																	&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																	this,
																																	std::placeholders::_1,												// 0, 
																																	std::placeholders::_2,												// 1 + 1, 
																																	std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																	std::placeholders::_4,												// Key
																																	std::placeholders::_5														// std::ref(blueprint)
																																)
																															);
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job2(std::bind
																																(
																																	&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																	this,
																																	std::placeholders::_1,												// 0, 
																																	std::placeholders::_2,												// 1 + 1, 
																																	std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																	std::placeholders::_4,												// Key
																																	std::placeholders::_5														// std::ref(blueprint)
																																)
																															);

		std::packaged_task<void()> Job3(std::bind
		(
			&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
			this,
			0,												// 0, 
			3 + 1,												// 1 + 1, 
			std::ref(EnclaveCollectionMap[Key]),												// std::ref(EnclaveCollectionMap[Key])
			Key,												// Key
			std::ref(blueprint)														// std::ref(blueprint)
		)
		);


		std::packaged_task<void()> Job4(std::bind
		(
			&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
			this,
			4,												// 0, 
			7+ 1,												// 1 + 1, 
			std::ref(EnclaveCollectionMap[Key]),												// std::ref(EnclaveCollectionMap[Key])
			Key,												// Key
			std::ref(blueprint)														// std::ref(blueprint)
		)
		);


		//std::thread t0(std::move(Job1), 0, 3 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		//t0.join();
		//cout << "FIRST JOB COMPLETE! " << endl;
		//std::thread t1(&EnclaveMultiJob::RunMultiJob2, EnclaveMultiJob(), start, end, std::ref(PrimeMatrix), std::ref(PROMISEitermap), &promiseref);
		// t1.join;
		//std::thread t1(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, EnclaveCollectionMatrix(), 4, 7 + 1,  std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		//t1.join();

		//std::thread t1(std::move(Job2), 4, 7 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		
		
		//std::thread t0(std::move(Job3));
		//t0.join();

		//std::thread t1(std::move(Job4));
		//t1.join();


		//cout << "SECOND JOB COMPLETE! " << endl;


		//std::future<void> testfuture2;
		//thread_pool *flermpone = &OrganicPointer->getpool();
		thread_pool *tpref = OrganicPointer->getpool();
		thread_pool *tpref2 = OrganicPointer->getpool2();
		//std::future<void> testfuture1 = tpref->submit(std::move(Job3));
		//std::future<void> testfuture1 = OrganicPointer->getpool().submit(std::move(Job3));

		//tpref->submit(std::move(Job3));
		//tpref->submit(std::move(Job4));

		auto jobstart = std::chrono::high_resolution_clock::now();
		std::future<void> testfuture1 = tpref->submit(std::move(Job3));
		
		
		std::future<void> testfuture2 = tpref2->submit(std::move(Job4));
		auto jobfinish = std::chrono::high_resolution_clock::now();
												// optional, for debugging
		std::chrono::duration<double> jobelapsed = jobfinish - jobstart;
		//cout << "job submit test2 (job 1): " << jobelapsed.count() << endl;

		//testfuture1.wait();
		//testfuture2.wait();

		auto start1 = std::chrono::high_resolution_clock::now();
		testfuture1.get();
		auto finish1 = std::chrono::high_resolution_clock::now();

		//auto start1 = std::chrono::high_resolution_clock::now();
		testfuture2.get();
				//// use placeholders with std::bind
		//auto finish1 = std::chrono::high_resolution_clock::now();															// optional, for debugging
		std::chrono::duration<double> elapsed1 = finish1 - start1;

		//cout << "future get test2 (): " << elapsed1.count() << endl;


		//OrganicPointer->getpool().submit(std::move(Job3));
		//std::future<void> testfuture2 = OrganicPointer->getpool->submit(std::move(Job4));

	}
	
	if (numThreads == 4)
	{
		/*	
		JobInstantiateAndPopulateEnclave(0, 7 + 1, EnclaveCollectionMap, EnclaveCollectionMap[Key], Key, blueprint);
		cout << "FIRST JOB COMPLETE! " << endl;
		*/

		// int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint

		//std::packaged_task<void(int, int, EnclaveCollection, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> task(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave);
		//std::packaged_task<void(int, int, EnclaveCollection, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> task(std::bind(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, this, 1 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint)));
		//task(0, 1 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));

		//std::packaged_task<int(int, int, int)> task(&EnclaveCollectionMatrix::TracePathToBlock);
		//std::packaged_task<int(int, int, int)> task( std::bind(&EnclaveCollectionMatrix::TracePathToBlock,3,3,3) );
		//vector<int> primesUpTo(int n);
		//std::packaged_task<vector<int>(int)> task(&primesUpTo);


		//std::packaged_task<void(int, int, EnclaveCollection, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> task(std::bind(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, this, 0, 1 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint)));
		
		//std::packaged_task<void(int, int)> task(std::bind(&EnclaveCollectionMatrix::testfunction, this, 0, 1));												//// ?????? what?????? This works...investigate std::bind
		auto start = std::chrono::high_resolution_clock::now();
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job1(std::bind
																																	(
																																		&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																		this, 
																																		std::placeholders::_1,												// 0, 
																																		std::placeholders::_2,												// 1 + 1, 
																																		std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																		std::placeholders::_4,												// Key
																																		std::placeholders::_5														// std::ref(blueprint)
																																	)	
																															);
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job2(std::bind
																																	(
																																		&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																		this,
																																		std::placeholders::_1,												// 0, 
																																		std::placeholders::_2,												// 1 + 1, 
																																		std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																		std::placeholders::_4,												// Key
																																		std::placeholders::_5														// std::ref(blueprint)
																																	)
																															);
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job3(std::bind
																																	(
																																		&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																		this,
																																		std::placeholders::_1,												// 0, 
																																		std::placeholders::_2,												// 1 + 1, 
																																		std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																		std::placeholders::_4,												// Key
																																		std::placeholders::_5														// std::ref(blueprint)
																																	)
																															);
		std::packaged_task<void(int, int, EnclaveCollection&, EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint*)> Job4(std::bind
																																	(
																																		&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave,			//// use placeholders with std::bind
																																		this,
																																		std::placeholders::_1,												// 0, 
																																		std::placeholders::_2,												// 1 + 1, 
																																		std::placeholders::_3,												// std::ref(EnclaveCollectionMap[Key])
																																		std::placeholders::_4,												// Key
																																		std::placeholders::_5														// std::ref(blueprint)
																																	)
																															);
		auto finish = std::chrono::high_resolution_clock::now();															// optional, for debugging
		//std::chrono::duration<double> elapsed = finish - start;																// ""
		//std::cout << "Elapsed time (Task instantiation , " << elapsed.count() << "): " << elapsed.count() << endl;	// ""
		//test
		//test
		//task(0,1);
		//task2(0, 1 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));


		//std::packaged_task<void()> task(testfunction);
		//std::packaged_task<int(int, int)> task([](int a, int b) {
		//	return std::pow(a, b);
		//});

		//std::packaged_task<int(int, int)> task(&f);
		//task(3, 3, 3);
		//task();


		//JobInstantiateAndPopulateEnclave(0, 1 + 1,  EnclaveCollectionMap[Key], Key, blueprint);
		//std::thread t0(std::move(task3));
		//std::thread t0(std::move(task));
		std::thread t0(std::move(Job1), 0, 1 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		t0.join();
		//cout << "FIRST JOB COMPLETE! " << endl;

		
		//std::thread t1(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, EnclaveCollectionMatrix(), 2, 3 + 1,  std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		std::thread t1(std::move(Job2), 2, 3 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		t1.join();
		//cout << "SECOND JOB COMPLETE! " << endl;


		//std::thread t2(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, EnclaveCollectionMatrix(), 4, 5 + 1,  std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		std::thread t2(std::move(Job3), 4, 5 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		t2.join();
		//cout << "THIRD JOB COMPLETE! " << endl;


		//std::thread t3(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave, EnclaveCollectionMatrix(), 6, 7 + 1,  std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint))
		std::thread t3(std::move(Job4), 6, 7 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		t3.join();
		//cout << "FOURTH JOB COMPLETE! " << endl;
		
	}
}

void EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclave(int beginRange, int endRange, EnclaveCollection &collectionRef, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint)
{
	/* Summary: this function performs enclave instantiations within a certain range; this "range" should be designed so that it 
	   can be used with a packaged_task
	*/

	/* Order of operations:
	1. Instantiate 512
	2. determine solids
	3. determine surfaces
	4. painting
	5. unveil polys
	6. smoothing
	7. attach to enclaves
	*/


	auto start = std::chrono::high_resolution_clock::now();			// option
	int chunkbitmask = 1;																				// set initial value of bitmask to be 128 (which is the top chunk)
	int chunkindex = 7;
	typedef unsigned char(&ElevationMapRef)[8][8];
	//ElevationMapRef solidChunk = blueprint.GetSolidChunkData[x][z];
	ElevationMapRef solidChunk = blueprint->GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef surfaceChunk = blueprint->GetSurfaceChunkData();
	ElevationMapRef paintableChunk = blueprint->GetPaintableChunkData();
	for (int x = beginRange; x < endRange; x++)
	{
		chunkbitmask = 1;
		for (int y = 0; y < 8; y++)
		{

			for (int z = 0; z < 8; z++)
			{
				if ((solidChunk[x][z] & chunkbitmask) == chunkbitmask)
				{

					//cout << x << " " << z << " " << chunkbitmask  << " HIT 1" << endl;
					Enclave stackEnclave(Key, x, y, z);											
					collectionRef.EnclaveArray[x][y][z] = stackEnclave;						
					collectionRef.EnclaveArray[x][y][z].InitializeRenderArray(1);				

					// step 3 begins here 
					if ((surfaceChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = y;
						tempKey.z = z;

						collectionRef.ActivateEnclaveForRendering(tempKey); // needs modification (7/26/2017)


						// step 5 will go here ? 
						for (int xx2 = 0; xx2 < 4; xx2++)
						{
							for (int zz2 = 0; zz2 < 4; zz2++)
							{
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePoly(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
							}
						}

					}

					// step 4 begins here
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						//cout << "Paintable chunk found! (" << x << ", " << y << ", " << z << ")" << endl;
						EnclavePainterList *painterListRef;
						Enclave *tempEnclave = &collectionRef.EnclaveArray[x][y][z];								// this line is used to get the current Enclave's unique key
						painterListRef = &blueprint->PaintListMatrix.PainterListMatrix[tempEnclave->UniqueKey];		// gets the list of the paint jobs to run for this specific chunk
						std::unordered_map<int, EnclavePainter>::iterator paintListIter;
						paintListIter = painterListRef->PaintList.begin();
						for (paintListIter; paintListIter != painterListRef->PaintList.end(); paintListIter++)
						{
							//cout << "block type to be painted: " << paintListIter->first << endl; // testing
						}


					}
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						for (int t = 0; t < 10000; t++)
						{

						}
					}



				}
				else
				{
					//cout << "HIT 2" << endl;
					Enclave stackEnclave(Key, x, y, z);												// add an enclave, with a collection key of Key
					collectionRef.EnclaveArray[x][y][z] = stackEnclave;					// copy the newly instantiated enclave onto the heap.
					//EnclaveCollection *collectionPtr = &collectionRef;
					collectionRef.EnclaveArray[x][y][z].InitializeRenderArray();		// initialize contents of the newly heaped enclave.
				}
			}
			chunkbitmask <<= 1;
		}
	}

	auto finish = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed = finish - start;																// ""
	//std::cout << "Elapsed time (multi-threaded enclave instantiation: " << elapsed.count() << endl;	// ""
}
Enclave& EnclaveCollectionMatrix::GetEnclaveFromCollection(EnclaveKeyDef::EnclaveKey Key, int x, int y, int z)
{
	/* Summary: returns a reference to an enclave at enclave coordinate xyz, within the collection specified by input value of "Key" */
	EnclaveKeyDef::EnclaveKey tempkey;										
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	return EnclaveCollectionMap[Key].EnclaveArray[x][y][z];				// fetch the Collection at Key, and return the enclave at x y z.
}

Enclave& EnclaveCollectionMatrix::GetEnclaveFromXYZ(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempkey;
	PathTraceContainer XPathTrace = GetCoordTrace(x);
	PathTraceContainer YPathTrace = GetCoordTrace(y);
	PathTraceContainer ZPathTrace = GetCoordTrace(z);
	tempkey.x = XPathTrace.CollectionCoord;
	tempkey.y = YPathTrace.CollectionCoord;
	tempkey.z = ZPathTrace.CollectionCoord;
	//cout << "X:: Collection: " << XPathTrace.CollectionCoord << ", " << XPathTrace.ChunkCoord << ", " << XPathTrace.BlockCoord << endl;
	return EnclaveCollectionMap[tempkey].EnclaveArray[XPathTrace.ChunkCoord][YPathTrace.ChunkCoord][ZPathTrace.ChunkCoord];
}

void EnclaveCollectionMatrix::TracePathToBlock(int x, int y, int z)
{
	/* Summary: outputs the xyz of a block at the world coordinate x/y/z, as well as the x/y/z of the collection it is found within, and the x/y/z of the chunk within the collection it is found in. */

	PathTraceContainer XPathTrace = GetCoordTrace(x);
	PathTraceContainer YPathTrace = GetCoordTrace(y);
	PathTraceContainer ZPathTrace = GetCoordTrace(z);

	//cout << "Path results for block: " << endl;
	//cout << "Collection XYZ = " << XPathTrace.CollectionCoord << ", " << YPathTrace.CollectionCoord << ", " << ZPathTrace.CollectionCoord << endl;
	//cout << "Chunk XYZ = " << XPathTrace.ChunkCoord << ", " << YPathTrace.ChunkCoord << ", " << ZPathTrace.ChunkCoord << endl;
	//cout << "Block XYZ = " << XPathTrace.BlockCoord << ", " << YPathTrace.BlockCoord << ", " << ZPathTrace.BlockCoord << endl;
	
}

PathTraceContainer EnclaveCollectionMatrix::GetCoordTrace(int x)
{
	/* Summary: returns a PathTraceContainer object, which contains either the x/y/z axis points of a collection, a chunk within the collection, and a block within chunk of said collection. */

	PathTraceContainer tempPathTrace;
	EnclaveKeyDef::EnclaveKey CollectionKey;
	CollectionKey.x = 0;
	CollectionKey.y = 0;
	CollectionKey.z = 0;
	//cout << "TracePath test: " << (x / 32) << endl;

	int x_divide = (x / 32);							// here, 32 is the length of an entire collection (8 chunks = 32 blocks width)
	int collection_x = 0;								// the coordinate of the collection that will be returned (x or y or z)
	int chunk_x = 0;									// the coordinate of the chunk that will be returned (x or y or z)
	int block_x = 0;									// the coordinate of the block that will be returned (x or y or z)

	if (x < 0)											// operations to be performed when the input value is less than 0.
	{
		collection_x = 0;
		if ((x_divide < 0) && x < -32)
		{
			if ((x % 32) != 0)
			{
				collection_x = (x_divide - 1);
			}
			else
			{
				collection_x = x_divide;
			}


		}
		else if ((x_divide < 0) && x >= -32)
		{
			collection_x = -1;
		}

		//cout << "collection_x: " << collection_x << endl;
		chunk_x = 7 - ((x % 32) / 4);
		//cout << "chunk_x: " << chunk_x << endl;
		block_x = ((x % 32) % 4);
		//cout << "block_x: " << block_x << endl;

	}
	else if (x > 0)										// operations t be performed when the input value is greater than/equal to 0
	{
		collection_x = 0;
		if ((x_divide > 0) && x > 32)
		{
			if ((x % 32) != 0)
			{
				collection_x = (x_divide + 1);
			}
			else
			{
				collection_x = x_divide;
			}
		}
		else if ((x_divide > 0) && x <= 32)
		{
			collection_x = 1;
		}
		//cout << "collection_x: " << collection_x << endl;
		chunk_x = ((x % 32) / 4);
		//cout << "chunk_x: " << chunk_x << endl;
		 block_x = ((x % 32) % 4);
		//cout << "block_x: " << block_x << endl;


		//cout << "NoOfCollections passed: " << NoOfCollections << endl;
	}
	tempPathTrace.CollectionCoord = collection_x;		// set the return value for the Collection coordinate
	tempPathTrace.ChunkCoord = chunk_x;					// set the return value for the Chunk coordinate
	tempPathTrace.BlockCoord = block_x;					// set the return value for the block coordinate
	return tempPathTrace;
}

ElevationMapRef& EnclaveCollectionMatrix::GetElevationMapFromCollection(EnclaveKeyDef::EnclaveKey InKey)
{
	/* Summary: returns a pointer to the ElevationMap for an associated EnclaveCollection having a value of InKey */
	return EnclaveCollectionMap[InKey].ElevationMap;
}

int EnclaveCollectionMatrix::KeyToSingle(EnclaveKeyDef::EnclaveKey InKey)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array

	int x = InKey.x * 64;
	int y = InKey.y * 16;
	int z = InKey.z;
	return x + y + z;
}

void EnclaveCollectionMatrix::testfunction(int beginRange,																			// this function is designed to be used with multi-threaded calls. See definition for more details.
	int endRange
	//EnclaveCollection &collectionRef,
	//EnclaveKeyDef::EnclaveKey Key,
	//EnclaveCollectionBlueprint *blueprint
)
{
	//return 5;
}

void EnclaveCollectionMatrix::SetOrganicSystem(OrganicSystem *organicRef)
{
	OrganicPointer = organicRef;
	cout << "test of ORGANICREF: " << organicRef->flermp << endl;
	

}