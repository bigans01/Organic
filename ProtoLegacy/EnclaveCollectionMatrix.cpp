#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "EnclaveCollectionMatrix.h"
#include "PathTraceContainer.h"
#include "EnclaveCollection.h"
#include "EnclavePainterList.h"
#include "EnclaveCollectionActivateList.h"
#include "EnclaveCollectionActivateListT2.h"
#include "OrganicSystem.h"
#include "thread_pool.h"
#include <chrono>
#include <thread>
#include <future>
#include <utility>
#include <vector>
#include "dummwrapper.h"

class OrganicSystem;

typedef unsigned char(&ElevationMapRef)[8][8];												// forward declaration for return type below.
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

void EnclaveCollectionMatrix::AddNewCollectionSkeleton(EnclaveKeyDef::EnclaveKey Key)
{
	Enclave stackEnclave(Key, 0, 0, 0);											// add an enclave, with a collection key value of tempkey.
	EnclaveCollectionMap[Key].EnclaveArray[0][0][0] = stackEnclave;
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
	

	auto truestart = std::chrono::high_resolution_clock::now();
	EnclaveCollectionMap[Key].totalRenderableEnclaves = 0;
	EnclaveCollection *collectionMapRef = &EnclaveCollectionMap[Key];
	auto truefinish = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> trueelapsed = truefinish - truestart;																// ""
	//std::cout << "Elapsed time (multi thread dummy instantiation , " << trueelapsed.count() << "): " << endl; //<< elapsed.count() << endl;	// ""
	
	auto prime_begin = std::chrono::high_resolution_clock::now();
	if (numThreads == 1)
	{
		//cout << "1 thread called to run..." << endl;
		thread_pool *tpref = OrganicPointer->getCell1();
		/*
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
		*/
		//EnclaveCollectionActivateListT2 listT2_1;
		//std::future<EnclaveCollectionActivateList> testfuture3 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveBeta,	this,0,	7 + 1,std::ref(EnclaveCollectionMap[Key]), Key,	std::ref(blueprint));
		//EnclaveCollectionActivateList tempReturnList = testfuture3.get();


		EnclaveCollectionActivateListT2 listT2_1;
		std::future<void> testfuture4 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha,	this,0,	7 + 1,std::ref(EnclaveCollectionMap[Key]),Key,std::ref(blueprint), std::ref(listT2_1));
		testfuture4.get();

		//for (int x = 0; x < tempReturnList.count; x++)			// populate from first list.
		//{
			//collectionRef.ActivateEnclaveForRendering(tempKey); // needs modification (7/26/2017)
		//	collectionMapRef->ActivateEnclaveForRendering(tempReturnList.activatables[x]);
		//}



		int chunkbitmask = 1;
		int bitmaskval = 0;
		for (int x = 0; x < 8; x++)
		{
			chunkbitmask = 1;
			bitmaskval = 0;
			for (int y = 0; y < 8; y++)
			{


				for (int z = 0; z < 8; z++)
				{
					if ((listT2_1.flagArray[x][z] & chunkbitmask) == chunkbitmask)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = bitmaskval;
						tempKey.z = z;
						collectionMapRef->ActivateEnclaveForRendering(tempKey);
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;
				bitmaskval++;

			}
		}

		////////////
		//EnclaveCollectionMatrix::
		//typedef typename std::result_of<FunctionType()>::type
		//std::result_of<EnclaveCollectionMatrix::dummyjob(int)>::type;
		
		/*
		std::packaged_task<int()> Job2(std::bind															/// WORKING INTS
		(
			&EnclaveCollectionMatrix::dummyjob2,
			this,
			2)
		);

		std::future<int> testval22 = tpref->submit3(std::move(Job2));
		int retKey = testval22.get();
		*/

		//std::packaged_task<EnclaveKeyDef::EnclaveKey()> Job6(std::bind
	//	(
		//	&EnclaveCollectionMatrix::dummyjob,			//// use placeholders with std::bind
		//	this
		//											// std::ref(blueprint)
		//)
		//);



		

		//std::future<EnclaveKeyDef::EnclaveKey> testval33 = tpref->submit4(std::move(Job6));
		//EnclaveKeyDef::EnclaveKey testval44 = testval33.get();
		//cout << "FUCKING FINALLY: " << testval44.x << " " << testval44.y << " " << testval44.z << endl;
		//std::future<EnclaveKeyDef::EnclaveKey> testval = Job2.get_future();
		//typename std::result_of<T(int)>::type;
		//std::future<int> testfuture2 = tpref->submit(std::move(Job2));
		//std::future<std::result_of<typename &EnclaveCollectionMatrix::dummyjob(int)>::type> testfuture2 = tpref->submit(std::move(Job2));
		//testfuture1.get();


		//std::thread t0(std::move(Job1));
		//t0.join();
	}

	if (numThreads == 2)
	{
		/*
		// TRUE MULTITHREAD TEST BEGINS HERE.
		//cout << "2 threads called to run..." << endl;
		//JobInstantiateAndPopulateEnclave(0, 3 + 1,  EnclaveCollectionMap[Key], Key, blueprint);
		//JobInstantiateAndPopulateEnclave(4, 7, EnclaveCollectionMap[Key], Key, blueprint);
		//JobInstantiateAndPopulateEnclave(4, 7, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		*/
		/*
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
		*/
		//auto jobstart = std::chrono::high_resolution_clock::now();
		thread_pool *tpref = OrganicPointer->getCell1();
		thread_pool *tpref2 = OrganicPointer->getCell2();
		//auto jobfinish = std::chrono::high_resolution_clock::now();
		// *********************************************************************************WORKING THREAD CODE*********************************************************************************
		//std::packaged_task<int(int)> Job7(std::bind
		//(
		//	&EnclaveCollectionMatrix::dummyjob2,			//// use placeholders with std::bind
		//	this,
		//	std::placeholders::_1
		//)
		//);	
		//std::future<int> testval33 = tpref->submit5(&EnclaveCollectionMatrix::dummyjob2, this, 5, 8);
		//auto start1 = std::chrono::high_resolution_clock::now();
		//std::future<EnclaveKeyDef::EnclaveKey> dumbmike = tpref->submit5(&EnclaveCollectionMatrix::truelydumb, this, 5, 10);	// returns an enclave key...
		//auto finish1 = std::chrono::high_resolution_clock::now();
		//int testval44 = testval33.get();
		//EnclaveKeyDef::EnclaveKey dumbmikesvalue = dumbmike.get();
		//cout << "FUCKING FINALLY: " << testval44 << "" << " YEAHHHH! "<< endl;
		//cout << "WOOOOO!!! " << dumbmikesvalue.x << ", " << dumbmikesvalue.y << ", " << dumbmikesvalue.z << " " << endl;
		// *********************************************************************************WORKING THREAD CODE*********************************************************************************	
		/*
		//tpref->submit6(&EnclaveCollectionMatrix::dummyjob2, this, 5);
		//std::future<void> testfuture1 = tpref->submit(std::move(Job3));
		//std::future<void> testfuture1 = OrganicPointer->getpool().submit(std::move(Job3));

		//tpref->submit(std::move(Job3));
		//tpref->submit(std::move(Job4));
		
		//list1;
		//EnclaveCollectionActivateList list2;
		*/
		EnclaveCollectionActivateListT2 listT2_1;
		EnclaveCollectionActivateListT2 listT2_2;
		/*
		//auto jobfinish = std::chrono::high_resolution_clock::now();
		//T2 listT2_3;
		//EnclaveCollectionActivateListT2 listT2_4;

		//EnclaveCollectionActivateListT2 listT2_5;
		//EnclaveCollectionActivateListT2 listT2_6;
		//auto jobfinish = std::chrono::high_resolution_clock::now();
		*/
		/*
		auto start1 = std::chrono::high_resolution_clock::now();
		std::future<EnclaveCollectionActivateList> testfuture1 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveBeta, this, 0,3 + 1,std::ref(EnclaveCollectionMap[Key]),Key,	std::ref(blueprint));
		std::future<EnclaveCollectionActivateList> testfuture2 = tpref2->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveBeta,this, 4, 7 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint));
		EnclaveCollectionActivateList returnedList1 = testfuture1.get();
		EnclaveCollectionActivateList returnedList2 = testfuture2.get();
		auto finish1 = std::chrono::high_resolution_clock::now();
		*/
		auto start1 = std::chrono::high_resolution_clock::now();
		std::future<void> testfuture3 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha, this, 0, 3 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint), std::ref(listT2_1));
		std::future<void> testfuture4 = tpref2->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha, this, 4, 7 + 1, std::ref(EnclaveCollectionMap[Key]), Key, std::ref(blueprint), std::ref(listT2_2));
		testfuture3.wait();
		testfuture4.wait();
		auto finish1 = std::chrono::high_resolution_clock::now();
		/*
		for (int x = 0; x < returnedList1.count; x++)			// populate from first list.
		{
			//collectionRef.ActivateEnclaveForRendering(tempKey); // needs modification (7/26/2017)
			collectionMapRef->ActivateEnclaveForRendering(returnedList1.activatables[x]);
		}

		for (int x = 0; x < returnedList2.count; x++)			// populate from first list.
		{
			//collectionRef.ActivateEnclaveForRendering(tempKey); // needs modification (7/26/2017)
			collectionMapRef->ActivateEnclaveForRendering(returnedList2.activatables[x]);
		}
		*/

		auto loopstart = std::chrono::high_resolution_clock::now();

		int chunkbitmask = 1;
		int bitmaskval = 0;
		for (int x = 0; x < 4; x++)
		{
			chunkbitmask = 1;
			bitmaskval = 0;
			for (int y = 0; y < 8; y++)
			{


				for (int z = 0; z < 8; z++)
				{
					if ((listT2_1.flagArray[x][z] & chunkbitmask)== chunkbitmask)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = bitmaskval;
						tempKey.z = z;
						collectionMapRef->ActivateEnclaveForRendering(tempKey);
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;
				bitmaskval++;
			
			}
		}

		int chunkbitmask2 = 1;
		int bitmaskval2 = 0;
		for (int x = 4; x < 8; x++)
		{
			chunkbitmask2 = 1;
			bitmaskval2 = 0;
			for (int y = 0; y < 8; y++)
			{


				for (int z = 0; z < 8; z++)
				{
					if ((listT2_2.flagArray[x][z] & chunkbitmask2)== chunkbitmask2)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = bitmaskval2;
						tempKey.z = z;
						collectionMapRef->ActivateEnclaveForRendering(tempKey);
						//cout << "value of key 2::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_2.flagArray[x][z]) << endl;
					}

				}
				chunkbitmask2 <<= 1;
				bitmaskval2++;
			}
		}
	
		//auto finish1 = std::chrono::high_resolution_clock::now();															// optional, for debugging
		auto loopend = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> elapsed2 = loopend - loopstart;
		//cout << "NEW Loop time: " << elapsed2.count() << endl;
		std::chrono::duration<double> elapsed1 = finish1 - start1;
		//cout << "Future get time: " << elapsed1.count() << endl;
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
	auto prime_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> total_elapsed = prime_end - prime_begin;

	//cout << "True time used:  " << total_elapsed.count() << endl;
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

	EnclaveCollectionActivateList tempList;
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
	//return tempList;
}

