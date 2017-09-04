#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <functional>
#include "OrganicSystem.h"
#include "OrganicTextureMeta.h"
#include "thread_pool.h"
#include "MDJobMaterializeCollection.h"
#include "MDListJobMaterializeCollection.h"
#include "EnclaveManifestFactoryT1Index.h"
#include <mutex>
#include <string>

OrganicSystem::OrganicSystem(int numberOfFactories)
{
	/* Summary: default constructor for the OrganicSystem */
	InterlockBaseCollections();
	AllocateFactories(numberOfFactories);
}

void OrganicSystem::InterlockBaseCollections()
{
	/* Summary: manually sets the required pointers for each of the base collections (EnclaveCollections, ManifestCollections, RenderCollections */
	ManifestCollections.SetCollectionMatrixRef(&EnclaveCollections);		// point the ManifestCollection to EnclaveCollection
	RenderCollections.SetManifestCollectionMatrixPtr(&ManifestCollections); // point the RenderCollection to the ManifestCollection
	RenderCollections.SetEnclaveCollectionMatrixPtr(&EnclaveCollections);
}

void OrganicSystem::AddAndMaterializeSingleCollectionMM(int x, int y, int z)
{

	/*------------------------------------------------------------------------------------------------------------- 
	Summary: this is a multi-step function that will create an EnclaveCollection with a key value of x/y/z:

	1. Add the collection, determine surface enclaves, determine solid enclaves, perform painting, unveil polys
	2. Iterate through all surface enclaves, and attach them to their appropriate ManifestCollection
	3. Prepare 3d rendering arrays (vertexes)
	4. Prepare 3d texture arrays 
	5. Assign pointers to an appropriate instance of EnclaveCollectionMesh
	
	-------------------------------------------------------------------------------------------------------------*/
	//thread_pool *poolptr;
	//poolptr = &testpool2;
	// STEP 1: add and instantiate 512 enclaves ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// NOTES: this step still needs to perform painting!
			
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;										// set temp key to input of x
	tempKey.y = y;										// set temp key to input of y
	tempKey.z = z;
	//cout << "Crash point testing " << endl;
	EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrix.BlueprintMap[tempKey];
	EnclaveCollectionBlueprintMatrix *blueprintmatrixptr = &BlueprintMatrix;
	// multithreaded testing begins here
	//EnclaveCollections.AddNewCollectionWithBlueprint(tempKey, blueprintptr);
	//EnclaveCollections.MultiAddNewCollectionWithBlueprint(4, tempKey, blueprintptr);
	//
	EnclaveCollections.SetOrganicSystem(this);
	auto start1 = std::chrono::high_resolution_clock::now();
	EnclaveCollections.MultiAddNewCollectionWithBlueprint(1, tempKey, blueprintmatrixptr, blueprintptr);
	//cout << "Crash point testing 2" << endl;
	//EnclaveCollections.MultiAddNewCollectionWithBlueprint(1, tempKey, blueprintptr);
	auto finish1 = std::chrono::high_resolution_clock::now();
	EnclaveCollection *collectionPtr = &EnclaveCollections.EnclaveCollectionMap[tempKey];
															// optional, for debugging
	std::chrono::duration<double> elapsed1 = finish1 - start1;
	cout << "Organic system phase 1: (Add collection, instantiate enclaves, determine solids, determine surface, perform painting, unveil polys): " << elapsed1.count() << endl;

	

	// STEP 2: iterate through surface enclaves and attach them |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	auto start3 = std::chrono::high_resolution_clock::now();
	ManifestCollections.AddNewCollection(tempKey);
	auto finish3 = std::chrono::high_resolution_clock::now();
	int manifestCounter = collectionPtr->totalRenderableEnclaves;
	EnclaveKeyDef::EnclaveKey innerTempKey;
	for (int a = 0; a < manifestCounter; a++)
	{
		innerTempKey = collectionPtr->RenderableEnclaves[a];
		//cout << "test of key values: [" << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << "]" << endl;
		ManifestCollections.AttachManifestToCollectedEnclave(tempKey, innerTempKey.x, innerTempKey.y, innerTempKey.z);
	}

																// optional, for debugging
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	cout << "Organic system phase 2: (Attachment to enclaves): " << elapsed3.count() << ": " << manifestCounter <<  endl;


	// STEP 3: prepare 3d rendering arrays ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	auto start4 = std::chrono::high_resolution_clock::now();
	RenderCollections.CreateRenderArrayFromManifestCollection(tempKey);	// STEP 6: prepare 3d rendering arrays
	auto finish4 = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed4 = finish4 - start4;
	//cout << "Organic system phase 3: (Create OpenGL vertex data array): " << elapsed4.count() << endl;
}

