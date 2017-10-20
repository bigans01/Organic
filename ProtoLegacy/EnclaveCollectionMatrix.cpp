#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "EnclaveCollectionMatrix.h"
#include "PathTraceContainer.h"
#include "EnclaveCollection.h"
#include "EnclavePainterList.h"
#include "EnclaveCollectionActivateListT2.h"
#include "EnclaveCollectionNeighborList.h"
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
	ElevationMapRef paintableChunk = blueprint->GetCustomPaintableChunkData();
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
				//ElevationMapRef paintableChunk = blueprint->GetCustomPaintableChunkData();

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
									//collectionPtr->UnveilSinglePolyWithMtrl(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
								collectionPtr->EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePolyWithMtrl(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
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


void EnclaveCollectionMatrix::MultiAddNewCollectionWithBlueprint(int numThreads, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprintMatrix* blueprintmatrixptr, EnclaveCollectionBlueprint* blueprint)
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
		//std::future<void> testfuture4 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha,	this,0,	7 + 1,std::ref(EnclaveCollectionMap[Key]),Key,std::ref(blueprint), std::ref(listT2_1));
		//std::future<void> testfuture4 = tpref->submit5(&EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha2,this,0,	7 + 1,std::ref(EnclaveCollectionMap[Key]),Key,std::ref(blueprint), std::ref(blueprintmatrixptr), std::ref(listT2_1));
		//testfuture4.get();

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
						// cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
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

	//EnclaveCollectionActivateList tempList;
	auto start = std::chrono::high_resolution_clock::now();			// option
	int chunkbitmask = 1;																				// set initial value of bitmask to be 128 (which is the top chunk)
	int chunkindex = 7;
	typedef unsigned char(&ElevationMapRef)[8][8];
	//ElevationMapRef solidChunk = blueprint.GetSolidChunkData[x][z];
	ElevationMapRef solidChunk = blueprint->GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef surfaceChunk = blueprint->GetSurfaceChunkData();
	ElevationMapRef paintableChunk = blueprint->GetCustomPaintableChunkData();
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
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePolyWithMtrl(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
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

void EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveBeta(int beginRange, int endRange, EnclaveCollection &collectionRef, EnclaveKeyDef::EnclaveKey Key, EnclaveCollectionBlueprint *blueprint)
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

	//EnclaveCollectionActivateList tempList;
	auto start = std::chrono::high_resolution_clock::now();			// option
	int chunkbitmask = 1;																				// set initial value of bitmask to be 128 (which is the top chunk)
	int chunkindex = 7;
	typedef unsigned char(&ElevationMapRef)[8][8];
	//ElevationMapRef solidChunk = blueprint.GetSolidChunkData[x][z];
	ElevationMapRef solidChunk = blueprint->GetSolidChunkData();						// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef surfaceChunk = blueprint->GetSurfaceChunkData();
	ElevationMapRef paintableChunk = blueprint->GetCustomPaintableChunkData();
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

						//tempList.activatables[tempList.count++] = tempKey;



																			// step 5 will go here ? 
						for (int xx2 = 0; xx2 < 4; xx2++)
						{
							for (int zz2 = 0; zz2 < 4; zz2++)
							{
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePolyWithMtrl(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
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
	//return tempList;
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
	ElevationMapRef paintableChunk = blueprint->GetCustomPaintableChunkData();
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
								collectionRef.EnclaveArray[tempKey.x][tempKey.y][tempKey.z].UnveilSinglePolyWithMtrl(xx2, 3, zz2, 0, 1, 0, 2, 0);	// STEP 3b: get the top faces, set the top face bit (2) to 1. 
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

void EnclaveCollectionMatrix::JobInstantiateAndPopulateEnclaveAlpha2(int beginRange,	// this function is designed for multithreading; it will return a list of enclaves that need to be rendered as a result
	int endRange,																		// of this job being run. It is the defacto job to call when dealing with populating/instantiating Enclaves	
	EnclaveCollection &collectionRef,													// as part of a multithreaded operation.
	EnclaveKeyDef::EnclaveKey Key,
	EnclaveCollectionBlueprint* blueprint,
	EnclaveCollectionBlueprintMatrix* blueprintmatrix,
	EnclaveCollectionActivateListT2& activateListRef,
	mutex& HeapMutex)
{
	auto start = std::chrono::high_resolution_clock::now();			
	int chunkbitmask = 1;																				// set initial value of bitmask to be 128 (which is the top chunk)
	int stdchunkbitmask = 1;
	int chunkindex = 7;
	typedef unsigned char(&ElevationMapRef)[8][8];
	ElevationMapRef solidChunk = blueprint->GetSolidChunkData();							// ?? better optimized? unknown. compare to declaring outside of loop (7/18/2017)
	ElevationMapRef customPaintableChunk = blueprint->GetCustomPaintableChunkData();		// custom chunk data
	ElevationMapRef standardPaintableChunk = blueprint->GetStandardPaintableChunkData();	// standard chunk data

	// Step One: perform collection-wide painting (required)
	collectionRef.RunCollectionPainters(blueprint);
	EnclaveCollection* collectionRefPtr = &collectionRef;

	// Step Two: determine what borders of this blueprint must be rendered, by comparing to borders in neighboring blueprints
	EnclaveCollectionBorderFlags borderFlags;											// contains west, north, east, south, top, bottom flags. 
	EnclaveCollectionBorderFlags* borderFlagsRef = &borderFlags;						// get pointer to borderFlags
	EnclaveCollectionNeighborList neighborList = blueprintmatrix->DetermineBlueprintBordersToRender(Key, blueprint, borderFlagsRef, 0);	// check this blueprint's neighbors; return a list that contains the appropriate pointers (so that we don't have to contantly 
																																		// look up the results)



	// Step Three: prepare all solid chunks
	for (int x = beginRange; x < endRange; x++)
	{
		int actualmask = 0;
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
					collectionRef.EnclaveArray[x][y][z].InitializeRenderArray(1);				// setup this solid enclave
					//cout << "Enclave set up complete: " << x << ", " << y << ", " << z << endl;
					//if (x == 7 && actualmask == 7 && z == 0)
					//{
						//cout << "test call" << endl;
					//}
					
				}
			}
			chunkbitmask <<= 1;
			actualmask++;
		}
	}

	// Step Four: unveil all polys in border chunks	
	// a flag of one indicates that there is no bordering collection on that side (i.e., West = 1 means there is no collection to render that is West of this collection)
	if (borderFlags.West == 1)
	{
		collectionRef.SetWestBorder(standardPaintableChunk, activateListRef);		// set up west border -- using the standardPaintableChunk; 
	}
	
	
	if (borderFlags.North == 1)
	{
		collectionRef.SetNorthBorder(standardPaintableChunk, activateListRef);		// set up north border -- using the standardPaintableChunk; 
	}
	
	if (borderFlags.East == 1)
	{
		collectionRef.SetEastBorder(standardPaintableChunk, activateListRef);
	}
	if (borderFlags.South == 1)
	{
		collectionRef.SetSouthBorder(standardPaintableChunk,  activateListRef);
	}


	//...but if the flag is 0 (meaning there is a neighbor), each border chunk must be compared to the opposite border chunk in the neighbor.
	if (borderFlags.West == 0)
	{
		EnclaveCollectionNeighborList* neighborListRef = &neighborList;
		collectionRef.SetWestBorder(standardPaintableChunk, activateListRef, neighborListRef);
	}

	if (borderFlags.North == 0)
	{
		EnclaveCollectionNeighborList* neighborListRef = &neighborList;
		collectionRef.SetNorthBorder(standardPaintableChunk, activateListRef, neighborListRef);
	}

	if (borderFlags.East == 0)
	{
		EnclaveCollectionNeighborList* neighborListRef = &neighborList;
		collectionRef.SetEastBorder(standardPaintableChunk, activateListRef, neighborListRef);
	}

	if (borderFlags.South == 0)
	{
		EnclaveCollectionNeighborList* neighborListRef = &neighborList;
		collectionRef.SetSouthBorder(standardPaintableChunk, activateListRef, neighborListRef);
	}



	// Step Five: find paintable chunks and determine the faces for the paintable blocks (which is later passed to UnveilPoly)
	for (int x = beginRange; x < endRange; x++)
	{
		chunkbitmask = 1;
		for (int y = 0; y < 8; y++)
		{
			for (int z = 0; z < 8; z++)
			{
				// Render customized chunks here
				if ((customPaintableChunk[x][z] & chunkbitmask) == chunkbitmask)
				{
					EnclaveKeyDef::EnclaveKey currentKey;
					currentKey.x = x;
					currentKey.y = y;
					currentKey.z = z;
					// cout << "custom key value: " << currentKey.x << ", " << currentKey.y << ", " << currentKey.z << endl;
					EnclaveUnveilMeta currentMeta = blueprint->SetupCarvePlan(currentKey);		// use the carve plan to determine the exact x/y/z chunk coords of each block to render
					collectionRef.EnclaveArray[x][y][z].UnveilMultipleAndNotifyNeighbors(currentMeta, borderFlagsRef, customPaintableChunk, collectionRefPtr, neighborList, 0); // 0 = go -y direction
					activateListRef.flagArray[x][z] = activateListRef.flagArray[x][z] | chunkbitmask;
					/*
					if (currentKey.x == 6 && currentKey.y == 7 && currentKey.z == 0)	// testing only
					{
						for (int x = 0; x < 4; x++)
						{
							for (int z = 0; z < 4; z++)
							{
								cout << "currentMeta["<< x << "][" << z << "] :" << int(currentMeta.EnclaveBlockLocation[x][z]) << endl;
							}
						}
					}
					*/
				}
			}
			chunkbitmask <<= 1;
		}
	}

	auto finish = std::chrono::high_resolution_clock::now();

	// Step Six: apply any blocks that need to be veiled


	//std::chrono::duration<double> elapsed = finish - start;
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

PathTraceContainer EnclaveCollectionMatrix::GetCoordTrace(float x)
{
	/* Summary: returns a PathTraceContainer object, which contains either the x/y/z axis points of a collection, a chunk within the collection, and a block within chunk of said collection. */

	PathTraceContainer tempPathTrace;
	EnclaveKeyDef::EnclaveKey CollectionKey;
	CollectionKey.x = 0;
	CollectionKey.y = 0;
	CollectionKey.z = 0;
	//cout << "TracePath test: " << (x / 32) << endl;

	float x_divide = (x / 32);							// here, 32 is the length of an entire collection (8 chunks = 32 blocks width)
	int collection_x = 0;								// the coordinate of the collection that will be returned (x or y or z)
	int chunk_x = 0;									// the coordinate of the chunk that will be returned (x or y or z)
	int block_x = 0;									// the coordinate of the block that will be returned (x or y or z)

	if (x < 0)											// operations to be performed when the input value is less than 0.
	{
		cout << ">>>>X_DIVIDE ENTRY; x_divide is  " << endl;
		collection_x = 0;
		if ((x_divide < 0) && x < -32)
		{
			
			if (fmod(x,32) != 0)
			{
				collection_x = -1;  // collection_x = (x_divide - 1);
			}
			//else
			//{
			//collection_x = x_divide;
			//}


		}
		else if ((x_divide < 0) && x >= -32)
		{
			collection_x = int(x_divide) - 1;
		}

		//cout << "collection_x: " << collection_x << endl;
		chunk_x = 7 - abs(int(fmod(x, 32) / 4));
		//cout << "chunk_x: " << chunk_x << endl;
		block_x = 3 - abs(fmod(ceil(fmod(x, 32)), 4));
		//cout << "block_x: " << block_x << endl;

	}
	else if (x > 0)										// operations t be performed when the input value is greater than/equal to 0
	{
		collection_x = 0;
		if ((x_divide > 0) && x > 32)
		{
			if (fmod(x, 32) != 0)
			{
				collection_x = x_divide;  // collection_x = (x_divide + 1);
			}
			//else
			//{
			//collection_x = x_divide;
			//}
		}
		//else if ((x_divide > 0) && x <= 32)
		//{
		//collection_x = 1;
		//}
		//cout << "collection_x: " << collection_x << endl;
		chunk_x = (fmod(x, 32) / 4);					// old:  chunk_x = ((x % 32) / 4);
														//cout << "chunk_x: " << chunk_x << endl;
		block_x = (fmod(fmod(x, 32), 4));				// old : block_x = ((x % 32) % 4);
	}
	tempPathTrace.CollectionCoord = collection_x;		// set the return value for the Collection coordinate
	tempPathTrace.ChunkCoord = chunk_x;					// set the return value for the Chunk coordinate
	tempPathTrace.BlockCoord = block_x;					// set the return value for the block coordinate
	return tempPathTrace;
}

CursorPathTraceContainer EnclaveCollectionMatrix::GetCursorCoordTrace(float x)
{
	CursorPathTraceContainer tempPathTrace;
	float x_divide = (x / 32);							// here, 32 is the length of an entire collection (8 chunks = 32 blocks width)
	int collection_x = 0;								// the coordinate of the collection that will be returned (x or y or z)
	int chunk_x = 0;									// the coordinate of the chunk that will be returned (x or y or z)
	int block_x = 0;									// the coordinate of the block that will be returned (x or y or z)
	float dist_to_pos = 0;								// distance from the camera's point to the nearest positive axis block border
	float dist_to_neg = 0;								// distance from the camera's point to the nearest negative axis block border
	float exact_xyz = 0;

	if (x < 0)											// operations to be performed when the input value is less than 0.
	{
		//cout << "----negative coordinate detected----" << endl;
		//cout << "x_divide result: " << x_divide << endl;
		collection_x = 0;
		if ((x_divide < 0) && x >= -32)
		{
			//cout << "x_divide entry. " << endl;
			if (fmod(x, 32) != 0)
			{
				collection_x = -1;  // collection_x = (x_divide - 1);
			}
			//else
			//{
				//collection_x = x_divide;
			//}


		}
		else if ((x_divide < 0) && x < -32)
		{
			collection_x = int(x_divide) -1;
		}

		//cout << "collection_x: " << collection_x << endl;
		//cout << "ABS: " << abs(int(fmod(x, 32) / 4)) << endl;
		//cout << "ABS 2: " <<  floor(fmod(fmod(x, 32), 4)) << endl;
		//cout << "ABS block: " << abs(fmod(ceil(fmod(x, 32)), 4)) << endl;
		//cout << "Dist to pos a: " << fmod(fmod(x, 32), 4) << endl;
		chunk_x = 7 - abs(int(fmod(x, 32) / 4));				// old:  chunk_x = ((x % 32) / 4);
		//cout << "chunk_x: " << chunk_x << endl;
		block_x = 3 - abs(fmod(ceil(fmod(x, 32)), 4));									//block_x = abs(fmod(fmod(x, 32), 4));				// old : block_x = ((x % 32) % 4);
		//cout << "block_x: " << block_x << endl;
		float enclaveBorderCoord = abs(fmod(floor(fmod(x, 32)), 4));


		dist_to_pos = abs((fmod(fmod(x, 32), 4)) + abs(fmod(ceil(fmod(x, 32)), 4)));			// distance from the camera's point to the east border of the block 
		if (enclaveBorderCoord == 0)
		{
			dist_to_neg = 4.0f - abs((fmod(fmod(x, 32), 4)));
		}
		else
		{
			dist_to_neg = abs(fmod(floor(fmod(x, 32)), 4)) - abs((fmod(fmod(x, 32), 4)));
		}
		//cout << "abs value 1: " << abs(fmod(floor(fmod(x, 32)), 4)) << endl;
		//cout << "abs value 2: " << abs((fmod(fmod(x, 32), 4))) << endl;
		exact_xyz = 0.0f + dist_to_neg;										// set the exact point in the block to be 0.0f + the distance from 0.0f
		//cout << "exact_xyz: " << exact_xyz << endl;
		//cout << "exact_pos_x: " << dist_to_pos << endl;
		//cout << "exact_neg_x: " << dist_to_neg << endl;

	}
	else if (x > 0)										// operations t be performed when the input value is greater than/equal to 0
	{
		collection_x = 0;
		if ((x_divide > 0) && x > 32)
		{
			if (fmod(x, 32) != 0)
			{
				collection_x = x_divide;  // collection_x = (x_divide + 1);
			}
			//else
			//{
				//collection_x = x_divide;
			//}
		}
		//else if ((x_divide > 0) && x <= 32)
		//{
			//collection_x = 1;
		//}
		//cout << "collection_x: " << collection_x << endl;
		chunk_x = (fmod(x, 32) / 4);					// old:  chunk_x = ((x % 32) / 4);
		//cout << "chunk_x: " << chunk_x << endl;
		block_x = (fmod(fmod(x, 32), 4));				// old : block_x = ((x % 32) % 4);
		//cout << "block_x: " << block_x << endl;

		dist_to_pos = ceil(fmod(fmod(x, 32), 4)) - (fmod(fmod(x, 32), 4));
		dist_to_neg = (fmod(fmod(x, 32), 4)) - block_x;
		exact_xyz = 0.0f + fmod(fmod(fmod(x, 32), 4), 1);									// set the exact point in the block to be 1.0f - the distance to get to 1.0f. // OLD: exact_xyz = 1.0f - dist_to_pos;
		//cout << "ceil of fmod x2: " << ceil(fmod(fmod(x, 32), 4)) << endl;
		//cout << "without ceil: " << fmod(fmod(x, 32), 4) << endl;
		//cout << "fmod test 1: " << fmod(x, 32) << endl;
		//cout << "fmod test 2: " << fmod(fmod(x, 32), 4) << endl;
		//cout << " exact_xyz (pos): " << exact_xyz << endl;

		//cout << "NoOfCollections passed: " << NoOfCollections << endl;
	}
	tempPathTrace.CollectionCoord = collection_x;		// set the return value for the Collection coordinate
	tempPathTrace.ChunkCoord = chunk_x;					// set the return value for the Chunk coordinate
	tempPathTrace.BlockCoord = block_x;					// set the return value for the block coordinate
	tempPathTrace.distance_to_pos = dist_to_pos;
	tempPathTrace.distance_to_neg = dist_to_neg;
	tempPathTrace.ExactBlockCoord = exact_xyz;				// the exact block coord
	

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