EnclaveCollectionActivateList EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveBeta(int beginRange, int endRange, EnclaveCollection &collectionRef, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint)
{
	/************ THIS FUNCTION IS CURRENTLY UNUSED ************/

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

	EnclaveCollectionActivateList tempList;
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

						//cout << "whoa boy its" <<  sizeof(EnclaveKeyDef::EnclaveKey) << endl;
						//collectionRef.ActivateEnclaveForRendering(tempKey); // needs modification (7/26/2017)

						tempList.activatables[tempList.count++] = tempKey;



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

					
					/*
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						for (int t = 0; t < 500000; t++)
						{

						}
					}
					*/


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

	auto finish = std::chrono::high_resolution_clock::now();															
	std::chrono::duration<double> elapsed = finish - start;																
	std::cout << "Elapsed time (multi-threaded enclave instantiation: " << elapsed.count() << endl;	// ""
	return tempList;
}

void EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha(int beginRange,	int endRange, EnclaveCollection &collectionRef, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint, EnclaveCollectionActivateListT2 &activateListRef)
{
	/* Summary: this function performs enclave instantiations within a certain range; this "range" should be designed so that it
	can be used with a packaged_task. It takes in the parameter, "activateListRef" as a pointer to an instance of 
	EnclaveCollectionActivateListT2 -- where it will be the only thread writing to this instance; this instance will contain a list
	of Enclaves to be renedered for a particular EnclaveCollection.
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
	int totalchunks = 0;
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
					
					// step 4 begins here
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						//cout << "Paintable chunk found! (" << x << ", " << y << ", " << z << ")" << endl;
						EnclavePainterList *painterListRef;
						Enclave *tempEnclave = &collectionRef.EnclaveArray[x][y][z];								// this line is used to get the current Enclave's unique key
						painterListRef = &blueprint->PaintListMatrix.PainterListMatrix[tempEnclave->UniqueKey];		// gets the list of the paint jobs to run for this specific chunk
						std::unordered_map<int, EnclavePainter>::iterator paintListIter;
						paintListIter = painterListRef->PaintList.begin();
						int currentblockindex = 0;				// determines the "single" value of the current block we are looking at (so that we can convert it to x/y/z in the chunk below)
						int currentbyte = 0;					// determine the current byte to be working on
						int paintedcount = 0;
						int chunkbitmask2 = 1;					// bitmask that is shifted to the left
						
						auto teststart = std::chrono::high_resolution_clock::now();

						
						for (paintListIter; paintListIter != painterListRef->PaintList.end(); paintListIter++)
						{
							paintedcount = 0;
							currentblockindex = 0;				// reset the currentblockindex
							EnclavePainter painterCopy = paintListIter->second;
							//cout << "block type to be painted: " << paintListIter->first << endl; // testing
							for (int a = 0; a < 4; a++)			// loop 4 times; do 2 bytes per loop
							{
								for (int b = 0; b < 2; b++)		// for each iteration of inner loop, check 
								{
									chunkbitmask2 = 1;					// reset the chunk bit mask
									for (int c = 0; c < 8; c++)
									{
										//cout << "value of current byte: (" << currentbyte << ") " <<  int(paintListIter->second.PaintData[currentbyte]) << endl;
										if (   (int(paintListIter->second.PaintData[currentbyte]) & chunkbitmask2) == chunkbitmask2)
										{
											EnclaveKeyDef::EnclaveKey singleToXYZresult = tempEnclave->SingleToEnclaveKey(currentblockindex);
											tempEnclave->ChangePolyMaterial(singleToXYZresult.x, singleToXYZresult.y, singleToXYZresult.z, paintListIter->first);
											//cout << "material changed... (bitmask: " << chunkbitmask2 <<", " << b << ")" << endl;
											paintedcount++;
										}
										chunkbitmask2 <<= 1;
										//cout << chunkbitmask2 << endl;
										currentblockindex++;
									}
									currentbyte++;
								}

							}
							//cout << "total painted: " << paintedcount << endl;
						}
						//cout << endl;
						auto testend = std::chrono::high_resolution_clock::now();
						std::chrono::duration<double> elapsed = testend - teststart;
						//std::cout << "Elapsed time, painting: " << elapsed.count() << endl;	// ""
						
						
						totalchunks++;
					}

					
					// step 3 begins here 
					if ((surfaceChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = y;
						tempKey.z = z;

						activateListRef.flagArray[x][z] = chunkbitmask;

						//cout << "cool..." << endl;

						// step 5 will go here ? 
						for (int xx2 = 0; xx2 < 4; xx2++)
						{
							for (int zz2 = 0; zz2 < 4; zz2++)
							{
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePoly(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
							}
						}

					}

					

					//for (int x = 0; x < 100000; x++)
					//{

					//}




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

	//cout << "total chunks: " << totalchunks << endl;
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	//std::cout << "Elapsed time (multi-threaded enclave instantiation: " << elapsed.count() << endl;	// ""
}

void EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha2(int beginRange,																		// this function is designed for multithreading; it will return a list of enclaves that need to be rendered as a result
	int endRange,																		// of this job being run. It is the defacto job to call when dealing with populating/instantiating Enclaves	
	EnclaveCollection &collectionRef,													// as part of a multithreaded operation.
	EnclaveKeyDef::EnclaveKey Key,
	EnclaveCollectionBlueprint blueprint,
	EnclaveCollectionActivateListT2 &activateListRef)
{
	/* Summary: this function performs enclave instantiations within a certain range; this "range" should be designed so that it
	can be used with a packaged_task. It takes in the parameter, "activateListRef" as a pointer to an instance of
	EnclaveCollectionActivateListT2 -- where it will be the only thread writing to this instance; this instance will contain a list
	of Enclaves to be renedered for a particular EnclaveCollection.
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
	ElevationMapRef solidChunk = blueprint.GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef surfaceChunk = blueprint.GetSurfaceChunkData();
	ElevationMapRef paintableChunk = blueprint.GetPaintableChunkData();
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

					// step 4 begins here
					if ((paintableChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						//cout << "Paintable chunk found! (" << x << ", " << y << ", " << z << ")" << endl;
						EnclavePainterList *painterListRef;
						Enclave *tempEnclave = &collectionRef.EnclaveArray[x][y][z];								// this line is used to get the current Enclave's unique key
						painterListRef = &blueprint.PaintListMatrix.PainterListMatrix[tempEnclave->UniqueKey];		// gets the list of the paint jobs to run for this specific chunk
						std::unordered_map<int, EnclavePainter>::iterator paintListIter;
						paintListIter = painterListRef->PaintList.begin();
						int currentblockindex = 0;				// determines the "single" value of the current block we are looking at (so that we can convert it to x/y/z in the chunk below)
						int currentbyte = 0;					// determine the current byte to be working on
						int paintedcount = 0;
						int chunkbitmask2 = 1;					// bitmask that is shifted to the left

						auto teststart = std::chrono::high_resolution_clock::now();


						for (paintListIter; paintListIter != painterListRef->PaintList.end(); paintListIter++)
						{
							paintedcount = 0;
							currentblockindex = 0;				// reset the currentblockindex
							EnclavePainter painterCopy = paintListIter->second;
							//cout << "block type to be painted: " << paintListIter->first << endl; // testing
							for (int a = 0; a < 4; a++)			// loop 4 times; do 2 bytes per loop
							{
								for (int b = 0; b < 2; b++)		// for each iteration of inner loop, check 
								{
									chunkbitmask2 = 1;					// reset the chunk bit mask
									for (int c = 0; c < 8; c++)
									{
										//cout << "value of current byte: (" << currentbyte << ") " <<  int(paintListIter->second.PaintData[currentbyte]) << endl;
										if ((int(paintListIter->second.PaintData[currentbyte]) & chunkbitmask2) == chunkbitmask2)
										{
											EnclaveKeyDef::EnclaveKey singleToXYZresult = tempEnclave->SingleToEnclaveKey(currentblockindex);
											tempEnclave->ChangePolyMaterial(singleToXYZresult.x, singleToXYZresult.y, singleToXYZresult.z, paintListIter->first);
											//cout << "material changed... (bitmask: " << chunkbitmask2 <<", " << b << ")" << endl;
											paintedcount++;
										}
										chunkbitmask2 <<= 1;
										//cout << chunkbitmask2 << endl;
										currentblockindex++;
									}
									currentbyte++;
								}

							}
							//cout << "total painted: " << paintedcount << endl;
						}
						//cout << endl;
						auto testend = std::chrono::high_resolution_clock::now();
						std::chrono::duration<double> elapsed = testend - teststart;
						//std::cout << "Elapsed time, painting: " << elapsed.count() << endl;	// ""



					}

					// step 3 begins here 
					if ((surfaceChunk[x][z] & chunkbitmask) == chunkbitmask)
					{
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = y;
						tempKey.z = z;

						activateListRef.flagArray[x][z] = chunkbitmask;

						//cout << "cool..." << endl;

						// step 5 will go here ? 
						for (int xx2 = 0; xx2 < 4; xx2++)
						{
							for (int zz2 = 0; zz2 < 4; zz2++)
							{
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePoly(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
							}
						}

					}



					//for (int x = 0; x < 100000; x++)
					//{

					//}




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

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
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



void EnclaveCollectionMatrix::SetOrganicSystem(OrganicSystem *organicRef)
{
	OrganicPointer = organicRef;
	//cout << "test of ORGANICREF: " << organicRef->flermp << endl;
	

}