void OrganicSystem::JobMaterializeSingleCollectionFromMM(	EnclaveKeyDef::EnclaveKey Key1,
												EnclaveCollectionBlueprintMatrix BlueprintMatrixRef, 
												EnclaveCollectionMatrix EnclaveCollectionsRef,
												ManifestCollectionMatrix ManifestCollectionsRef, 
												RenderCollectionMatrix RenderCollectionsRef, 
												EnclaveCollection *CollectionRef,
												ManifestCollection *ManifestCollectionRef)			// change this name...to something better!
{
	/*
	
	Summary: materializes a single EnclaveCollection, by utilizing a MM

	*/


	// Phase 1: EnclaveCollection instantiation
	auto truestart = std::chrono::high_resolution_clock::now();										// optional, for performance tuning 
	EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef.BlueprintMap[Key1];				// hardcoded blueprint (for testing)
	EnclaveCollectionActivateListT2 listT2_1;														// set up the activation list
	EnclaveCollectionsRef.JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1));	// call the job to instantiate the EnclaveCollection
	int chunkbitmask = 1;	// set the chunk bit mask used below
	int bitmaskval = 0;		// ""
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
					EnclaveKeyDef::EnclaveKey tempKey;							// create a tempKey for this iteration
					tempKey.x = x;
					tempKey.y = bitmaskval;										// set the y to be equivalent to the current value of bitmask val (i.e, 1, 2, 4, 8 , 16, 32, 64, 128)
					tempKey.z = z;
					CollectionRef->ActivateEnclaveForRendering(tempKey);		// activate the enclave for rendering
					//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
				}

			}
			chunkbitmask <<= 1;
			bitmaskval++;

		}
	}



	// Phase 2: ManifestCollection set up
	int manifestCounter = CollectionRef->totalRenderableEnclaves;	// set the manifestCounter equal to the number of renderable manifests from the EnclaveCollection ref
	EnclaveKeyDef::EnclaveKey innerTempKey;		// set up temporary EnclaveKey thats used in loop below
	ManifestCollection *tempMCollectionRef = &ManifestCollectionsRef.GetRefManiCollectionMap(Key1);		// temp collection ref fails when using on line 169.
	for (int a = 0; a < manifestCounter; a++)	// begin loop
	{
		innerTempKey = CollectionRef->RenderableEnclaves[a];					// set temporary EnclaveKey variable equal to the EnclaveKey in the current element of RenderableEnclaves
		ManifestCollectionsRef.AttachManifestToCollectedEnclave2(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z, ManifestCollectionRef);	// activate attachment of manifest to enclave
	}


	// Phase 3: Render collection set up
	RenderCollectionsRef.CreateRenderArrayFromManifestCollection(Key1);
	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//cout << "Total time:  " << trueelapsed.count() << " :" << endl;
}

void OrganicSystem::MaterializeCollection(EnclaveKeyDef::EnclaveKey Key1, EnclaveKeyDef::EnclaveKey Key2)
{
	EnclaveCollections.SetOrganicSystem(this);
	thread_pool *tpref = getCell1();
	thread_pool *tpref2 = getCell2();
	EnclaveKeyDef::EnclaveKey dumbtempkey;
	dumbtempkey.x = 0;
	dumbtempkey.y = 0;
	dumbtempkey.z = 0;
	EnclaveCollectionBlueprint *blueprintptrpass = &BlueprintMatrix.BlueprintMap[dumbtempkey];
	//auto start4 = std::chrono::high_resolution_clock::now();

	// temporary

	EnclaveKeyDef::EnclaveKey key3, key4, key5, key6, key7, key8;
	key3.x = 3;
	key3.y = 0;
	key3.z = 1;

	key4.x = 8;
	key4.y = 0;
	key4.z = 1;


	key5.x = 9;
	key5.y = 0;
	key5.z = 1;

	key6.x = 10;
	key6.y = 0;
	key6.z = 1;

	key7.x = 11;
	key7.y = 0;
	key7.z = 1;

	key8.x = 12;
	key8.y = 0;
	key8.z = 1;

	
	/////////////////////////////////////////////////////////////////// DATA SET UP


	// -------------------- For first list to be added to queue

	MDListJobMaterializeCollection tempJobList;


	MDListJobMaterializeCollection newJobList, newJobList2;

	EnclaveCollectionBlueprintMatrix *passBlueprintMatrixPtr = &BlueprintMatrix;
	EnclaveCollectionMatrix *passEnclaveCollectionPtr = &EnclaveCollections;
	EnclaveCollection *passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[Key1];
	ManifestCollection *passManifestPtr = &ManifestCollections.ManiCollectionMap[Key1];

	EnclaveCollection *passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key3];
	ManifestCollection *passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key3];

	ManifestCollectionMatrix *passManifestCollPtr = &ManifestCollections;
	RenderCollectionMatrix *passRenderCollMatrixPtr = &RenderCollections;



	/////////////////////////////////////////////////////////////////

	// BEGIN NEW JOB STYLE TASKS

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[Key1];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[Key1];
	MDJobMaterializeCollection NewMDJob1(Key1, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList.ListMatrix[Key1] = NewMDJob1;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[Key2];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[Key2];
	MDJobMaterializeCollection NewMDJob2(Key2, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList.ListMatrix[Key2] = NewMDJob2;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key5];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key5];
	MDJobMaterializeCollection NewMDJob3(key5, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList.ListMatrix[key5] = NewMDJob3;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key6];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key6];
	MDJobMaterializeCollection NewMDJob4(key6, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList.ListMatrix[key6] = NewMDJob4;




	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key3];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key3];
	MDJobMaterializeCollection NewMDJob5(key3, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList2.ListMatrix[key3] = NewMDJob5;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key4];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key4];
	MDJobMaterializeCollection NewMDJob6(key4, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList2.ListMatrix[key4] = NewMDJob6;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key7];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key7];
	MDJobMaterializeCollection NewMDJob7(key7, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList2.ListMatrix[key7] = NewMDJob7;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key8];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key8];
	MDJobMaterializeCollection NewMDJob8(key8, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
	newJobList2.ListMatrix[key8] = NewMDJob8;

	// END NEW JOB STYLE TASKS


	std::mutex mutexval;

	////////////////////////////////////////////////////////////////////////////// BEGIN HIGH MEMORY EFFICIENCY TEST

	//std::future<void> coll_1 = tpref->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromMM, this, std::ref(newJobList), std::ref(mutexval), 1);							// MOST UP TO DATE 
	//std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromMM, this, std::ref(newJobList2), std::ref(mutexval), 2);
	auto start4 = std::chrono::high_resolution_clock::now();															// optional, for debugging
	//coll_1.wait();																										
	//coll_2.wait();																										
	auto finish4 = std::chrono::high_resolution_clock::now();															
	std::chrono::duration<double> elapsed4 = finish4 - start4;															// ""
	cout << "Dual coollection instantiation speed (High Memory Efficiency):  " << elapsed4.count() << endl;				// ""

	////////////////////////////////////////////////////////////////////////////// END HIGH MEMORY EFFICIENCY TEST


	////////////////////////////////////////////////////////////////////////////// BEGIN LOW MEMORY EFFICIENCY TEST

	//EnclaveManifestFactoryT1Index MainFactoryIndex;

	OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap["Factory 1"];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;

	OrganicFactoryIndex.FactoryMap["Factory 2"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr2 = &OrganicFactoryIndex.FactoryMap["Factory 2"];
	FactoryPtr2->TextureDictionaryRef = &TextureDictionary;
	
	//std::future<void> coll_3 = tpref->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory, this, std::ref(tempJobList), std::ref(mutexval), std::ref(FactoryPtr), 1);
	//std::future<void> coll_4 = tpref2->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory, this, std::ref(tempJobList2), std::ref(mutexval), std::ref(FactoryPtr2), 2);

	//std::future<void> coll_3 = tpref->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory, this, newJobList, std::ref(mutexval), std::ref(FactoryPtr), 1);				// MOST UP TO DATE 
	//std::future<void> coll_4 = tpref2->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory, this, newJobList2, std::ref(mutexval), std::ref(FactoryPtr2), 2);			// MOST UP TO DATE 
	 // std::future<void> coll_4 = tpref2->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, tempJobList2);


	auto lowstart = std::chrono::high_resolution_clock::now();

	//coll_3.wait();
	//coll_4.wait();
	auto lowend = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (Low Memory Efficiency):  " << lowelapsed.count() << endl;
	////////////////////////////////////////////////////////////////////////////// END LOW MEMORY EFFICIENCY TEST

	///////////////////////////////////// COLLECTION REMATERIALIZE TEST
	auto rematstart = std::chrono::high_resolution_clock::now();
	EnclaveCollection *CollectionPtr = &EnclaveCollections.EnclaveCollectionMap[key3];
	RenderCollectionMatrix *RenderCollectionsPtr = &RenderCollections;
	std::future<void> remat_1 = tpref->submit5(&OrganicSystem::JobRematerializeSingleExistingCollectionFromFactory, this, key3, std::ref(CollectionPtr), std::ref(FactoryPtr), std::ref(RenderCollectionsPtr), std::ref(mutexval));
	remat_1.wait();
	auto rematend = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> rematelapsed = rematend - rematstart;
	cout << "Rematerialization speed:  " << rematelapsed.count() << endl;



	///////////////////////////////////// END COLLECTION REMATERIALIZE TEST


	//// DUMMY JOB TEST

	auto dummystart = std::chrono::high_resolution_clock::now();
	std::future<void> dummyjob_1 = tpref->submit5(&OrganicSystem::DummyJob, this, 5, std::ref(FactoryPtr2), std::ref(mutexval));
	dummyjob_1.wait();
	auto dummyend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dummytotal = dummyend - dummystart;
	cout << "Dummy job complete time:  " << dummytotal.count() << endl;

	/////////////////////////

	EnclaveCollection *T2CollectionRef = &EnclaveCollections.EnclaveCollectionMap[key4];

	auto factorystart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < 64; x++)
	{
		EnclaveKeyDef::EnclaveKey tempKey = T2CollectionRef->RenderableEnclaves[x];
		Enclave *tempEnclavePtr = &T2CollectionRef->GetEnclaveByKey(tempKey);
		FactoryPtr->AttachManifestToEnclave(tempEnclavePtr);
		FactoryPtr->StorageArray[x].VertexArrayCount = 2;

	}

	auto factoryend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> factoryelapsed = factoryend - factorystart;
	cout << "Factory iteration time  " << factoryelapsed.count() << endl;
}

void OrganicSystem::MaterializeAllCollectionsInRenderList()
{
	EnclaveCollections.SetOrganicSystem(this);
	thread_pool *tpref = getCell1();				
	thread_pool *tpref2 = getCell2();
	int totalCollectionsToRender = 0;																				// will count total number of collections to be rendered from renderCollectionList
	int numberOfThreadsToRun = 2;																					// indicates the number of threads that will be running these jobs
	int collectionsPerThread = 0;																					// indicates how many collections each thread will process.
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter = renderCollectionList.KeyVector.begin();	// set iterator to be the beginning of the list.
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter2 = renderCollectionList.KeyVector.begin();

	// determine the number of collections to render.
	for (collectionListIter; collectionListIter != renderCollectionList.KeyVector.end(); ++collectionListIter)
	{
		totalCollectionsToRender++;
	}


	// split up the collections into MDListJobMaterializeCollection objects; number of MDListJobMaterializeCollection objects will be equal to numberOfThreadsToRun
	collectionsPerThread = totalCollectionsToRender / numberOfThreadsToRun;				// determine the number of collections per thread

	EnclaveCollectionBlueprintMatrix *passBlueprintMatrixPtr = &BlueprintMatrix;		// set up required pointers that are used as parameters for each MDJob
	EnclaveCollectionMatrix *passEnclaveCollectionPtr = &EnclaveCollections;
	ManifestCollectionMatrix *passManifestCollPtr = &ManifestCollections;
	RenderCollectionMatrix *passRenderCollMatrixPtr = &RenderCollections;
	EnclaveCollection *passCollectionPtrNew;
	ManifestCollection *passManifestPtrNew;


	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		MDListJobMaterializeCollection tempMatCollList;
		MatCollList.MaterializeCollectionList.push_back(tempMatCollList);
		MDListJobMaterializeCollection* tempMatCollListRef = &MatCollList.MaterializeCollectionList.back();
		for (int y = 0; y < collectionsPerThread; y++)
		{
			EnclaveKeyDef::EnclaveKey tempKey = *collectionListIter2;
			passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[tempKey];
			passManifestPtrNew = &ManifestCollections.ManiCollectionMap[tempKey];
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew));
			++collectionListIter2;
			tempMatCollListRef->ListMatrix[tempKey] = tempMDJob;

		}
	}

	std::mutex mutexval;

	OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap["Factory 1"];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;

	OrganicFactoryIndex.FactoryMap["Factory 2"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr2 = &OrganicFactoryIndex.FactoryMap["Factory 2"];
	FactoryPtr2->TextureDictionaryRef = &TextureDictionary;

	MDListJobMaterializeCollection* list1 = &MatCollList.MaterializeCollectionList.front();
	MDListJobMaterializeCollection* list2 = &MatCollList.MaterializeCollectionList.back();

	std::future<void> coll_3 = tpref->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(list1), std::ref(mutexval), std::ref(FactoryPtr), 1);
	std::future<void> coll_4 = tpref2->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(list2), std::ref(mutexval), std::ref(FactoryPtr2), 2);

	auto lowstart = std::chrono::high_resolution_clock::now();

	coll_3.wait();
	coll_4.wait();
	auto lowend = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (Low Memory Efficiency):  " << lowelapsed.count() << endl;
}

void OrganicSystem::SetupFutureCollectionMM(int x, int y, int z)
{
	/* Summary: sets up all necessary future data structures for a collection
	that will be rendered at a later time, and which is also generated from
	a ManifestMatrix (type 1 generation)
	*/

	//cout << "future collection call: " << endl;

	EnclaveKeyDef::EnclaveKey tempKey;		// temporary key used to uniquely identify the collections about to be added
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	EnclaveCollections.AddNewCollectionSkeleton(tempKey);							// adds a new EnclaveCollection to the OrganicSystem's EnclaveCollection matrix.
	ManifestCollections.AddNewCollection(tempKey);									// adds a new ManifestCollection to the OrganicSystem's MM
	ManifestCollection& tempRef = ManifestCollections.ManiCollectionMap[tempKey];	// unknown, possibly obsolete (test later)
	ManifestCollections.ManiCollectionMapRef.emplace(tempKey, tempRef);				// unknown, possibly obsolete (test later)

	RenderCollections.RenderMatrix[tempKey].LastCollectionTriangleCount = 0;	// set up some value in the map
}

void OrganicSystem::SetupFutureCollectionMM(EnclaveKeyDef::EnclaveKey tempKey)
{
	EnclaveCollections.AddNewCollectionSkeleton(tempKey);							// adds a new EnclaveCollection to the OrganicSystem's EnclaveCollection matrix.
	ManifestCollections.AddNewCollection(tempKey);									// adds a new ManifestCollection to the OrganicSystem's MM
	ManifestCollection& tempRef = ManifestCollections.ManiCollectionMap[tempKey];	// unknown, possibly obsolete (test later)
	ManifestCollections.ManiCollectionMapRef.emplace(tempKey, tempRef);				// unknown, possibly obsolete (test later)

	RenderCollections.RenderMatrix[tempKey].LastCollectionTriangleCount = 0;	// set up some value in the map
}

void OrganicSystem::ChangeSingleBlockMaterialAtXYZ(int x, int y, int z, int newmaterial)
{
	/* Summary: changes the material of a block at the absolute world x/y/z */
	EnclaveKeyDef::EnclaveKey CollectionKey, EnclaveKey;
	PathTraceContainer XPathTrace = EnclaveCollections.GetCoordTrace(x);	// get x coords for collection/chunk/block
	PathTraceContainer YPathTrace = EnclaveCollections.GetCoordTrace(y);	// get y coords	for collection/chunk/block
	PathTraceContainer ZPathTrace = EnclaveCollections.GetCoordTrace(z);	// get z coords for collection/chunk/block
	CollectionKey.x = XPathTrace.CollectionCoord;					// set coords for the Enclave Collection
	CollectionKey.y = YPathTrace.CollectionCoord;
	CollectionKey.z = ZPathTrace.CollectionCoord;
	EnclaveKey.x = XPathTrace.ChunkCoord;						// set coords for the x/y/z location of Enclave within the Collection
	EnclaveKey.y = YPathTrace.ChunkCoord;
	EnclaveKey.z = ZPathTrace.ChunkCoord;
	int BlockX = XPathTrace.BlockCoord;
	int BlockY = YPathTrace.BlockCoord;
	int BlockZ = ZPathTrace.BlockCoord;

	//cout << "Testing for ChangeSingleBlockMaterialAtXYZ: " << endl;
	//cout << "Collection XYZ: " << CollectionKey.x << " " << CollectionKey.y << " " << CollectionKey.z << " " << endl;
	//cout << "Chunk XYZ: " << EnclaveKey.x << " " << EnclaveKey.y << " " << EnclaveKey.z << " " << endl;
	//cout << "Block XYZ: " << BlockX << " " << BlockY << " " << BlockZ << " " << endl;

	//EnclaveCollectionMap[tempkey].EnclaveArray[XPathTrace.ChunkCoord][YPathTrace.ChunkCoord][ZPathTrace.ChunkCoord];
	Enclave *tempEnclave = &EnclaveCollections.EnclaveCollectionMap[CollectionKey].EnclaveArray[EnclaveKey.x][EnclaveKey.y][EnclaveKey.z];
	tempEnclave->ChangePolyMaterial(BlockX, BlockY, BlockZ, 1);
	tempEnclave->UnveilSinglePolyWithMtrl(2, 2, 0, 0, 1, 0, 40, 0);
	tempEnclave->UnveilSinglePolyWithMtrl(2, 1, 0, 0, 1, 0, 40, 0);
	tempEnclave->UnveilSinglePolyWithMtrl(2, 0, 0, 0, 1, 0, 40, 0);

	ManifestCollections.UpdateAttachedManifest(CollectionKey, EnclaveKey.x, EnclaveKey.y, EnclaveKey.z);	// causes bizarre issue 
	tempEnclave->ChangePolyMaterial(0, 3, 0, 1);												// very fast operation; doing all material blocks to be changed in the current chunk is orders more efficient...
	tempEnclave->ChangePolyMaterial(1, 3, 0, 1);
	tempEnclave->ChangePolyMaterial(2, 3, 0, 1);
										
	tempEnclave->ChangePolyMaterial(3, 3, 0, 1);
	tempEnclave->ChangePolyMaterial(0, 3, 1, 1);
	tempEnclave->ChangePolyMaterial(1, 3, 2, 1);
	tempEnclave->ChangePolyMaterial(2, 3, 3, 1);
										
	tempEnclave->ChangePolyMaterial(0, 3, 0, 1);
	tempEnclave->ChangePolyMaterial(1, 3, 1, 1);
	tempEnclave->ChangePolyMaterial(2, 3, 2, 1);
	tempEnclave->ChangePolyMaterial(3, 3, 3, 1);
										
	tempEnclave->ChangePolyMaterial(0, 3, 0, 1);
	tempEnclave->ChangePolyMaterial(1, 3, 1, 1);
	tempEnclave->ChangePolyMaterial(2, 3, 2, 1);

	
	
	
	
	std::mutex mutexval;
	thread_pool *tpref = getCell1();
	//OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap["Factory 1"];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;
	EnclaveCollection *CollectionPtr = &EnclaveCollections.EnclaveCollectionMap[CollectionKey];
	RenderCollectionMatrix *RenderCollectionsPtr = &RenderCollections;
	std::future<void> remat_1 = tpref->submit5(&OrganicSystem::JobRematerializeSingleExistingCollectionFromFactory, this, CollectionKey, std::ref(CollectionPtr), std::ref(FactoryPtr), std::ref(RenderCollectionsPtr), std::ref(mutexval));
	remat_1.wait();
	
}

void OrganicSystem::AddBlueprint(int x, int y, int z, EnclaveCollectionBlueprint blueprint)
{
	/* Summary: adds a new blueprint to the OrganicSystem's blueprint map. */

	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;										// set temp key to input of x
	tempKey.y = y;										// set temp key to input of y
	tempKey.z = z;										// set temp key to input of z
	BlueprintMatrix.BlueprintMap[tempKey] = blueprint;	// add blueprint to the BlueprintMatrix's unordered map -- BlueprintMap
}

thread_pool* OrganicSystem::getCell1()
{
	return Cell1;
}

thread_pool* OrganicSystem::getCell2()
{
	return Cell2;
}

void OrganicSystem::SetOrganicCell1(thread_pool *thread_pool_ref)
{
	Cell1 = thread_pool_ref;
}

void OrganicSystem::SetOrganicCell2(thread_pool *thread_pool_ref)
{
	Cell2 = thread_pool_ref;
}

void OrganicSystem::AddOrganicTextureMetaArray(string mapname)
{
	/* Summary: adds a new texture meta array, that will have a key value of the passed in parameter, "mapname" */

	OrganicTextureMeta tempMeta(0);															// dummy constructor for map compatibility (can fix later)
	TextureDictionary.Dictionary[mapname].Index[1] = tempMeta;								// set up the texture meta for a block id of 1.
	OrganicTextureMeta *tempMetaRef = &TextureDictionary.Dictionary[mapname].Index[1];		// set up a reference to the new texture data for the block

	// set up some data 
	tempMetaRef->BlockData.FaceIndex[0].FaceData[0].U = 2;									// test data only, will be fixed later.
}

void OrganicSystem::JobMaterializeMultiCollectionFromMM(MDListJobMaterializeCollection mdjob, mutex& mutexval, int ThreadID)
{
	/* Summary: this method materializes one or more EnclaveCollections, by using a ManifestMatrix. */

	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;		// set up an iterator to point to the beginning of the job list
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;	// set up an iterator to point to the end of the job list

	
	JobIterator = mdjob.ListMatrix.begin();		// set iterators before loop starts
	JobIteratorEnd = mdjob.ListMatrix.end();	// ""
	
	EnclaveCollectionBlueprintMatrix *BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;		// set Blueprint matrix ref
	EnclaveCollectionMatrix *EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;		// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix *RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;			// set a ref to the RenderCollection matrix

	//mutexval.lock();
	auto lockstart = std::chrono::high_resolution_clock::now();				// performance testing timestamps (optional use, only used for debugging; disable at will)
	auto lockend = std::chrono::high_resolution_clock::now();				// ""
	auto truestart = std::chrono::high_resolution_clock::now();				// ""
	//mutexval.unlock();

	for (JobIterator = mdjob.ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)			// begin iterator looping
	{
	
	
		auto initstart = std::chrono::high_resolution_clock::now();											// performance testing only
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;											// set the EnclaveKey for this loop iteration
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];					// set a pointer to the appropriate blueprint
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;						// set a pointer to the actual EnclaveCollection


		mutexval.lock();																					/*thread safety:		*/
		ManifestCollection *ManifestCollectionRef = JobIterator->second.MDManifestCollectionPtr;			// set pointer in thread-safe code
		mutexval.unlock();																					// unlock when finished


		//mutexval.lock();																					// potentially unneeded lock (lock is disabled for now)
		auto initend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> initelapsed = initend - initstart;
		//mutexval.unlock();																				// potentially unneeded lock (lock is disabled for now)

		EnclaveCollectionActivateListT2 listT2_1;																									// creation an activation list for instantiating the enclaves
		EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1));	// run the instantiation job on this thread (all 512 enclaves)
		int chunkbitmask = 1;		// set the chunk bit mask used below
		int bitmaskval = 0;			// ""


		// Phase 1: EnclaveCollection instantiation
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
						EnclaveKeyDef::EnclaveKey tempKey;								// create a tempKey for this iteration
						tempKey.x = x;														
						tempKey.y = bitmaskval;											// set the y to be equivalent to the current value of bitmask val (i.e, 1, 2, 4, 8 , 16, 32, 64, 128)
						tempKey.z = z;													
						CollectionRef->ActivateEnclaveForRendering(tempKey);			// activate the enclave for rendering
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;		// move the bitmask to the "left"
				bitmaskval++;			// increment bitmask val
			}
		}


		// Phase 2: ManifestCollection set up
		//mutexval.lock();
		int manifestCounter = CollectionRef->totalRenderableEnclaves;	// set the manifestCounter equal to the number of renderable manifests from the EnclaveCollection ref
		auto start5 = std::chrono::high_resolution_clock::now();	// optional performance testing values
		EnclaveKeyDef::EnclaveKey innerTempKey;		// create a variable to store a temporary key
		for (int a = 0; a < manifestCounter; a++)	// loop count is equal to the number of manifests to be rendered 
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			ManifestCollectionRef->AddManifestToMatrix(innerTempKey.x, innerTempKey.y, innerTempKey.z, Key1, 3, std::ref(mutexval));
		}
		auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
		std::chrono::duration<double> elapsed5 = finish5 - start5;									// ""
		//mutexval.unlock();
		//cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[innerTempKey].TotalEnclaveTriangles << endl;				// RENAME THIS TO SOMETHING ELSE! (ManifestCollectionRef)


		// Phase 3: Render collection set up
		mutexval.lock();																		/*thread safety:
																								make sure only one thread is actually accessing a ManifestCollection from a MM at a time;
																								without the lock, one thread could write (on the heap) while another reads -- resulting in potential heap corruption.
																								*/
		RenderCollectionsRef->CreateRenderArrayFromManifestCollection(Key1);						// creates the to-be rendered array, from a MM
		mutexval.unlock();

		
		
	}

	// below: completely optional performance testing
	mutexval.lock();
	std::chrono::duration<double> truelocktime = lockstart - lockend;
	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	cout << "Total time:  " << trueelapsed.count() << endl;
	mutexval.unlock();
}

void OrganicSystem::JobMaterializeMultiCollectionFromFactory(MDListJobMaterializeCollection mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID)
{
	/* Summary: this method materializes one or more EnclaveCollections, by using a Factory */
	auto truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only																										
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;			// set up an iterator to point to the beginning of the job list
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;		// set up an iterator to point to the end of the job list

																																		//auto truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
	auto trueend = std::chrono::high_resolution_clock::now();
	JobIterator = mdjob.ListMatrix.begin();							// set iterators before loop starts
	JobIteratorEnd = mdjob.ListMatrix.end();						// ""

	EnclaveCollectionBlueprintMatrix *BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;			// set Blueprint matrix ref
	EnclaveCollectionMatrix *EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;			// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix *RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;				// set a ref to the RenderCollection matrix

																											//truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
	for (JobIterator = mdjob.ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)
	{
		//truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
		auto initstart = std::chrono::high_resolution_clock::now();												// for performance testing only
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;												// set the EnclaveKey for this loop iteration	
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];						// set a pointer to the appropriate blueprint
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;							// set a pointer to the actual EnclaveCollection


		auto initend = std::chrono::high_resolution_clock::now();												// for performance testing only
		std::chrono::duration<double> initelapsed = initend - initstart;										// ""

		EnclaveCollectionActivateListT2 listT2_1;																									// creation an activation list for instantiating the enclaves
		EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
		//EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha2(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(BlueprintMatrixRef), std::ref(listT2_1));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
																																					//trueend = std::chrono::high_resolution_clock::now();
		int chunkbitmask = 1;	// set the chunk bit mask used below
		int bitmaskval = 0;		// ""
		int renderablecount = 0;


		// Phase 1: EnclaveCollection instantiation
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
						EnclaveKeyDef::EnclaveKey tempKey;						// create a tempKey for this iteration
						tempKey.x = x;
						tempKey.y = bitmaskval;									// set the y to be equivalent to the current value of bitmask val (i.e, 1, 2, 4, 8 , 16, 32, 64, 128)
						tempKey.z = z;
						renderablecount++;
						CollectionRef->ActivateEnclaveForRendering(tempKey);	// activate the enclave for rendering
					}

				}
				chunkbitmask <<= 1;
				bitmaskval++;
			}
		}


		//cout << "renderable count: " << renderablecount << endl;

		// Phase 2: Factory work
		int manifestCounter = CollectionRef->totalRenderableEnclaves;
		//auto start5 = std::chrono::high_resolution_clock::now();
		EnclaveKeyDef::EnclaveKey innerTempKey;
		FactoryRef->CurrentStorage = 0;					// reset storage location.
		FactoryRef->StorageArrayCount = 0;
		for (int a = 0; a < manifestCounter; a++)
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			Enclave *tempEnclavePtr = &CollectionRef->GetEnclaveByKey(innerTempKey);
			FactoryRef->AttachManifestToEnclave(tempEnclavePtr);
		}

		// Phase 3: Render actual collection
		RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval));		// call function to add data into array; pass mutex to use
		RenderCollection* collPtr = &RenderCollectionsRef->RenderMatrix[Key1];
		//cout << "Total renderables for Key (" << Key1.x << ", " << Key1.y << ", " << Key1.z << ") :" << tempdumbcount << ": " << collPtr->RenderCollectionArraySize << endl;
		//trueend = std::chrono::high_resolution_clock::now();

	}
	trueend = std::chrono::high_resolution_clock::now();
	mutexval.lock();
	//std::chrono::duration<double> truelocktime = lockstart - lockend;
	//auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//std::chrono::duration<double> unordered_elapsed = unordered_end - unordered_start;

	cout << "Total time: " << trueelapsed.count() << endl;
	mutexval.unlock();
}

void OrganicSystem::JobMaterializeMultiCollectionFromFactory2(MDListJobMaterializeCollection* mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID)
{
	/* Summary: this method materializes one or more EnclaveCollections, by using a Factory */
	//mutexval.lock();
	auto truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only																										
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;			// set up an iterator to point to the beginning of the job list
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;		// set up an iterator to point to the end of the job list

																																		//auto truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
	auto trueend = std::chrono::high_resolution_clock::now();
	JobIterator = mdjob->ListMatrix.begin();							// set iterators before loop starts
	JobIteratorEnd = mdjob->ListMatrix.end();						// ""

	EnclaveCollectionBlueprintMatrix *BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;			// set Blueprint matrix ref
	EnclaveCollectionMatrix *EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;			// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix *RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;				// set a ref to the RenderCollection matrix
																									//truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
	for (JobIterator = mdjob->ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)
	{
		//truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
		auto initstart = std::chrono::high_resolution_clock::now();												// for performance testing only
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;												// set the EnclaveKey for this loop iteration	
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];						// set a pointer to the appropriate blueprint
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;							// set a pointer to the actual EnclaveCollection


		auto initend = std::chrono::high_resolution_clock::now();												// for performance testing only
		std::chrono::duration<double> initelapsed = initend - initstart;										// ""

		EnclaveCollectionActivateListT2 listT2_1;		
		
		EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha2(0, 7 + 1, std::ref(*CollectionRef), Key1, std::ref(blueprintptr), std::ref(BlueprintMatrixRef), std::ref(listT2_1), std::ref(mutexval));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
		int chunkbitmask = 1;	// set the chunk bit mask used below
		int bitmaskval = 0;		// ""
		int renderablecount = 0;

		
		// Phase 1: EnclaveCollection instantiation
		
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
						EnclaveKeyDef::EnclaveKey tempKey;						// create a tempKey for this iteration
						tempKey.x = x;
						tempKey.y = bitmaskval;									// set the y to be equivalent to the current value of bitmask val (i.e, 1, 2, 4, 8 , 16, 32, 64, 128)
						tempKey.z = z;
						renderablecount++;
						CollectionRef->ActivateEnclaveForRendering(tempKey);	// activate the enclave for rendering
					}
					
				}
				chunkbitmask <<= 1;
				bitmaskval++; 
			}
		}

		// Phase 2: Factory work
		int manifestCounter = CollectionRef->totalRenderableEnclaves;
		//auto start5 = std::chrono::high_resolution_clock::now();
		EnclaveKeyDef::EnclaveKey innerTempKey;
		FactoryRef->CurrentStorage = 0;					// reset storage location.
		FactoryRef->StorageArrayCount = 0;
		for (int a = 0; a < manifestCounter; a++)
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			//cout << "Renderable key: " << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << endl;
			Enclave *tempEnclavePtr = &CollectionRef->GetEnclaveByKey(innerTempKey);
			FactoryRef->AttachManifestToEnclave(tempEnclavePtr);
		}

		// Phase 3: Render actual collection
		//cout << "Phase 3 beginning..." << endl;
		//mutexval.lock();
		RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval));		// call function to add data into array; pass mutex to use
		//mutexval.unlock();
		//RenderCollection* collPtr = &RenderCollectionsRef->RenderMatrix[Key1];
		//cout << "Total renderables for Key (" << Key1.x << ", " << Key1.y << ", " << Key1.z << ") :" << tempdumbcount << ": " << collPtr->RenderCollectionArraySize << endl;
		//trueend = std::chrono::high_resolution_clock::now();
		

	}
	trueend = std::chrono::high_resolution_clock::now();
	mutexval.lock();
	//std::chrono::duration<double> truelocktime = lockstart - lockend;
	//auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//std::chrono::duration<double> unordered_elapsed = unordered_end - unordered_start;

	cout << "Total time: " << trueelapsed.count() << endl;
	mutexval.unlock();
}

void OrganicSystem::DummyJob(int value, EnclaveManifestFactoryT1 *FactoryRef, mutex& mutexval)
{
	auto dummystart = std::chrono::high_resolution_clock::now();
	//std::future<void> dummyjob_1 = tpref->submit5(&OrganicSystem::DummyJob, this, 5);
	//dummyjob_1.wait();
	
	for (int x = 0; x < 1000000; x++)
	{

	}
	auto dummyend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dummytotal = dummyend - dummystart;
	//cout << "Dummy job real finish time:  " << dummytotal.count() << endl;
}

void OrganicSystem::JobRematerializeSingleExistingCollectionFromFactory(EnclaveKeyDef::EnclaveKey Key1, EnclaveCollection *CollectionRef, EnclaveManifestFactoryT1 *FactoryRef, RenderCollectionMatrix *RenderCollectionsRef, mutex& mutexval)
{
	/* Summary: this method rematerializes exactly one EnclaveCollection; it assumes that the EnclaveCollection 
		data has already been modified/loaded and is ready to go.
	*/

	// Phase 2: Factory work
	int manifestCounter = CollectionRef->totalRenderableEnclaves;
	EnclaveKeyDef::EnclaveKey innerTempKey;
	FactoryRef->CurrentStorage = 0;					// reset storage location.
	FactoryRef->StorageArrayCount = 0;
	for (int a = 0; a < manifestCounter; a++)
	{
		innerTempKey = CollectionRef->RenderableEnclaves[a];
		//cout << "test of key values: [" << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << "]" << endl;
		//ManifestCollectionsRef.AttachManifestToCollectedEnclave2(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z, ManifestCollectionRef, dumboutput);
		Enclave *tempEnclavePtr = &CollectionRef->GetEnclaveByKey(innerTempKey);
		FactoryRef->AttachManifestToEnclave(tempEnclavePtr);

	}

	// Phase 3: Render actual collection
	RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval));		// call function to add data into array; pass mutex to use
}

void OrganicSystem::SetGraphicsAPI()
{
	OGLM.InitializeOpenGL();

}

void OrganicSystem::RenderGLTerrain()
{
	OGLM.RenderReadyArrays();
}

void OrganicSystem::GLCleanup()
{
	OGLM.ShutdownOpenGL();
}

GLfloat* OrganicSystem::GetVertexDataFromRenderCollection(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	//GLfloat *returnPtr = &RenderCollections.RenderMatrix[tempKey].GetGLData;
	//return &RenderCollections.RenderMatrix[tempKey].GetGLData;
	//return returnPtr;
	GLfloat *tempPtr = RenderCollections.RenderMatrix[tempKey].GetGLData();
	return tempPtr;
	
}

void OrganicSystem::SendDataFromCollectionToGLBuffer(GLfloat* inFloatPtr, int inSize)
{
	OGLM.sendDataToBuffer(inFloatPtr, inSize);
}

RenderCollection* OrganicSystem::GetRenderCollectionPtr(int x, int y, int z)
{
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	RenderCollection *renderCollPtr = &RenderCollections.RenderMatrix[tempKey];
	return renderCollPtr;
}

void OrganicSystem::SendDataFromRenderPtrToGLBuffer(RenderCollection* renderCollectionPtr)
{
	OGLM.sendRenderCollectionDataToBuffer(renderCollectionPtr);
}

void OrganicSystem::AnalyzeRenderArray(int x, int y, int z, int xyz)
{
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	RenderCollection *renderCollPtr = &RenderCollections.RenderMatrix[tempKey];
	int totalloops = renderCollPtr->RenderCollectionArraySize / 4;
	int currentindex = 0;
	int startindex = 0;
	//cout << " dummy value test:" << renderCollPtr->GLFloatPtr[startindex] << ", " << renderCollPtr->GLFloatPtr[startindex++] << ", " << renderCollPtr->GLFloatPtr[startindex++] << endl;
	for (int x = 0; x < 100; x++)
	{
		//if ((renderCollPtr->GLFloatPtr[startindex+1 ]) < 28)
		//{
			cout << " dummy value test:" << renderCollPtr->GLFloatPtr[startindex ] << ", " << renderCollPtr->GLFloatPtr[startindex+1] << ", " << renderCollPtr->GLFloatPtr[startindex+2] << endl;
	//	}
		startindex += 3;
			
		currentindex += 3;
	}
}

void OrganicSystem::AllocateFactories(int noOfFactories)
{
	/* Summary: sets up Factories that will be used by the application */
	auto factorystart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < noOfFactories; x++)
	{
		char factoryprefix[] = "Factory ";
		string factorystring(factoryprefix);
		string factorynumber = to_string(x);
		string factory = factorystring + factorynumber;
		//cout << "Factory: " << factory << endl;
		//OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
		OrganicFactoryIndex.FactoryMap[factory].StorageArray[0].VertexArrayCount = 0;
	}
	auto factoryend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> factorytime = factoryend - factorystart;
	cout << "Organic Factory build time: " << factorytime.count() << endl;
}

void OrganicSystem::SetupFutureCollectionMMFromRenderList()
{

}

void OrganicSystem::AddKeyToRenderList(EnclaveKeyDef::EnclaveKey tempKey)
{
	renderCollectionList.KeyVector.push_back(tempKey);
	SetupFutureCollectionMM(tempKey);
}

void OrganicSystem::ArrayTest()
{
	EnclaveCollectionActivateListT2 listT2_1;
	listT2_1.flagArray[0][0] = 128;
}

void OrganicSystem::SendRenderListToGLTerrainBuffer()
{
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator renderListIter = renderCollectionList.KeyVector.begin();
	RenderCollection* newRenderCollPtr;
	for (renderListIter; renderListIter != renderCollectionList.KeyVector.end(); ++renderListIter)
	{
		EnclaveKeyDef::EnclaveKey tempKey = *renderListIter;						// get the key at this point in the vector
		newRenderCollPtr = GetRenderCollectionPtr(tempKey.x, tempKey.y, tempKey.z);	// get the pointer to the RenderCollection that has this key
		SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);							// send this RenderCollection's data to the buffer
	}
}