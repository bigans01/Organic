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

OrganicSystem::OrganicSystem(int numberOfFactories, int T1_bufferCubeSize, int T2_bufferCubeSize, int windowWidth, int windowHeight)
{
	/* Summary: default constructor for the OrganicSystem */
	InterlockBaseCollections();
	int numOfFactoriesToCreate = CreateThreads(numberOfFactories);		// creates the worker threads, returns the number of factories to create (will be equal to number of worker threads)
	T1_OGLMcubesize = T1_bufferCubeSize;
	T2_OGLMcubesize = T2_bufferCubeSize;
	AllocateFactories(numOfFactoriesToCreate);			// setup the factories
	OrganicGLManager* tempGLManagerPtr = &OGLM;			// get a pointer to the OrganicSystem's OGLM instance, and set the reference.
	OGLM.SendPointerToBufferManager(tempGLManagerPtr);	// send the pointer to the buffer manager, so that it may use it to set up its buffer arrays
	OGLM.SetupBufferManagerArrays(T1_bufferCubeSize, T2_bufferCubeSize);	// setup the buffer manager's dynamic arrays for both T1 and T2 terrain types
	OGLM.setWindowSize(windowWidth, windowHeight);		// set OpenGL window size
	OGLM.organicSystemPtr = this;
	OGLM.OrganicBufferManager.OGLMRMC.createContainerArrays(T1_bufferCubeSize, T2_bufferCubeSize);	// create the T1 and T2 dynamic array in the OGLMRMC; the arrays will be populated at a later time, once the location of the camera/player is known
	OGLM.createRenderableCollectionList(T2_bufferCubeSize);	// create the dynamic array that stores a list of renderable collections; the max number of renderable collections is equal to bufferCubeSize cubed.
	OGLM.OrganicBufferManager.DCMPtr = &OGLM.renderableCollectionList;	// set the OrganicBufferManager's DCMPtr (a pointer to an instance of OGLMDrawCallMeta)
	OGLM.OrganicBufferManager.blueprintMatrixPtr = &BlueprintMatrix;	// set the OrganicBufferManager's blueprint matrix pointer
	OGLM.OrganicBufferManager.organicSystemPtr = this;	// set the pointer to the processing queue in OGLM's buffer manager			
	OrganicCellManager* tempOCManagerPtr = &OCManager;	// create a temporary pointer to the intance of OrganicCellManager (used as a copy-by-value argument in following line)
	OCLimitsList.setInitialOrganicCellLimits(this, tempOCManagerPtr);		// sets the initial cell limits on engine startup (should only be called here); passes required pointer to this instance of OrganicSystem
	blockTargetMeta.setVertexOffsets();					// set up vertex offsets
	SetupCellMeta();									// add cells into the availability pool
	OrganicCellLimitsList* tempOrganicCellLimitsListPtr = &OCLimitsList;	// create a temporary pointer to the instance of OrganicCellLimitsList (used as a copy-by-value argument in following line)
	OCManager.setOrganicCellLimitsListPtr(tempOrganicCellLimitsListPtr);	// set the pointer to the OrganicCellLimits object, in the OCManager
	OCManager.setOrganicSystemPtr(this);				// pass the reference to this instance of OrganicSystem to the OCManager
	OCManager.populateCellMapsOnEngineStart();						// populate the cell maps in OCManager, based on the current number of cells available, and the chosen workPriority
	int tupleWidth = SetRenderMode(1);												// set render mode (0 = vertex only single color, 1 = vertex + vertex shaded triangles); this is requried before calling SetGraphicsAPI, which sets up OpenGL buffers, VBO tuples, etc
	OGLM.renderableCollectionList.setVertexTupleWidth(tupleWidth);	// set the appropriate tuple width
	SetGraphicsAPI();												// setup the graphics API (OpenGL context, etc)																			
	SelectShader();													// select the appropriate shader
}

OrganicSystem::~OrganicSystem()
{
	if (isThreadIndexInitialized == 1)
	{
		cout << "attempting delete of threads..." << endl;	// print to out, to check that threads were in process of being deleted
		for (int x = 0; x < threadIndexAmount; x++)
		{
			delete organicThreadIndex[x];		// thread cleanup, don't allow threads to run rampant in OS
		}
	}

}

int OrganicSystem::CreateThreads(int in_numberOfThreads)
{
	isThreadIndexInitialized = 1;
	//int numberOfThreads = std::thread::hardware_concurrency() - 1;
	int numberOfThreads = in_numberOfThreads;
	threadIndexAmount = numberOfThreads;				// set the thread index amount, for when we need to properly delete the threads.
	for (int x = 0; x < numberOfThreads; x++)
	{
		organicThreadIndex[x] = new thread_pool;		// create a new instance of thread_pool on the heap, allocate to it
		AddOrganicCell(organicThreadIndex[x]);			// add the cell
	}
	return numberOfThreads;				// return the number of threads; used to determine number of factories to create (next step in OrganicSystem constructor)
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
	thread_pool *tpref = OCList.cellList[0].threadPtr;
	thread_pool *tpref2 = OCList.cellList[1].threadPtr;
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
	MDJobMaterializeCollection NewMDJob1(Key1, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList.ListMatrix[Key1] = NewMDJob1;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[Key2];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[Key2];
	MDJobMaterializeCollection NewMDJob2(Key2, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList.ListMatrix[Key2] = NewMDJob2;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key5];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key5];
	MDJobMaterializeCollection NewMDJob3(key5, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList.ListMatrix[key5] = NewMDJob3;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key6];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key6];
	MDJobMaterializeCollection NewMDJob4(key6, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList.ListMatrix[key6] = NewMDJob4;




	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key3];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key3];
	MDJobMaterializeCollection NewMDJob5(key3, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList2.ListMatrix[key3] = NewMDJob5;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key4];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key4];
	MDJobMaterializeCollection NewMDJob6(key4, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList2.ListMatrix[key4] = NewMDJob6;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key7];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key7];
	MDJobMaterializeCollection NewMDJob7(key7, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList2.ListMatrix[key7] = NewMDJob7;

	passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[key8];
	passManifestPtrNew = &ManifestCollections.ManiCollectionMap[key8];
	MDJobMaterializeCollection NewMDJob8(key8, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
	newJobList2.ListMatrix[key8] = NewMDJob8;

	// END NEW JOB STYLE TASKS


	//std::mutex mutexval;

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

	OrganicFactoryIndex.FactoryMap[0].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap[0];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;

	OrganicFactoryIndex.FactoryMap[1].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr2 = &OrganicFactoryIndex.FactoryMap[1];
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
	std::future<void> remat_1 = tpref->submit5(&OrganicSystem::JobRematerializeSingleExistingCollectionFromFactory, this, key3, std::ref(CollectionPtr), std::ref(FactoryPtr), std::ref(RenderCollectionsPtr), std::ref(heapmutex));
	remat_1.wait();
	auto rematend = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> rematelapsed = rematend - rematstart;
	cout << "Rematerialization speed:  " << rematelapsed.count() << endl;



	///////////////////////////////////// END COLLECTION REMATERIALIZE TEST


	//// DUMMY JOB TEST

	auto dummystart = std::chrono::high_resolution_clock::now();
	std::future<void> dummyjob_1 = tpref->submit5(&OrganicSystem::DummyJob, this, 5, std::ref(FactoryPtr2), std::ref(heapmutex));
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
		FactoryPtr->AttachManifestToEnclave(tempEnclavePtr, 1);	// last argument is NEW PROTOTYPE DUMB VALUE
		FactoryPtr->StorageArray[x].VertexArrayCount = 2;

	}

	auto factoryend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> factoryelapsed = factoryend - factorystart;
	cout << "Factory iteration time  " << factoryelapsed.count() << endl;
}

void OrganicSystem::MaterializeAllCollectionsInRenderList(int renderProcess)
{
	/* Summary: renders all enclave keys found in member variable renderCollectionList*/
	if (renderProcess == 0)					// if 0, use Factory(s)
	{
		MaterializeRenderablesByFactory();
	}

	if (renderProcess == 1)					// if 1, use Manifest Matrix style (MM)
	{
		MaterializeRenderablesByMM();
	}
}

void OrganicSystem::MaterializeAllCollectionsInRenderList()
{
	auto newstart = std::chrono::high_resolution_clock::now();
	EnclaveCollections.SetOrganicSystem(this);
	// PHASE 1: split up the work, into the T1 and T2 processing queues
	int T1_axis_size = OGLM.OrganicBufferManager.T1_cubesize / 2;	// get the length of the T1 cube on a single axis
	int T2_axis_size = OGLM.OrganicBufferManager.T2_cubesize / 2;	// get the length of the T2 cube on a single axis
	EnclaveKeyDef::EnclaveKey T1_centerElementIndexValue;		// declare index values
	EnclaveKeyDef::EnclaveKey T2_centerElementIndexValue;

	T1_centerElementIndexValue.x = T1_axis_size;						// determine the center values of the T1 dynamic array
	T1_centerElementIndexValue.y = T1_axis_size;
	T1_centerElementIndexValue.z = T1_axis_size;

	T2_centerElementIndexValue.x = T2_axis_size;						// determine the center values of the T2 dynamic array
	T2_centerElementIndexValue.y = T2_axis_size;
	T2_centerElementIndexValue.z = T2_axis_size;

	EnclaveKeyDef::EnclaveKey centerKey = OGLM.OrganicBufferManager.currentCenterCollectionKey;

	cout << "++++++" << endl;
	cout << "Materialize Collections V2: " << centerKey.x << ", " << centerKey.y << ", " << centerKey.x << ", " << endl;
	cout << "Center element value is: " << T2_centerElementIndexValue.x << ", " << T2_centerElementIndexValue.y << ", " << T2_centerElementIndexValue.z << endl;
	cout << "++++++" << endl;
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter = renderCollectionList.KeyVector.begin();	// set iterator to be the beginning of the list.
	for (collectionListIter; collectionListIter != renderCollectionList.KeyVector.end(); ++collectionListIter)		// loop through each key
	{
		EnclaveKeyDef::EnclaveKey listKey = *collectionListIter;	// set the temp listKey
		
		// -7, 0, 0
		int tempelement = 0; 
		// Type 1 (T1) terrain checks...add the key to the list of of T1 keys to be rendered, if the key exists within the T1 field
		if (abs(listKey.x - centerKey.x) <= T1_axis_size		&&		abs(listKey.y - centerKey.y) <= T1_axis_size		&&		abs(listKey.z - centerKey.z) <= T1_axis_size)	// if the absolute difference between the listKey and the CenterKey is <= the T1_size, it will go into the T1 dynamic array (condition must be met for x y AND z).
		{
			//cout << "Crash test 0 (T1)" << endl;
			// part A: perform updates in T1 dynamic array
			SetupFutureCollectionMM(listKey);
			int currentT1BufferElement = OGLM.OrganicBufferManager.T1_translateXYZToSingle(T1_centerElementIndexValue.x + (listKey.x - centerKey.x),	// get the currentBufferElement
																						   T1_centerElementIndexValue.y + (listKey.y - centerKey.y), 
																						   T1_centerElementIndexValue.z + (listKey.z - centerKey.z));
			OGLM.OrganicBufferManager.OGLMRMC.T1_renderMetaContainerArray[currentT1BufferElement].ElementCollectionKey = listKey;
			OGLM.OrganicBufferManager.OGLMRMC.T1_renderMetaContainerArray[currentT1BufferElement].ContainsUsedT1Key = 1;								// indicate that this element actually contains a used key

			// part B: get "true" OpenGL buffer value for this element, by getting the center value of the T2 array and adding the (listKey - centerKey) value
			int trueBufferElement = OGLM.OrganicBufferManager.T2_translateXYZToSingle(T2_centerElementIndexValue.x + (listKey.x - centerKey.x),
																					  T2_centerElementIndexValue.y + (listKey.y - centerKey.y),
																					  T2_centerElementIndexValue.z + (listKey.z - centerKey.z)
				);

			// part C: prepare morph meta that is to be added to the T1 queue
			OrganicMorphMeta tempMorphMeta;
			tempMorphMeta.subBufferIndex = trueBufferElement;
			tempMorphMeta.collectionKey = listKey;
			T1CollectionProcessingQueue.push(tempMorphMeta);					// add to the T1 processing queue
			//cout << "Terrain type 1 (T1) List key added! " << listKey.x << ", " << listKey.y << ", " << listKey.z << endl;
		}
		
		//if (listKey.x == -7 && listKey.y == 0 && listKey.z == 0)
		//{
			//cout << "list key would be: " << listKey.x << endl;
			//cout << "center key would be: " << centerKey.x << endl;
		//}

		// Type 2 (T2) terrain checks
		else if (abs(listKey.x - centerKey.x) <= T2_axis_size		&&		abs(listKey.y - centerKey.y) <= T2_axis_size		&&		abs(listKey.z - centerKey.z) <= T2_axis_size)
		{


			//cout << "Crash test 0 (T2)" << endl;
			// part A: perform updates in T1 dynamic array
			int currentT2BufferElement = OGLM.OrganicBufferManager.T2_translateXYZToSingle(T2_centerElementIndexValue.x + (listKey.x - centerKey.x),
				T2_centerElementIndexValue.y + (listKey.y - centerKey.y),
				T2_centerElementIndexValue.z + (listKey.z - centerKey.z));
			tempelement = currentT2BufferElement;
			OGLM.OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[currentT2BufferElement].ContainsUsedT2Key = 1;								// indicate that this element actually contains a used key


			// part B: get "true" OpenGL buffer value for this element
			int trueBufferElement = OGLM.OrganicBufferManager.T2_translateXYZToSingle(T2_centerElementIndexValue.x + (listKey.x - centerKey.x),
				T2_centerElementIndexValue.y + (listKey.y - centerKey.y),
				T2_centerElementIndexValue.z + (listKey.z - centerKey.z)
			);

			// part C: prepare morph meta that is to be added to the T2 queue
			OrganicMorphMeta tempMorphMeta;
			tempMorphMeta.subBufferIndex = trueBufferElement;
			tempMorphMeta.collectionKey = listKey;
			T2CollectionProcessingQueue.push(tempMorphMeta);
			//cout << "Terrain type 2 (T2) List key added! " << listKey.x << ", " << listKey.y << ", " << listKey.z << endl;
			/*
			if (listKey.x == 6 && listKey.y == 0 & listKey.z == -3)
			{
				//cout << "----testing other values: " << endl;
				cout << T2_centerElementIndexValue.x << " + (" << (listKey.x - centerKey.x) << endl;
				cout << T2_centerElementIndexValue.y << " + (" << (listKey.y - centerKey.y) << endl;
				cout << T2_centerElementIndexValue.z << " + (" << (listKey.z - centerKey.z) << endl;

				cout << T2_centerElementIndexValue.x << " + (" << listKey.x << " - " << centerKey.x << ")" << endl;
				cout << T2_centerElementIndexValue.y << " + (" << listKey.y << " - " << centerKey.y << ")" << endl;
				cout << T2_centerElementIndexValue.z << " + (" << listKey.z << " - " << centerKey.z << ")" << endl;

				//cout << "Translate to T2 (actual key): " << OGLM.OrganicBufferManager.T2_translateXYZToSingle(T2_centerElementIndexValue.x + (listKey.x - centerKey.x),
					//T2_centerElementIndexValue.y + (listKey.y - centerKey.y),
					//T2_centerElementIndexValue.z + (listKey.z - centerKey.z));

				//cout << "Translate to T2 (actual key): " << OGLM.OrganicBufferManager.T2_translateXYZToSingle(T2_centerElementIndexValue.x + (listKey.x - centerKey.x),
					//T2_centerElementIndexValue.y + (listKey.y - centerKey.y),
					//T2_centerElementIndexValue.z + (listKey.z - centerKey.z));


				//cout << "key is registered!!!!!! (T2 type)" << endl;
				//cout << "MATERIALIZE ELEMENT VALUE IS:" << currentT2BufferElement << endl;
				int correctElement = OGLM.OrganicBufferManager.T2_translateXYZToSingle(6, 0, -3);
				//cout << "VAlUE SHOULD BE: " << correctElement << endl;
			}
			*/
		}

		// 767
		// cout << "VALUE OF 767 IS: (" << tempelement << ") (" << listKey.x << ", " << listKey.y << ", " << listKey.z << ") " << OGLM.OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[767].ContainsUsedT2Key << endl;
		

	}

	// PHASE 2: submit jobs to appropriate worker threads
	//cout << "T1 queue size (before loop): " << T1CollectionProcessingQueue.size() << endl;
	//cout << "T2 queue size (before loop): " << T2CollectionProcessingQueue.size() << endl;
	
	while (!T1CollectionProcessingQueue.empty() || !T2CollectionProcessingQueue.empty())	// do this until everything has been processed
	{
		DivideTickWork();			// split work for the tick here
		CheckProcessingQueue();		// check for work in collection processing queue(s)
		WaitForPhase2Promises();	// finish by waiting for promises to be fulfilled
		//cout << "aww snap" << endl;
	}
	//cout << "T1 queue size (after loop): " << T1CollectionProcessingQueue.size() << endl;
	//cout << "T2 queue size (after loop): " << T2CollectionProcessingQueue.size() << endl;
	auto newend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> newduration = newend - newstart;

	cout << "Collection loading complete...elapsed time: " << newduration.count() << endl;
	EnclaveKeyDef::EnclaveKey dumboutputKey = OGLM.OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[OGLM.OrganicBufferManager.T2_translateXYZToSingle(5, 1, -5)].ElementCollectionKey;
	cout << "TESTING OF ELEMENT VALUE: " << dumboutputKey.x << ", " << dumboutputKey.y << ", " << dumboutputKey.z << endl;
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
	auto setupstart = std::chrono::high_resolution_clock::now();
	EnclaveCollections.AddNewCollectionSkeleton(tempKey);							// adds a new EnclaveCollection to the OrganicSystem's EnclaveCollection matrix.
	ManifestCollections.AddNewCollection(tempKey);									// adds a new ManifestCollection to the OrganicSystem's MM
	ManifestCollection& tempRef = ManifestCollections.ManiCollectionMap[tempKey];	// unknown, possibly obsolete (test later)
	ManifestCollections.ManiCollectionMapRef.emplace(tempKey, tempRef);				// unknown, possibly obsolete (test later)

	RenderCollections.RenderMatrix[tempKey].LastCollectionTriangleCount = 0;	// set up some value in the map
	auto setupend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> setupduration = setupend - setupstart;
	cout << "Future MM setup time: " << setupduration.count() << endl;
}

void OrganicSystem::ChangeSingleBlockMaterialAtXYZ(int x, int y, int z, int newmaterial)
{
	/* Summary: changes the material of a block at the absolute world x/y/z */
	EnclaveKeyDef::EnclaveKey CollectionKey, EnclaveKey;
	PathTraceContainer XPathTrace = EnclaveCollections.GetCoordTrace(float(x));	// get x coords for collection/chunk/block
	PathTraceContainer YPathTrace = EnclaveCollections.GetCoordTrace(float(y));	// get y coords	for collection/chunk/block
	PathTraceContainer ZPathTrace = EnclaveCollections.GetCoordTrace(float(z));	// get z coords for collection/chunk/block
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

	
	
	
	
	//std::mutex mutexval;
	thread_pool *tpref = OCList.cellList[0].threadPtr;
	//OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap[1];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;
	EnclaveCollection *CollectionPtr = &EnclaveCollections.EnclaveCollectionMap[CollectionKey];
	RenderCollectionMatrix *RenderCollectionsPtr = &RenderCollections;
	std::future<void> remat_1 = tpref->submit5(&OrganicSystem::JobRematerializeSingleExistingCollectionFromFactory, this, CollectionKey, std::ref(CollectionPtr), std::ref(FactoryPtr), std::ref(RenderCollectionsPtr), std::ref(heapmutex));
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



void OrganicSystem::AddOrganicCell(thread_pool* thread_pool_ref)
{
	OrganicCell cellToAdd;			// create a temporary unfilled cell
	cellToAdd.cellStatus = 0;		// set the cell's status
	cellToAdd.threadPtr = thread_pool_ref;	// add the thread pointer
	OCList.cellList[OCList.numberOfCells++] = cellToAdd;	// add the cell to the map; the value of numberOfCells indicates the key in the map
}

void OrganicSystem::SetupCellMeta()
{
	std::unordered_map<int, EnclaveManifestFactoryT1>::iterator factoryMapIter;	// create a temp iterator
	std::map<int, OrganicCell>::iterator cellListIter;
	std::map<int, OrganicCell*>::iterator cellManagerIter;
	factoryMapIter = OrganicFactoryIndex.FactoryMap.begin();					// set the factory iterator's begin value
	cellListIter = OCList.cellList.begin();										// set the cellList iterator's begin value; we will start looking at the first OrganicCell in the list
	
	for (int x = 0; x < OCList.numberOfCells; x++)	// cycle through all cells
	{
		cellListIter->second.factoryPtr = &factoryMapIter->second;				// set the reference to the factory that this OrganicCell will bea paired with
		OCManager.availableCellMap[x] = &cellListIter->second;					// add the cell as available to the OCManager
		factoryMapIter++;			// increment the iterators
		cellListIter++;				// ""
	}
	
	//OCLimitsList.setInitialOrganicCellLimits(this);								// set the values in the OCLimitsList, such ax max number of cells (which is equal to the number of cells added); pass a pointer 
																	            // to this OrganicSystem. This can only be called after cells have been actually added. ( done in a previous step by calling 
}

void OrganicSystem::AddOrganicTextureMetaArray(string mapname)
{
	/* Summary: adds a new texture meta array, that will have a key value of the passed in parameter, "mapname" */

	OrganicTextureMeta tempMeta(0);															// dummy constructor for map compatibility (can fix later)
	//cout << "umm what" << endl;
	TextureDictionary.Dictionary[mapname].Index[1] = tempMeta;								// set up the texture meta for a block id of 1.
	//cout << "umm what" << endl;
	OrganicTextureMeta *tempMetaRef = &TextureDictionary.Dictionary[mapname].Index[1];		// set up a reference to the new texture data for the block

	// set up some data 
	tempMetaRef->BlockData.FaceIndex[0].FaceData[0].U = 2;									// test data only, will be fixed later.
}

void OrganicSystem::AddOrganicVtxColorMetaArray(string mapname)
{
	OrganicVtxColorMeta tempColorMeta(0);
	VertexColorDictionary.Dictionary[mapname].Index[1] = tempColorMeta;
	OrganicVtxColorMeta *tempColorMetaRef = &VertexColorDictionary.Dictionary[mapname].Index[1];
	
	// west face data
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[0].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[0].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[0].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[1].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[1].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[1].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[2].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[2].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[2].blue = 0.100f;

	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[3].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[3].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[3].blue = 0.150f;
	
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[4].red = 0.583f;		// lighter
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[4].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[4].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[5].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[5].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[0].FaceMeta[5].blue = 0.100f;

	//north face data
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[0].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[0].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[0].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[1].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[1].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[1].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[2].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[2].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[2].blue = 0.100f;

	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[3].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[3].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[3].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[4].red = 0.583f;		// lighter
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[4].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[4].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[5].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[5].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[1].FaceMeta[5].blue = 0.100f;

	//east face data
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[0].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[0].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[0].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[1].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[1].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[1].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[2].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[2].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[2].blue = 0.100f;

	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[3].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[3].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[3].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[4].red = 0.583f;		// lighter
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[4].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[4].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[5].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[5].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[2].FaceMeta[5].blue = 0.100f;

	//south face data
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[0].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[0].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[0].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[1].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[1].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[1].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[2].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[2].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[2].blue = 0.100f;

	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[3].red = 0.583f;			// lighter
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[3].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[3].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[4].red = 0.583f;		// lighter
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[4].green = 0.331f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[4].blue = 0.150f;

	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[5].red = 0.383f;			// darker
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[5].green = 0.131f;
	tempColorMetaRef->BlockData.FaceIndex[3].FaceMeta[5].blue = 0.100f;

	//top face data
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[0].red = 0.183f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[0].green = 0.771f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[0].blue = 0.014f;
										  
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[1].red = 0.171f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[1].green = 0.901f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[1].blue = 0.014f;
										  
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[2].red = 0.171f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[2].green = 0.800f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[2].blue = 0.04f;
										  
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[3].red = 0.183f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[3].green = 0.471f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[3].blue = 0.014f;

	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[4].red = 0.171f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[4].green = 0.901f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[4].blue = 0.014f;

	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[5].red = 0.171f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[5].green = 0.800f;
	tempColorMetaRef->BlockData.FaceIndex[4].FaceMeta[5].blue = 0.04f;

	//bottom face data
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[0].red = 0.583f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[0].green = 0.771f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[0].blue = 0.014f;
										  
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[1].red = 0.771f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[1].green = 0.221f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[1].blue = 0.014f;
										  
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[2].red = 0.171f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[2].green = 0.221f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[2].blue = 0.44f;
										  
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[3].red = 0.95f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[3].green = 0.021f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[3].blue = 0.044f;
										  
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[4].red = 0.971f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[4].green = 0.121f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[4].blue = 0.131f;
										  
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[5].red = 0.371f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[5].green = 0.44f;
	tempColorMetaRef->BlockData.FaceIndex[5].FaceMeta[5].blue = 0.521f;


}

void OrganicSystem::JobMaterializeMultiCollectionFromMM(MDListJobMaterializeCollection* mdjob, mutex& mutexval, int ThreadID)
{
	/* Summary: this method materializes one or more EnclaveCollections, by using a ManifestMatrix. */
	int totalprocessed = 0;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;		// set up an iterator to point to the beginning of the job list
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;	// set up an iterator to point to the end of the job list

	
	JobIterator = mdjob->ListMatrix.begin();		// set iterators before loop starts
	JobIteratorEnd = mdjob->ListMatrix.end();	// ""
	
	EnclaveCollectionBlueprintMatrix *BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;		// set Blueprint matrix ref
	EnclaveCollectionMatrix *EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;		// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix *RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;			// set a ref to the RenderCollection matrix

	auto lockstart = std::chrono::high_resolution_clock::now();				// performance testing timestamps (optional use, only used for debugging; disable at will)
	auto lockend = std::chrono::high_resolution_clock::now();				// ""
	auto truestart = std::chrono::high_resolution_clock::now();				// ""


	for (JobIterator = mdjob->ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)			// begin iterator looping
	{
		auto initstart = std::chrono::high_resolution_clock::now();											// performance testing only
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;											// set the EnclaveKey for this loop iteration
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];					// set a pointer to the appropriate blueprint
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;						// set a pointer to the actual EnclaveCollection


		mutexval.lock();																					/*thread safety:		*/
		ManifestCollection *ManifestCollectionRef = JobIterator->second.MDManifestCollectionPtr;			// set pointer in thread-safe code
		mutexval.unlock();																					// unlock when finished



		auto initend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> initelapsed = initend - initstart;

		EnclaveCollectionActivateListT2 listT2_1;																									// creation an activation list for instantiating the enclaves
		//EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1));	// run the instantiation job on this thread (all 512 enclaves)
		mutexval.lock();
		EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha2(0, 7 + 1, std::ref(*CollectionRef), Key1, std::ref(blueprintptr), std::ref(BlueprintMatrixRef), std::ref(listT2_1), std::ref(mutexval));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
		mutexval.unlock();
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
						//if (tempKey.x == 3 && tempKey.y == 1 && tempKey.z == 0)
						//{
							//cout << "enclave will be rendered..." << endl;
						//}
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;		// move the bitmask to the "left"
				bitmaskval++;			// increment bitmask val
			}
		}


		// Phase 2: ManifestCollection set up
		if (Key1.x == 0 && Key1.y == 0 && Key1.z == 0)
		{
			Enclave testEnclavePtr = CollectionRef->EnclaveArray[2][1][0];
			cout << "TEST: " << testEnclavePtr.GetTotalTrianglesInEnclave() << endl;
		}
		
		int manifestCounter = CollectionRef->totalRenderableEnclaves;	// set the manifestCounter equal to the number of renderable manifests from the EnclaveCollection ref
		auto start5 = std::chrono::high_resolution_clock::now();	// optional performance testing values
		EnclaveKeyDef::EnclaveKey innerTempKey;		// create a variable to store a temporary key
		for (int a = 0; a < manifestCounter; a++)	// loop count is equal to the number of manifests to be rendered 
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			if (Key1.x == 0 && Key1.y == 0 && Key1.z == 0)
			{
				if (innerTempKey.x == 3 && innerTempKey.y == 1 && innerTempKey.z == 0)
				{
					cout << "---------------------------------innerTempKey: " << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << " | " << CollectionRef->EnclaveArray[4][1][0].TotalRenderable <<endl;

				}
			}
			//cout << "innerTempKey: " << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << endl;
			ManifestCollectionRef->AddManifestToMatrix(innerTempKey.x, innerTempKey.y, innerTempKey.z, Key1, 3, std::ref(mutexval));
		}
		auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
		std::chrono::duration<double> elapsed5 = finish5 - start5;									// ""
																									
		//cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[innerTempKey].TotalEnclaveTriangles << endl;				// RENAME THIS TO SOMETHING ELSE! (ManifestCollectionRef)
		// Phase 3: Render collection set up													
		RenderCollectionsRef->CreateRenderArrayFromManifestCollection(Key1, std::ref(mutexval), 1);						// creates the to-be rendered array, from a MM;;; // last argument is NEW PROTOTYPE DUMB VALUE

		
		totalprocessed++;
		
	}

	// below: completely optional performance testing

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
			FactoryRef->AttachManifestToEnclave(tempEnclavePtr, 1);		// last argument is NEW PROTOTYPE DUMB VALUE
		}

		// Phase 3: Render actual collection
		RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval), 1);		// call function to add data into array; pass mutex to use;; // last argument is NEW PROTOTYPE DUMB VALUE
		RenderCollection* collPtr = &RenderCollectionsRef->RenderMatrix[Key1];
		//cout << "Total renderables for Key (" << Key1.x << ", " << Key1.y << ", " << Key1.z << ") :" << tempdumbcount << ": " << collPtr->RenderCollectionArraySize << endl;
		//trueend = std::chrono::high_resolution_clock::now();

	}
	trueend = std::chrono::high_resolution_clock::now();

}

void OrganicSystem::JobMaterializeMultiCollectionFromFactory2(MDListJobMaterializeCollection* mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID)
{
	int totalProcessed = 0;
	/* Summary: this method materializes one or more EnclaveCollections, by using a Factory */
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
	for (JobIterator; JobIterator != JobIteratorEnd; ++JobIterator)
	{
		//truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
		auto initstart = std::chrono::high_resolution_clock::now();												// for performance testing only
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;												// set the EnclaveKey for this loop iteration	
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];						// set a pointer to the appropriate blueprint
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;							// set a pointer to the actual EnclaveCollection


		auto initend = std::chrono::high_resolution_clock::now();												// for performance testing only
		std::chrono::duration<double> initelapsed = initend - initstart;										// ""

		EnclaveCollectionActivateListT2 listT2_1;		
		//cout << "test within blueprint processing: " << blueprintptr->WestBorderBlocks.faceflagarray[1] << endl;
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
		//cout << "dictionary debug check (0) " << endl;
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
			FactoryRef->AttachManifestToEnclave(tempEnclavePtr, 1);		// last argument is NEW PROTOTYPE DUMB VALUE
		}

		// Phase 3: Render actual collection
		//cout << "Phase 3 beginning..." << endl;
		RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval), 1);		// call function to add data into array; pass mutex to use;; // last argument is NEW PROTOTYPE DUMB VALUE
		//RenderCollection* collPtr = &RenderCollectionsRef->RenderMatrix[Key1];
		//cout << "Total renderables for Key (" << Key1.x << ", " << Key1.y << ", " << Key1.z << ") :" << tempdumbcount << ": " << collPtr->RenderCollectionArraySize << endl;
		//trueend = std::chrono::high_resolution_clock::now();
		
		totalProcessed++;
	}
	trueend = std::chrono::high_resolution_clock::now();

}

void OrganicSystem::JobMaterializeCollectionFromFactoryViaMorph(MDJobMaterializeCollection* mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef)
{
	// firstly, clean out / initialize the Factory collection
	auto truestart = std::chrono::high_resolution_clock::now();		// optional, for performance testing only	
	Enclave tempEnclave;
	FactoryRef->FactoryCollections[0].EnclaveArray[0][0][0] = tempEnclave;
	int mdjobRenderMode = mdjob->currentRenderMode;
	// cout << "TEST: mdjobRenderMode is: " << mdjobRenderMode;
	// get a pointer to above collection
	EnclaveCollection* CollectionRef = &FactoryRef->FactoryCollections[0];

	// set up pointers
	
	EnclaveCollectionBlueprintMatrix *BlueprintMatrixRef = mdjob->MDBlueprintMatrixRef;			// set Blueprint matrix ref
	EnclaveCollectionMatrix *EnclaveCollectionsRef = mdjob->MDEnclaveCollectionsRef;			// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix *RenderCollectionsRef = mdjob->MDRenderCollectionsRef;				// set a ref to the RenderCollection matrix
	EnclaveCollectionActivateListT2 listT2_1;													// set up a activate list
	EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef->BlueprintMap[mdjob->MDKey];			// the blueprint to use

	////////////// BEGIN PROCESSING	(33% of estimated heavy workload (11/1/2017))
	auto truestart2 = std::chrono::high_resolution_clock::now();		// optional, for performance testing only
	EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha2(0, 7 + 1, std::ref(*CollectionRef), mdjob->MDKey, std::ref(blueprintptr), std::ref(BlueprintMatrixRef), std::ref(listT2_1), std::ref(mutexval));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
	
	//auto trueend2 = std::chrono::high_resolution_clock::now();		// optional, for performance testing only	
	//std::chrono::duration<double> trueelapsed2 = trueend2 - truestart2;

	int chunkbitmask = 1;	// set the chunk bit mask used below
	int bitmaskval = 0;		// ""
	int renderablecount = 0;

	// Phase 1: EnclaveCollection instantiation

	// reset total renderable enclaves value
	CollectionRef->totalRenderableEnclaves = 0;

	
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
		//cout << "manifestCounter loop pass" << endl;
		FactoryRef->AttachManifestToEnclave(tempEnclavePtr, mdjobRenderMode);
	}
	

	// Phase 3: Render actual collection
	//cout << "Phase 3 beginning..." << endl;
	//cout << "FACTORY WORK COMPLETE PASS..." << mdjob->MDKey.x << ", " << mdjob->MDKey.y << ", " << mdjob->MDKey.z << endl;
	RenderCollectionsRef->CreateRenderArrayFromFactoryMorph(mdjob->MDKey, FactoryRef, std::ref(mutexval), mdjobRenderMode);		// call function to add data into array; pass mutex to use


																									//RenderCollection* collPtr = &RenderCollectionsRef->RenderMatrix[Key1];
																									//cout << "Total renderables for Key (" << Key1.x << ", " << Key1.y << ", " << Key1.z << ") :" << tempdumbcount << ": " << collPtr->RenderCollectionArraySize << endl;
																									//trueend = std::chrono::high_resolution_clock::now();

	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//std::chrono::duration<double> unordered_elapsed = unordered_end - unordered_start;

	auto trueend2 = std::chrono::high_resolution_clock::now();		// optional, for performance testing only	
	std::chrono::duration<double> trueelapsed2 = trueend2 - truestart2;

}

int OrganicSystem::JobMaterializeCollectionFromMM(MDJobMaterializeCollection* mdjob, mutex& mutexval)
{
	//cout << "T1 job called!!! " << endl;
	EnclaveCollectionBlueprintMatrix* BlueprintMatrixRef = mdjob->MDBlueprintMatrixRef;		// set Blueprint matrix ref
	EnclaveCollectionMatrix* EnclaveCollectionsRef = mdjob->MDEnclaveCollectionsRef;		// set a ref to the EnclaveCollection matrix		
	RenderCollectionMatrix* RenderCollectionsRef = mdjob->MDRenderCollectionsRef;			// set a ref to the RenderCollection matrix

	EnclaveKeyDef::EnclaveKey Key1 = mdjob->MDKey;												// set the EnclaveKey for this loop iteration	
	EnclaveCollectionBlueprint* blueprintptr = &BlueprintMatrixRef->BlueprintMap[Key1];						// set a pointer to the appropriate blueprint
	EnclaveCollection* CollectionRef = mdjob->MDEnclaveCollectionPtr;							// set a pointer to the actual EnclaveCollection
	EnclaveCollectionStateArray* stateArrayPtr = mdjob->MDStateArrayPtr;
	int mdjobRenderMode = mdjob->currentRenderMode;					// NEW
	mutexval.lock();																					/*thread safety:		*/
	ManifestCollection *ManifestCollectionRef = mdjob->MDManifestCollectionPtr;			// set pointer in thread-safe code; another ManifestCollection could be allocating an EnclaveManifest on heap at same time.
	mutexval.unlock();																					// unlock when finished

	
	EnclaveCollectionActivateListT2 listT2_1;																									// creation an activation list for instantiating the enclaves
																																				//EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1));	// run the instantiation job on this thread (all 512 enclaves)
	//mutexval.lock();
	EnclaveCollectionsRef->JobInstantiateAndPopulateEnclaveAlpha2(0, 7 + 1, std::ref(*CollectionRef), Key1, std::ref(blueprintptr), std::ref(BlueprintMatrixRef), std::ref(listT2_1), std::ref(mutexval));	// run the instantiation job on this thread (all 512 enclaves) //EnclaveCollectionMap[Key]
	//mutexval.unlock();
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
																					//if (tempKey.x == 3 && tempKey.y == 1 && tempKey.z == 0)
																					//{
																					//cout << "enclave will be rendered..." << endl;
																					//}
																					//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
				}

			}
			//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
			chunkbitmask <<= 1;		// move the bitmask to the "left"
			bitmaskval++;			// increment bitmask val
		}
	}

	int manifestCounter = CollectionRef->totalRenderableEnclaves;	// set the manifestCounter equal to the number of renderable manifests from the EnclaveCollection ref
	auto start5 = std::chrono::high_resolution_clock::now();	// optional performance testing values
	EnclaveKeyDef::EnclaveKey innerTempKey;		// create a variable to store a temporary key

	//cout << "collection key value: " << Key1.x << ", " << Key1.y << ", " << Key1.z << endl;
	for (int a = 0; a < manifestCounter; a++)	// loop count is equal to the number of manifests to be rendered 
	{
		innerTempKey = CollectionRef->RenderableEnclaves[a];
		/*
		if (Key1.x == 0 && Key1.y == 0 && Key1.z == 0)
		{
			if (innerTempKey.x == 3 && innerTempKey.y == 1 && innerTempKey.z == 0)
			{
				cout << "---------------------------------innerTempKey: " << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << " | " << CollectionRef->EnclaveArray[4][1][0].TotalRenderable << endl;

			}
		}
		*/
		//cout << "innerTempKey: " << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << endl;
		ManifestCollectionRef->AddManifestToMatrix(innerTempKey.x, innerTempKey.y, innerTempKey.z, Key1, mdjobRenderMode, std::ref(mutexval));
	}

	auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
	std::chrono::duration<double> elapsed5 = finish5 - start5;									// ""

																								//cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[innerTempKey].TotalEnclaveTriangles << endl;				// RENAME THIS TO SOMETHING ELSE! (ManifestCollectionRef)
																								// Phase 3: Render collection set up
																											

	// Phase 3: create render array
	RenderCollectionsRef->CreateRenderArrayFromManifestCollection(Key1, std::ref(mutexval), mdjobRenderMode);						// creates the to-be rendered array, from a MM
	int findResult = stateArrayPtr->findIndexOfKeyToUpdate(Key1);
	return findResult;

}

void OrganicSystem::JobCalibrateBlueprintBordersFromFactory(EnclaveKeyDef::EnclaveKey Key1, EnclaveManifestFactoryT1 *FactoryRef)
{
	EnclaveCollectionBlueprint* blueprintPtr = &BlueprintMatrix.BlueprintMap[Key1];						// set a pointer to the appropriate blueprint
	EnclaveCollectionBlueprintMatrix* blueprintMatrixPtr = &BlueprintMatrix;
	EnclaveCollection* collectionPtr = FactoryRef->EnclaveCollectionPtr;								// get the pointer to the appropriate collection in the factory

	collectionPtr->SetupEnclaveDataFromSingleBlueprint(Key1, blueprintPtr, blueprintMatrixPtr);									// populate the enclave based on the blueprint
	blueprintPtr->CalibrateBlueprintBorders(collectionPtr);



}

void OrganicSystem::JobCalibrateBlueprintBordersFromFactory(EnclaveKeyDef::EnclaveKey Key1, EnclaveCollectionBlueprint* inBlueprintPtr, EnclaveManifestFactoryT1 *FactoryRef)
{
	EnclaveCollectionBlueprint* blueprintPtr = inBlueprintPtr;						// set a pointer to the appropriate blueprint
	EnclaveCollectionBlueprintMatrix* blueprintMatrixPtr = &BlueprintMatrix;
	EnclaveCollection* collectionPtr = FactoryRef->EnclaveCollectionPtr;								// get the pointer to the appropriate collection in the factory

	collectionPtr->SetupEnclaveDataFromSingleBlueprint(Key1, blueprintPtr, blueprintMatrixPtr);									// populate the enclave based on the blueprint
	blueprintPtr->CalibrateBlueprintBorders(collectionPtr);

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
		FactoryRef->AttachManifestToEnclave(tempEnclavePtr, 1);		// last argument is NEW PROTOTYPE DUMB VALUE

	}

	// Phase 3: Render actual collection
	RenderCollectionsRef->CreateRenderArrayFromFactory(Key1, FactoryRef, std::ref(mutexval), 1);		// call function to add data into array; pass mutex to use ;; // last argument is NEW PROTOTYPE DUMB VALUE
}

void OrganicSystem::SetGraphicsAPI()
{
	OGLM.InitializeOpenGL();

}

int OrganicSystem::SetRenderMode(int x)
{
	if (x == 0)			// mode 0: fragment shader only
	{
		OGLM.renderMode = 0;		// sets to default terrain render mode
		//OGLM.selectShader();		// selects the default shader (because renderMode was set to 0)
		int vertexTupleWidth = 12;
		return vertexTupleWidth;
	}

	if (x == 1)
	{
		OGLM.renderMode = 1;		// sets to vertex shading terrain render mode (non textured)
		//OGLM.selectShader();
		int vertexTupleWidth = 24;
		return vertexTupleWidth;
	}
}

void OrganicSystem::SelectShader()
{
	OGLM.selectShader();
}


void OrganicSystem::RenderGLTerrain()
{
	OGLM.RenderReadyArrays();
}

void OrganicSystem::DetermineMouseCursorTargets(glm::vec3* originVector, glm::vec3* directionVector, int length)
{
	

	// step 1: calculate slope
	glm::vec3 origin_point = *originVector;
	glm::vec3 direction_point = *directionVector;
	glm::vec3 rayDirection = direction_point - origin_point;

	int block_traverse_x = 0;
	int block_traverse_y = 0;
	int block_traverse_z = 0;

	//cout << "Slope: " << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << endl;

	// step 2: calculate length of ray between the two points
	
	float x_pow = pow(rayDirection.x, 2.0f);		// square difference in x, that is between two points
	float y_pow = pow(rayDirection.y, 2.0f);		// square difference in y, that is between two points
	float z_pow = pow(rayDirection.z, 2.0f);		// square difference in z, that is between two points
	float rayLength = sqrt(x_pow + y_pow + z_pow);
	//cout << "Ray Length: " << rayLength << endl;

	// step 3: calculate the end point of the ray, by dividing the desired length (from input parameter) by the rayLength result above.
	// we will multiply all points from rayLength by this value, to determine the coordinates of the point at the end of the ray.
	float rayMultiplier = length / rayLength;

	//cout << "Ray multiplier: " << rayMultiplier << endl;

	// step 4: calculate the offset of x/y/z needed to get to the end of the ray; do this by multiplying rayDirection values by the rayMultiplier.
	float x_offset = rayDirection.x * rayMultiplier;
	float y_offset = rayDirection.y * rayMultiplier;
	float z_offset = rayDirection.z * rayMultiplier;

	float x_offset_pow = pow(x_offset, 2.0f);
	float y_offset_pow = pow(y_offset, 2.0f);
	float z_offset_pow = pow(z_offset, 2.0f);
	float newRayLength = sqrt(x_offset_pow + y_offset_pow + z_offset_pow);

	//cout << "Final ray length test: " << newRayLength << endl;


	// step 6: determine distances to traverse, assuming origin's x/y/z is less than 1.0. 
	float tmax_x = 1.0f / rayDirection.x;	// x = 1
	float tmax_y = 1.0f / rayDirection.y;	// y = 1
	float tmax_z = 1.0f / rayDirection.z;	// z = 1 

	// step 5: get distances for Deltas

	///////////////////////////////////////////////////////////////////////////////////////////////////////////// Determine distance to max's (that is, "first x", "first y", "first z")

	// x delta
	float xDelta_Begin_distance = 1.0f - origin_point.x;							// get the distance traveled between x = 1.0f and x  = 0.2f
	float xDelta_multiplier = xDelta_Begin_distance * tmax_x;						// multiply the distance to traverse to get to x = 1.0 by  the multiplier for x (in this case, 5)	
	float xDelta_x_coord = origin_point.x + (rayDirection.x * xDelta_multiplier);	// get the value of x where x = 1
	float xDelta_y_coord = origin_point.y + (rayDirection.y * xDelta_multiplier);	// get the value of y where x = 1
	float xDelta_z_coord = origin_point.z + (rayDirection.z * xDelta_multiplier);	// get the value of z where x = 1
	float xDelta_x_pow = float(pow(xDelta_x_coord, 2.0f));
	float xDelta_y_pow = float(pow(xDelta_y_coord, 2.0f));
	float xDelta_z_pow = float(pow(xDelta_z_coord, 2.0f));
	float xInitialDeltaDistance = sqrt(xDelta_x_pow + xDelta_y_pow + xDelta_z_pow);

	// y delta
	float yDelta_Begin_distance = 1.0f - origin_point.y;
	float yDelta_multiplier = yDelta_Begin_distance * tmax_y;
	float yDelta_x_coord = origin_point.x + (rayDirection.x * yDelta_multiplier);
	float yDelta_y_coord = origin_point.y + (rayDirection.y * yDelta_multiplier);
	float yDelta_z_coord = origin_point.z + (rayDirection.z * yDelta_multiplier);
	float yDelta_x_pow = float(pow(yDelta_x_coord, 2.0f));
	float yDelta_y_pow = float(pow(yDelta_y_coord, 2.0f));
	float yDelta_z_pow = float(pow(yDelta_z_coord, 2.0f));
	float yInitialDeltaDistance = sqrt(yDelta_x_pow + yDelta_y_pow + yDelta_z_pow);

	// z delta
	float zDelta_Begin_distance = 1.0f - origin_point.z;
	float zDelta_multiplier = zDelta_Begin_distance * tmax_z;
	float zDelta_x_coord = origin_point.x + (rayDirection.x * zDelta_multiplier);
	float zDelta_y_coord = origin_point.y + (rayDirection.y * zDelta_multiplier);
	float zDelta_z_coord = origin_point.z + (rayDirection.z * zDelta_multiplier);
	float zDelta_x_pow = float(pow(zDelta_x_coord, 2.0f));
	float zDelta_y_pow = float(pow(zDelta_y_coord, 2.0f));
	float zDelta_z_pow = float(pow(zDelta_z_coord, 2.0f));
	float zInitialDeltaDistance = sqrt(zDelta_x_pow + zDelta_y_pow + zDelta_z_pow);

	//cout << "Delta checks: " << endl;

	//cout << "-------------checks for first x/y/z line cross --------------" << endl;
	//cout << "Delta x, x: " << xDelta_x_coord << endl;
	//cout << "Delta x, y: " << xDelta_y_coord << endl;
	//cout << "Delta x, z: " << xDelta_z_coord << endl;
	//cout << "Delta x distance to first point: " << xInitialDeltaDistance << endl;

	//cout << "Delta y, x: " << yDelta_x_coord << endl;
	//cout << "Delta y, y: " << yDelta_y_coord << endl;
	//cout << "Delta y, z: " << yDelta_z_coord << endl;
	//cout << "Delta y distance to first point: " << yInitialDeltaDistance << endl;

	//cout << "Delta z, x: " << zDelta_x_coord << endl;
	//cout << "Delta z, y: " << zDelta_y_coord << endl;
	//cout << "Delta z, z: " << zDelta_z_coord << endl;
	//cout << "Delta z distance to first point: " << zInitialDeltaDistance << endl;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////// Determine delta's of all x, y, z

	// x delta
	 xDelta_Begin_distance = 1.0f;							// get the distance traveled between x = 1.0f and x  = 0.2f
	 xDelta_multiplier = xDelta_Begin_distance * (1.0f / rayDirection.x);						// multiply the distance to traverse to get to x = 1.0 by  the multiplier for x (in this case, 5)	
	 xDelta_x_coord =  (rayDirection.x * xDelta_multiplier);	// get the value of x where x = 1
	 xDelta_y_coord =  (rayDirection.y * xDelta_multiplier);	// get the value of y where x = 1
	 xDelta_z_coord =  (rayDirection.z * xDelta_multiplier);	// get the value of z where x = 1
	 xDelta_x_pow = pow(xDelta_x_coord, 2.0f);
	 xDelta_y_pow = pow(xDelta_y_coord, 2.0f);
	 xDelta_z_pow = pow(xDelta_z_coord, 2.0f);
	 float xDeltaDistance = sqrt(xDelta_x_pow + xDelta_y_pow + xDelta_z_pow);

	// y delta
	 yDelta_Begin_distance = 1.0f;
	 yDelta_multiplier = yDelta_Begin_distance * (1.0f / rayDirection.y);
	 yDelta_x_coord =  (rayDirection.x * yDelta_multiplier);
	 yDelta_y_coord =  (rayDirection.y * yDelta_multiplier);
	 yDelta_z_coord =  (rayDirection.z * yDelta_multiplier);
	 yDelta_x_pow = pow(yDelta_x_coord, 2.0f);
	 yDelta_y_pow = pow(yDelta_y_coord, 2.0f);
	 yDelta_z_pow = pow(yDelta_z_coord, 2.0f);
	 float yDeltaDistance = sqrt(yDelta_x_pow + yDelta_y_pow + yDelta_z_pow);

	// z delta
	 zDelta_Begin_distance = 1.0f;
	 zDelta_multiplier = zDelta_Begin_distance * (1.0f / rayDirection.z);
	 zDelta_x_coord =  (rayDirection.x * zDelta_multiplier);
	 zDelta_y_coord =  (rayDirection.y * zDelta_multiplier);
	 zDelta_z_coord =  (rayDirection.z * zDelta_multiplier);
	 zDelta_x_pow = pow(zDelta_x_coord, 2.0f);
	 zDelta_y_pow = pow(zDelta_y_coord, 2.0f);
	 zDelta_z_pow = pow(zDelta_z_coord, 2.0f);
	 float zDeltaDistance = sqrt(zDelta_x_pow + zDelta_y_pow + zDelta_z_pow);



	//cout << "-------------checks for true x/y/z line delta --------------" << endl;

	//cout << "x delta traversal: " << xDeltaDistance << endl;
	//cout << "y delta traversal: " << yDeltaDistance << endl;
	//cout << "z delta traversal: " << zDeltaDistance << endl;









	float xDelta_End = 2.0f;

	//cout << "Traversal values: " << endl;
	//cout << "x: " << tmax_x << endl;
	//cout << "y: " << tmax_y << endl;
	//cout << "z: " << tmax_z << endl;

	// for later: calculate delta by taking the total time to traverse between x = 2.0 and x = 1.0. Use this continuously for rest of function, for x/y/z.
	if (tmax_x < tmax_y)
	{

	}



}

void OrganicSystem::DetermineMouseCursorTargets2(glm::vec3* originVector, glm::vec3* directionVector, int length)
{
	blockTargetMeta.hasAcquiredTarget = 0;			// reset blockTargetMeta's acquisition flag to 0


	//cout << endl;
	// step 1: calculate direction and slope
	glm::vec3 origin_point = *originVector;						// original point being checked
	glm::vec3 direction_point = *directionVector;				// direction vector based on mouse input
	glm::vec3 center_point = origin_point;
	//cout << "Pre-standardization of points: " << origin_point.x << ", " << origin_point.y << ", " << origin_point.z << endl;
	//cout << ">>>>Pre-Direction: (" << direction_point.x << ", " << direction_point.y << ", " << direction_point.z << ") " << endl;
	// perform centering of x coordinates

	
	if (center_point.x <= 0)
	{
		// if the the floor of center_point.x is equal to the origin_point (meaning the point was for example at 3.0f), subtract 5
		if (floor(center_point.x) == origin_point.x)
		{
			//cout << "(NEG X) |||| x points are a .0f" << endl;
			origin_point.x -= 0.0005f;
		}

	}
	else if (center_point.x > 0)
	{
		if (ceil(center_point.x) == origin_point.x)
		{
			//cout << "(POS X) |||| x points are a .0f" << endl;
			origin_point.x += 0.0005f;
		}

	}




	// perform centering of y coordinates
	if (center_point.y <= 0)
	{
		// if the the floor of center_point.y is equal to the origin_point (meaning the point was for example at 3.0f), subtract 5
		if (floor(center_point.y) == origin_point.y)
		{
			//cout << "(NEG Y) |||| y points are a .0f" << endl;
			origin_point.y -= 0.0005f;
		}

	}
	else if (center_point.y > 0)
	{
		if (ceil(center_point.y) == origin_point.y)
		{
			//cout << "(POS Y) |||| y points are a .0f" << endl;
			origin_point.y += 0.0005f;
		}

	}
	


	// perform centering of z coordinates
	if (center_point.z <= 0)
	{
		// if the the floor of center_point.y is equal to the origin_point (meaning the point was for example at 3.0f), subtract 5
		if (floor(center_point.z) == origin_point.z)
		{
			//cout << "(NEG Z) |||| z points are a .0f" << endl;
			origin_point.z -= 0.0005f;
		}

	}
	else if (center_point.z > 0)
	{
		if (ceil(center_point.z) == origin_point.z)
		{
			//cout << "(POS Z) |||| z points are a .0f" << endl;
			origin_point.z += 0.0005f;
			
		}

	}
	

	//glm::vec3 direction_point = *directionVector;				// direction vector based on mouse input
	//cout << "Original vector (" << originVector->x << ", " << originVector->y << ", " << originVector->z << ") " << endl;

	//cout << "Post-standardization of Origin point: (" << origin_point.x << ", " << origin_point.y << ", " << origin_point.z << ") " << endl;
	//cout << ">>>>>Direction: (" << direction_point.x << ", " << direction_point.y << ", " << direction_point.z << ") " << endl;

	
	int block_traverse_x = 0;
	int block_traverse_y = 0;
	int block_traverse_z = 0;

	// second, set up current collection, chunk in collection, and block in chunk
	CursorPathTraceContainer x_container, y_container, z_container;
	x_container = EnclaveCollections.GetCursorCoordTrace(origin_point.x);
	y_container = EnclaveCollections.GetCursorCoordTrace(origin_point.y);
	z_container = EnclaveCollections.GetCursorCoordTrace(origin_point.z);

	// set values for Camera keys
	EnclaveKeyDef::EnclaveKey lastCollectionKey, newCollectionKey;

	// get the key for the previous collection, and put it into the temp value
	lastCollectionKey.x = PreviousCCKey.x;
	lastCollectionKey.y = PreviousCCKey.y;
	lastCollectionKey.z = PreviousCCKey.z;

	
	
	// set keys
	CameraCollectionKey.x = int(x_container.CollectionCoord);
	CameraCollectionKey.y = int(y_container.CollectionCoord);
	CameraCollectionKey.z = int(z_container.CollectionCoord);

	CameraChunkKey.x = int(x_container.ChunkCoord);
	CameraChunkKey.y = int(y_container.ChunkCoord);
	CameraChunkKey.z = int(z_container.ChunkCoord);

	CameraBlockKey.x = int(x_container.BlockCoord);
	CameraBlockKey.y = int(y_container.BlockCoord);
	CameraBlockKey.z = int(z_container.BlockCoord);

	// get the current key for the collection, and put it into the temp value
	newCollectionKey.x = CameraCollectionKey.x;
	newCollectionKey.y = CameraCollectionKey.y;
	newCollectionKey.z = CameraCollectionKey.z;
	EnclaveCollectionMatrix* ECMPointer = &EnclaveCollections;
	if (lastCollectionKey.x != newCollectionKey.x || lastCollectionKey.y != newCollectionKey.y || lastCollectionKey.z != newCollectionKey.z)
	{
		// shift of matrix collection pointers will be here (9/28/2017); this needs to be done before EnclaveBlockRayTracker (aka rayTracker) does its calculations (see below)
		// CollectionStateArray.? (? == placeholder for function name)
		CollectionStateArray.ShiftCenterCollection(PreviousCCKey, CameraCollectionKey, ECMPointer);			// shift the collection state array
		OGLM.OrganicBufferManager.setShiftedCollectionKeys(lastCollectionKey, newCollectionKey);									// send the shifted keys to the OGLM
		//cout << "collection has changed! Old: (" << lastCollectionKey.x << ", " << lastCollectionKey.y << " , " << lastCollectionKey.z << ") || New: (" << newCollectionKey.x << ", " << newCollectionKey.y << ", " << newCollectionKey.z << ") " << endl;
	}

	// set PreviousCCKey to new key
	PreviousCCKey.x = newCollectionKey.x;
	PreviousCCKey.y = newCollectionKey.y;
	PreviousCCKey.z = newCollectionKey.z;

	// cout << "Origin point z: " << origin_point.z <<"Collection: (" << CameraCollectionKey.x << ", " << CameraCollectionKey.y << ", " << CameraCollectionKey.z << ") || Enclave: (" << CameraChunkKey.x << ", " << CameraChunkKey.y << ", " << CameraChunkKey.z << " ) || Block: " << CameraBlockKey.x << ", " << CameraBlockKey.y << ", " << CameraBlockKey.z  << ")" << endl;
	// get the temp origin. Temp_origin stores the exact x/y/z coordinates, being greater >= 0, and < 1 (?). This is used to compare to 1.0f values used below.
	float block_x_precise = x_container.ExactBlockCoord;	// get the value for the exact x/y/z coordinates within a block
	float block_y_precise = y_container.ExactBlockCoord;
	float block_z_precise = z_container.ExactBlockCoord;
	glm::vec3 temp_origin;									// set up a vector that represents the origin (will be used 3 times below); temp origin is the exact block coordinates 
	temp_origin.x = block_x_precise;
	temp_origin.y = block_y_precise;
	temp_origin.z = block_z_precise;
	



	glm::vec3 second_point = temp_origin + direction_point;		// add the direction to the temp_origin, store the result in second_point
	glm::vec3 rayDirection = second_point - temp_origin;		// the difference between the second_point and the temp_origin will determine the rayDirection.
	//cout << ">>>>>RayDirection: (" << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << ") " << endl;

	
	//cout << "----BEGIN ANALYSIS (begin point) ----" << endl;
	//cout << "Origin Point: (" << origin_point.x << ", " << origin_point.y << ", " << origin_point.z << ") " << endl;
	//cout << "Collection: (" << CameraCollectionKey.x << ", " << CameraCollectionKey.y << ", " << CameraCollectionKey.x << ") " << endl;
	//cout << "Chunk: (" << CameraChunkKey.x << ", " << CameraChunkKey.y << ", " << CameraChunkKey.z << ") " << endl;
	//cout << "Block: (" << CameraBlockKey.x << ", " << CameraBlockKey.y << ", " << CameraBlockKey.z << ") " << endl;
	//cout << "Exact Block: (" << block_x_precise << ", " << block_y_precise << ", " << block_z_precise << ") " << endl;
	//cout << ">>>>Post-Direction: (" << direction_point.x << ", " << direction_point.y << ", " << direction_point.z << ") " << endl;
	

	//cout << "Slope: (" << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << ") " << endl;

	// cout << "|| test of y ray direction: " << rayDirection.y << endl;
	// --------------------------------------- PHASE 1 BEGIN: determine intial traversal time for x/y/z, determine deltas (distance traveled along ray) for initial x/y/z traversal

	float time_to_complete_x_traversal;		// may be an unnecessary calculation; test later
	float time_to_complete_y_traversal;
	float time_to_complete_z_traversal;

	float initial_xMax;		// the initial time it takes to get from the origin to the border 
	float initial_yMax;
	float initial_zMax;

	float normal_xMax;		// the standard time it takes to get from 0.0f to 1.0f
	float normal_yMax;
	float normal_zMax;

	float initial_xLength;	// the initial length to traverse from the origin to the border
	float initial_yLength;
	float initial_zLength;

	glm::vec3 distance_between_points_x;	// the length of the ray between the origin and the border
	glm::vec3 distance_between_points_y;
	glm::vec3 distance_between_points_z;


	// determine if the distance to 1.0 or -1.0 for x is based on "distance_to_pos" or "distance_to_neg"
	float x_border_distance;
	if (rayDirection.x >= 0.0f)		// if the direction of rayDirection.x is positive, use the distance to positive.
	{
		//cout << "Ray's x is greater than 0" << endl;
		x_border_distance = x_container.distance_to_pos;								// the value of x_border_distance will be based on distance_to_pos
		float border_to_compare_x = 1.0f;												// because the direction of x is positive, compare it to 1.0.
		float origin_to_border_x_diff = border_to_compare_x - temp_origin.x;			// subtract the origin from the value of 1.0f. For example, if origin is at x = 0.2f, result would be 0.8f.
		time_to_complete_x_traversal = origin_to_border_x_diff / rayDirection.x;		// determine the multiplier needed to get to x = 1.0f, by using x's slope coming from the temp_origin.x value.

		// next, determine length of ray between the origin_point and the point where x = 1.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = 1.0f;
		border_point.y = temp_origin.y + (rayDirection.y*time_to_complete_x_traversal);				// add slope*time to temp_origin.y
		border_point.z = temp_origin.z + (rayDirection.z*time_to_complete_x_traversal);				// add slope*time to temp_origin.z

		distance_between_points_x = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_x.x, 2.0f));		// calculate squared values for x/y/z
		float squared_y = float(pow(distance_between_points_x.y, 2.0f));
		float squared_z = float(pow(distance_between_points_x.z, 2.0f));
		//cout << "temp_origin.x: " << temp_origin.x << endl;
		//cout << "rayDirection.x: " << rayDirection.x << endl;
		initial_xMax = (1.0f - temp_origin.x) / rayDirection.x;			// get the time it takes to get to 1.0f from the origin
		initial_xLength = sqrt(squared_x + squared_y + squared_z);		// calculate the initial length
		normal_xMax = (1.0f / rayDirection.x);							// the normal amount of time it takes to traverse a distance of exactly 1.0f
	}
	else
	{
		x_border_distance = x_container.distance_to_neg;								// the value of x_border_distance will be based on distance_to_neg
		//float border_to_compare_x = 1.0f;												// because the direction of x is negative, compare it to 0.0
		//float origin_to_border_x_diff = border_to_compare_x - temp_origin.x;			// add the origin to the value of 0.0f. For example, if origin is at 0.2f, the result would be 0.2f.
		float origin_to_border_x_diff = temp_origin.x;
		if (origin_to_border_x_diff == 0)
		{
			origin_to_border_x_diff = 1.0f;
		}
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / rayDirection.x); // determine the multiplier needed to get to x = 0.0f, by using x's slope coming from the temp_origin.x value; use abs to get the absolute value (since we are going negative)

		// next, determine length of ray between the origin_point and the point where x = 0.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = 1.0f;
		border_point.y = temp_origin.y + abs(rayDirection.y*time_to_complete_x_traversal);
		border_point.z = temp_origin.z + abs(rayDirection.z*time_to_complete_x_traversal);

		distance_between_points_x = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_x.x, 2.0f));		// calculate squared values for x/y/z
		float squared_y = float(pow(distance_between_points_x.y, 2.0f));
		float squared_z = float(pow(distance_between_points_x.z, 2.0f));
		initial_xMax = abs((origin_to_border_x_diff) / rayDirection.x);	// get the time it takes to get to 1.0f from the origin
		initial_xLength = sqrt(squared_x + squared_y + squared_z);		// calculate the initial length
		normal_xMax = abs(1.0f / rayDirection.x);						// the normal amount of time it takes to traverse a distance of exactly 1.0f


	}



	// determine if the distance to 1.0 or -1.0 for y is based on "distance_to_pos" or "distance_to_neg"
	float y_border_distance;
	if (rayDirection.y >= 0.0f)		// if the direction of rayDirection.y is positive, use the distance to positive.
	{
		y_border_distance = y_container.distance_to_pos;								// the value of y_border_distance will be based on distance_to_pos
		float border_to_compare_y = 1.0f;												// because the direction of y is positive, compare it to 1.0.
		float origin_to_border_y_diff = border_to_compare_y - temp_origin.y;			// subtract the origin from the value of 1.0f. For example, if origin is at x = 0.2f, result would be 0.8f.
		time_to_complete_y_traversal = origin_to_border_y_diff / rayDirection.y;	// determine the multiplier needed to get to y = 1.0f, by using y's slope coming from the temp_origin.y value.

		// next, determine length of ray between the origin_point and the point where y = 1.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = temp_origin.x + (rayDirection.x*time_to_complete_y_traversal);				// add slope*time to temp_origin.x
		border_point.y = 1.0f;
		border_point.z = temp_origin.z + (rayDirection.z*time_to_complete_y_traversal);				// add slope*time to temp_origin.z

		distance_between_points_y = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_y.x, 2.0f));		// calculate squared values for x/y/z
		float squared_y = float(pow(distance_between_points_y.y, 2.0f));
		float squared_z = float(pow(distance_between_points_y.z, 2.0f));
		initial_yMax = (1.0f - temp_origin.y) / rayDirection.y;			// get the time it takes to get to 1.0f from the origin
		initial_yLength = sqrt(squared_x + squared_y + squared_z);		// calculate the initial length
		normal_yMax = 1.0f / rayDirection.y;							// the normal amount of time it takes to traverse a distance of exactly 1.0f
	}
	else
	{
		y_border_distance = y_container.distance_to_neg;								// the value of y_border_distance will be based on distance_to_neg
		float origin_to_border_y_diff = temp_origin.y;
		if (origin_to_border_y_diff == 0)
		{
			origin_to_border_y_diff = 1.0f;
		}
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / rayDirection.y);	// determine the multiplier needed to get to y = 0.0f, by using y's slope coming from the temp_origin.y value; use abs to get the absolute value (since we are going negative)

		// next, determine length of ray between the origin_point and the point where y = 0.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = temp_origin.x + abs(rayDirection.x*time_to_complete_y_traversal);			// add slope*time to temp_origin.x
		border_point.y = 1.0f;
		border_point.z = temp_origin.z + abs(rayDirection.z*time_to_complete_y_traversal);			// add slope*time to temp_origin.z

		distance_between_points_y = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_y.x, 2.0f));		// calculate squared values for x/y/z
		float squared_y = float(pow(distance_between_points_y.y, 2.0f));
		float squared_z = float(pow(distance_between_points_y.z, 2.0f));
		initial_yMax = abs((origin_to_border_y_diff) / rayDirection.y);	// get the time it takes to get to 1.0f from the origin
		initial_yLength = sqrt(squared_x + squared_y + squared_z);		// calculate the initial length
		normal_yMax = abs(1.0f / rayDirection.y);						// the normal amount of time it takes to traverse a distance of exactly 1.0f

	}



	// determine if the distance to 1.0 or -1.0 for z is based on "distance_to_pos" or "distance_to_neg"
	float z_border_distance;
	if (rayDirection.z >= 0.0f)		// if the direction of rayDirection.z is positive, use the distance to positive.
	{
		//cout << "z border checks..." << endl;
		z_border_distance = z_container.distance_to_pos;	// the value of z_border_distance will be based on distance_to_pos
		float border_to_compare_z = 1.0f;
		float origin_to_border_z_diff = border_to_compare_z - temp_origin.z;
		//cout << "temp origin z" << temp_origin.z << endl;
		//cout << "origin_to_broder_z_diff: " << origin_to_border_z_diff << endl;
		time_to_complete_z_traversal = origin_to_border_z_diff / rayDirection.z;

		// next, determine length of ray between the origin_point and the point where z = 1.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = temp_origin.x + (rayDirection.x*time_to_complete_z_traversal);
		border_point.y = temp_origin.y + (rayDirection.y*time_to_complete_z_traversal);
		border_point.z = 1.0f;

		distance_between_points_z = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_z.x, 2.0f));
		float squared_y = float(pow(distance_between_points_z.y, 2.0f));
		float squared_z = float(pow(distance_between_points_z.z, 2.0f));

		//cout << "temp_origin.z: " << temp_origin.z << endl;
		//cout << "rayDirection.z: " << rayDirection.z << endl;
		initial_zMax = (1.0f - temp_origin.z) / rayDirection.z;			// get the time it takes to get to 1.0f
		initial_zLength = sqrt(squared_x + squared_y + squared_z);
		normal_zMax = 1.0f / rayDirection.z;
	}
	else
	{
		z_border_distance = z_container.distance_to_neg;	// ...otherwise use the negative value
		//float border_to_compare_z = 1.0f;
		//float origin_to_border_z_diff = border_to_compare_z - temp_origin.z;
		float origin_to_border_z_diff = temp_origin.z;
		if (origin_to_border_z_diff == 0)
		{
			origin_to_border_z_diff = 1.0f;
		}
		time_to_complete_z_traversal = origin_to_border_z_diff / rayDirection.z;

		// next, determine length of ray between the origin_point and the point where z = 1.0f (pythagorean)
		glm::vec3 border_point;
		border_point.x = temp_origin.x + abs(rayDirection.x*time_to_complete_z_traversal);
		border_point.y = temp_origin.y + abs(rayDirection.y*time_to_complete_z_traversal);
		border_point.z = 1.0f;

		distance_between_points_z = border_point - temp_origin;			// get the difference between the border point and the origin point
		float squared_x = float(pow(distance_between_points_z.x, 2.0f));
		float squared_y = float(pow(distance_between_points_z.y, 2.0f));
		float squared_z = float(pow(distance_between_points_z.z, 2.0f));
		initial_zMax = abs((origin_to_border_z_diff) / rayDirection.z);	// get the time it takes to get to 1.0f
		initial_zLength = sqrt(squared_x + squared_y + squared_z);
		normal_zMax = abs(1.0f / rayDirection.z);


	}

	/*
	cout << "Traversal time x: " << time_to_complete_x_traversal << endl;
	cout << "Traversal time y: " << time_to_complete_y_traversal << endl;
	cout << "Traversal time z: " << time_to_complete_z_traversal << endl;

	cout << "Initial x delta time traverse value: " << initial_xMax << endl;
	cout << "Initial y delta time traverse value: " << initial_yMax << endl;
	cout << "Initial z delta time traverse value: " << initial_zMax << endl;

	cout << "Initial x length " << initial_xLength << endl;
	cout << "Initial y length " << initial_yLength << endl;
	cout << "Initial z length " << initial_zLength << endl;

	cout << "Standard 0 to 1.0f x traverse time value: " << normal_xMax << endl;
	cout << "Standard 0 to 1.0f y traverse time value: " << normal_yMax << endl;
	cout << "Standard 0 to 1.0f z traverse time value: " << normal_zMax << endl;
	*/
	

	//cout << "|| pre y-multiplier, distance between points: " << distance_between_points_y.y << endl;
	float xDelta_multiplier = abs(1.0f / distance_between_points_x.x);			// value to multiply to get the Delta for x between point 0.0f and 1.0f
	float yDelta_multiplier = abs(1.0f / distance_between_points_y.y);			// value to multiply to get the Delta for y between point 0.0f and 1.0f	
	float zDelta_multiplier = abs(1.0f / distance_between_points_z.z);			// value to multiply to get the Delta for z between point 0.0f and 1.0f
	float xDelta_final = initial_xMax * xDelta_multiplier;						// xDelta_final is the true delta distance to traverse for a start value of x = 0.0, and end value of 1.0 on this ray
	float yDelta_final = initial_yMax * yDelta_multiplier;						// yDelta_final is the true delta distance to traverse for a start value of y = 0.0, and end value of 1.0 on this ray
	float zDelta_final = initial_zMax * zDelta_multiplier;						// zDelta_final is the true delta distance to traverse for a start value of z = 0.0, and end value of 1.0 on this ray

	//cout << "xDelta final: " << xDelta_final << endl;
	//cout << "yDelta final: " << yDelta_final << endl;
	//cout << "zDelta final: " << zDelta_final << endl;


	float x_pow = float(pow(rayDirection.x, 2.0f));		// square difference in x, that is between two points
	float y_pow = float(pow(rayDirection.y, 2.0f));		// square difference in y, that is between two points
	float z_pow = float(pow(rayDirection.z, 2.0f));		// square difference in z, that is between two points
	float rayLength = sqrt(x_pow + y_pow + z_pow);		// use ray length to determine end of while loop
	//cout << "Ray Length: " << rayLength << endl;

	// step 3: calculate the end point of the ray, by dividing the desired length (from input parameter) by the rayLength result above.
	// we will multiply all points from rayLength by this value, to determine the coordinates of the point at the end of the ray.
	float rayMultiplier = length / rayLength;

	// calculate the least minimums, based on initial maxes
	int initialFlagLoop = 0;			// indicates what to do after first iteration

	/**In this loop, traverse along the ray until a block is found. Call EnclaveBlockRayTracker.search() until it returns an appropriate value. **/
	// create an instance of EnclaveBlockRayTracker for traversal; pass in x/y/z containers and a pointer to the EnclaveCollection located at the center of the array in EnclaveCollectionStateArray.StateMatrix
	int trackResult = 0;				
	//EnclaveBlockRayTracker rayTracker(x_container, y_container, z_container, CollectionStateArray.StateMatrix[1][1][1].collectionPtr);
	//cout << "----Ray tracker begin point----" << endl;
	//cout << "Collection: (" << CameraCollectionKey.x << ", " << CameraCollectionKey.y << ", " << CameraCollectionKey.x << ") " << endl;
	//cout << "Chunk: (" << CameraChunkKey.x << ", " << CameraChunkKey.y << ", " << CameraChunkKey.z << ") Block: (" << CameraBlockKey.x << ", " << CameraBlockKey.y << ", " << CameraBlockKey.z << ") " << endl;
	// cout << "Block: (" << CameraBlockKey.x << ", " << CameraBlockKey.y << ", " << CameraBlockKey.z << ") " << endl;


	int indexval = CollectionStateArray.translateXYZToSingle(CollectionStateArray.centerCollectionStateOffset, CollectionStateArray.centerCollectionStateOffset, CollectionStateArray.centerCollectionStateOffset);	// get the center of the dynamic array
	EnclaveCollectionStateArray* stateArrayPtr = &CollectionStateArray;
	OrganicBlockTarget* blockTargetPtr = &blockTargetMeta;
	EnclaveBlockRayTracker rayTracker(x_container, y_container, z_container, CollectionStateArray.StateMtxPtr, stateArrayPtr, indexval, blockTargetPtr);
	
	//cout << "traverse pass" << endl;
	int maxTravelAttempts = length;		// set travel (traversal) attempts to 10
	int travelAttempts = 0;				// set counter to 0

	//cout << "initial x: " << initial_xMax << endl;
	//cout << "initial y: " << initial_yMax << endl;
	//cout << "initial z: " << initial_zMax << endl;

	//cout << "Normal x time: " << normal_xMax << endl;
	//cout << "Normal y time: " << normal_yMax << endl;
	//cout << "Normal z time: " << normal_zMax << endl;

	//cout << "pre-while loop" << endl;
	while ((travelAttempts < maxTravelAttempts) && trackResult == 0)
	{
		if (initial_xMax < initial_yMax)
		{
			if (initial_xMax < initial_zMax)
			{


				if (rayDirection.x >= 0.0f)			// if x was positive, traverse +1
				{
					block_traverse_x += 1;
					trackResult = rayTracker.MoveEast();			// move one block east
				}
				else								// otherwise, traverse -1
				{
					block_traverse_x -= 1;
					trackResult = rayTracker.MoveWest();			// move one block west
				}
				initial_xMax = initial_xMax + normal_xMax;
				initialFlagLoop = 1;
			}
			else
			{

				if (rayDirection.z >= 0.0f)			// if z was positive, traverse +1
				{
					block_traverse_z += 1;
					trackResult = rayTracker.MoveSouth();			// move one block south
				}
				else								// otherwise, traverse -1
				{
					block_traverse_z -= 1;
					trackResult = rayTracker.MoveNorth();			// move one block north
				}
				initial_zMax = initial_zMax + normal_zMax;
				initialFlagLoop = 1;
			}
		}
		else
		{
			if (initial_yMax < initial_zMax)
			{

				if (rayDirection.y >= 0.0f)			// if y was positive, traverse +1
				{
					block_traverse_y += 1;			
					trackResult = rayTracker.MoveAbove();			// move one block above
				}
				else								// otherwise, traverse -1
				{
					block_traverse_y -= 1;
					trackResult = rayTracker.MoveBelow();			// move one block below
					//cout << "moved below..." << endl;
				}
				initial_yMax = initial_yMax + normal_yMax;
				initialFlagLoop = 1;
			}
			else
			{

				if (rayDirection.z >= 0.0f)			// if z was positive, traverse +1
				{
					block_traverse_z += 1;
					trackResult = rayTracker.MoveSouth();			// move one block south
				}
				else								// otherwise, traverse -1
				{
					block_traverse_z -= 1;
					trackResult = rayTracker.MoveNorth();			// move one block north
				}
				initial_zMax = initial_zMax + normal_zMax;
				initialFlagLoop = 1;
			}
		}
		if (trackResult == 1)
		{
			cout << "Unveil block found: Collection (" << rayTracker.collectionKey.x << ", " << rayTracker.collectionKey.y << ", " << rayTracker.collectionKey.z << ") | Enclave (" << rayTracker.enclaveKey.x << ", " << rayTracker.enclaveKey.y << ", " << rayTracker.enclaveKey.z << ") | Block: (" << rayTracker.blockKey.x << ", " << rayTracker.blockKey.y << ", " << rayTracker.blockKey.z << ") " <<  endl;
			//rayTracker.printOutTargetBlockHighlightData();
		}
		/*
		if (trackResult == 0)
		{
			//cout << "----track attempt end----" << endl;
			//cout << "Unveil block NOT found: Collection (" << rayTracker.collectionKey.x << ", " << rayTracker.collectionKey.y << ", " << rayTracker.collectionKey.z << ") | Enclave (" << rayTracker.enclaveKey.x << ", " << rayTracker.enclaveKey.y << ", " << rayTracker.enclaveKey.z << ") | Block: (" << rayTracker.blockKey.x << ", " << rayTracker.blockKey.y << ", " << rayTracker.blockKey.z << ") " << endl;
		}
		*/


		travelAttempts++;
		if (travelAttempts == length)
		{
			//cout << "attempt limit reached." << endl;
		}
	}

	
	/*
	if (trackResult == 0)
	{
		//cout << "----track attempt end----" << endl;
		//cout << "Unveil block NOT found: Collection (" << rayTracker.collectionKey.x << ", " << rayTracker.collectionKey.y << ", " << rayTracker.collectionKey.z << ") | Enclave (" << rayTracker.enclaveKey.x << ", " << rayTracker.enclaveKey.y << ", " << rayTracker.enclaveKey.z << ") | Block: (" << rayTracker.blockKey.x << ", " << rayTracker.blockKey.y << ", " << rayTracker.blockKey.z << ") " << endl;
	}
	*/
	//cout << "number of x traversals: " << block_traverse_x << endl;
	//cout << "number of y traversals: " << block_traverse_y << endl;
	//cout << "number of z traversals: " << block_traverse_z << endl;




}

void OrganicSystem::ListEnclaveCollectionsInMatrix()
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionMatrixIterBegin = EnclaveCollections.EnclaveCollectionMap.begin();
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionMatrixIterEnd = EnclaveCollections.EnclaveCollectionMap.end();
	for (collectionMatrixIterBegin; collectionMatrixIterBegin != collectionMatrixIterEnd; ++collectionMatrixIterBegin)
	{
		EnclaveKeyDef::EnclaveKey iterKey = collectionMatrixIterBegin->first;
		cout << "Key found in matrix: " << iterKey.x << ", " << iterKey.y << ", " << iterKey.z << endl;
	}
}

void OrganicSystem::InitializeCollectionStateArray(float x, float y, float z)
{
	EnclaveKeyDef::EnclaveKey keyToPass;
	CursorPathTraceContainer x_container, y_container, z_container;
	x_container = EnclaveCollections.GetCursorCoordTrace(x);
	y_container = EnclaveCollections.GetCursorCoordTrace(y);
	z_container = EnclaveCollections.GetCursorCoordTrace(z);
	keyToPass.x = int(x_container.CollectionCoord);
	keyToPass.y = int(y_container.CollectionCoord);
	keyToPass.z = int(z_container.CollectionCoord);

	CollectionStateArray.CreateAndInitializeStateMatrix(T1_OGLMcubesize, x, y, z, keyToPass);
}

void OrganicSystem::SetupWorldArea(float x, float y, float z)
{
	// first, set up world camera
	SetWorldCameraPosition(x, y, z);


	// second, set up current collection, chunk in collection, and block in chunk
	CursorPathTraceContainer x_container, y_container, z_container;
	x_container = EnclaveCollections.GetCursorCoordTrace(x);
	y_container = EnclaveCollections.GetCursorCoordTrace(y);
	z_container = EnclaveCollections.GetCursorCoordTrace(z);

	//cout << "Camera's current collection: " << x_container.CollectionCoord << ", " << y_container.CollectionCoord << ", " << z_container.CollectionCoord << endl;
	//cout << "Camera's current chunk in collection: " << x_container.ChunkCoord << ", " << y_container.ChunkCoord << ", " << z_container.ChunkCoord << endl;
	//cout << "Camera's current block in chunk: " << x_container.BlockCoord << ", " << y_container.BlockCoord << ", " << z_container.BlockCoord << endl;

	// set values for Camera keys; old collection key + all "current" keys
	PreviousCCKey.x = int(x_container.CollectionCoord);
	PreviousCCKey.y = int(y_container.CollectionCoord);
	PreviousCCKey.z = int(z_container.CollectionCoord);

	CameraCollectionKey.x = int(x_container.CollectionCoord);
	CameraCollectionKey.y = int(y_container.CollectionCoord);
	CameraCollectionKey.z = int(z_container.CollectionCoord);

	CameraChunkKey.x = int(x_container.ChunkCoord);
	CameraChunkKey.y = int(y_container.ChunkCoord);
	CameraChunkKey.z = int(z_container.ChunkCoord);

	CameraBlockKey.x = int(x_container.BlockCoord);
	CameraBlockKey.y = int(y_container.BlockCoord);
	CameraBlockKey.z = int(z_container.BlockCoord);

	// set up the center collection and its neighbors
	EnclaveCollectionMatrix* matrixPtr = &EnclaveCollections;
	//CollectionStateArray.CreateStateMatrix(T1_OGLMcubesize);		// ensure the state matrix is equal to the appropriate size (do not hardcode!)
	CollectionStateArray.SetCenterCollectionDynamic(CameraCollectionKey, matrixPtr);
	CollectionStateArray.SetCenterCollection(CameraCollectionKey, matrixPtr);

	// OGLM.OrganicBufferManager.PopulateOGLMRMCArrays();	// now, populate the previously created arrays with default data
	//OGLM.OrganicBufferManager.PopulateOGLMRMCArrays(CameraCollectionKey);	// now, populate the previously created arrays with default data

}

void OrganicSystem::SetWorldCameraPosition(float x, float y, float z)
{
	glm::vec3* tempVecPtr = OGLM.positionVecPtr;	// grab the OGLM's pointer to the position vector;
	tempVecPtr->x = x;
	tempVecPtr->y = y;
	tempVecPtr->z = z;


}

void OrganicSystem::GLCleanup()
{
	OGLM.ShutdownOpenGL();
}


RenderCollection* OrganicSystem::GetRenderCollectionPtr(int x, int y, int z)
{
	/* Summary: returns a pointer to a RenderCollection, by using x/y/z passed in as input*/
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	RenderCollection *renderCollPtr = &RenderCollections.RenderMatrix[tempKey];
	return renderCollPtr;
}

void OrganicSystem::SendDataFromRenderPtrToGLBuffer(RenderCollection* renderCollectionPtr)
{
	//OGLM.sendRenderCollectionDataToBuffer(renderCollectionPtr);
	OGLM.sendRenderCollectionDataToBufferOnGameLoad(renderCollectionPtr);
}

void OrganicSystem::LoadVCDataToGLBuffer(RenderCollection* renderCollectionPtr)
{
	//OGLM.sendRenderCollectionVCDataToBuffer(renderCollectionPtr);
	OGLM.sendRenderCollectionVCDataTOBufferOnGameLoad(renderCollectionPtr);
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
		//char factoryprefix[] = "Factory ";
		//string factorystring(factoryprefix);
		//string factorynumber = to_string(x);
		//string factory = factorystring + factorynumber;
		//cout << "Factory: " << factory << endl;
		//OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
		OrganicFactoryIndex.FactoryMap[x].StorageArray[0].VertexArrayCount = 0;
		OrganicFactoryIndex.FactoryMap[x].InsertEnclaveCollectionIntoFactory();
		OrganicFactoryIndex.FactoryMap[x].TextureDictionaryRef = &TextureDictionary;
		OrganicFactoryIndex.FactoryMap[x].VertexColorDictionaryRef = &VertexColorDictionary;
	}
	auto factoryend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> factorytime = factoryend - factorystart;
	cout << "Organic Factory build time: " << factorytime.count() << endl;
}


void OrganicSystem::AddKeyToRenderList(EnclaveKeyDef::EnclaveKey tempKey)
{
	renderCollectionList.KeyVector.push_back(tempKey);
	//SetupFutureCollectionMM(tempKey);
}

void OrganicSystem::SendRenderListToGLTerrainBuffer()
{
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator renderListIter = renderCollectionList.KeyVector.begin();
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator renderListIterEnd = renderCollectionList.KeyVector.end();
	RenderCollection* newRenderCollPtr;

	// send first vertex attribute: positions
	OGLM.RMContainer.CurrentIndex = 0;
	//auto factorystart = std::chrono::high_resolution_clock::now();
	for (renderListIter; renderListIter != renderListIterEnd; ++renderListIter)
	{
		EnclaveKeyDef::EnclaveKey tempKey = *renderListIter;						// get the key at this point in the vector
		
		newRenderCollPtr = GetRenderCollectionPtr(tempKey.x, tempKey.y, tempKey.z);	// get the pointer to the RenderCollection that has this key
		
		SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);							// send this RenderCollection's data to the buffer
	}
	//auto factoryend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> factorytime = factoryend - factorystart;
	//cout << "Test of first render collection send: " << factorytime.count() << endl;

	// send second vertex attribute: colors
	OGLM.RMContainer.CurrentIndex = 0;
	renderListIter = renderCollectionList.KeyVector.begin();
	for (renderListIter; renderListIter != renderListIterEnd; ++renderListIter)
	{
		//cout << " test " << endl;
		EnclaveKeyDef::EnclaveKey tempKey = *renderListIter;						// get the key at this point in the vector
		newRenderCollPtr = GetRenderCollectionPtr(tempKey.x, tempKey.y, tempKey.z);	// get the pointer to the RenderCollection that has this key
		LoadVCDataToGLBuffer(newRenderCollPtr);							// send this RenderCollection's data to the buffer
	}
}

void OrganicSystem::MaterializeRenderablesByMM()
{
	EnclaveCollections.SetOrganicSystem(this);
	//thread_pool *tpref = getCell1();
	//thread_pool *tpref2 = getCell2();
	int totalCollectionsToRender = 0;																				// will count total number of collections to be rendered from renderCollectionList
	int numberOfThreadsToRun = OCList.numberOfCells;																					// indicates the number of threads that will be running these jobs
	int collectionsPerThread = 0;																					// indicates how many collections each thread will process.
	int oddflag = 0;																								// indicates if the number of collections to render is even or odd
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter = renderCollectionList.KeyVector.begin();	// set iterator to be the beginning of the list.
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter2 = renderCollectionList.KeyVector.begin();

	// determine the number of collections to render.
	for (collectionListIter; collectionListIter != renderCollectionList.KeyVector.end(); ++collectionListIter)
	{
		totalCollectionsToRender++;
	}


	// split up the collections into MDListJobMaterializeCollection objects; number of MDListJobMaterializeCollection objects will be equal to numberOfThreadsToRun
	collectionsPerThread = totalCollectionsToRender / numberOfThreadsToRun;				// determine the number of collections per thread
	if (totalCollectionsToRender % numberOfThreadsToRun >= 1)
	{
		cout << "odd number of collections!!!" << endl;
		oddflag = 1;
	}

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
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
			++collectionListIter2;
			tempMatCollListRef->ListMatrix[tempKey] = tempMDJob;

		}
	}

	// if the number of collections was odd, add the remainining collection to the first list
	if (oddflag == 1)
	{
		int collectionsLeftToProcess = totalCollectionsToRender % numberOfThreadsToRun;
		std::vector<MDListJobMaterializeCollection>::iterator listJobIterator = MatCollList.MaterializeCollectionList.begin();
		for (int x = 0; x < collectionsLeftToProcess; x++)
		{
			MDListJobMaterializeCollection* tempMatCollListRef = &*listJobIterator;			// &MatCollList.MaterializeCollectionList.front();
			EnclaveKeyDef::EnclaveKey tempKey = *collectionListIter2;
			passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[tempKey];
			passManifestPtrNew = &ManifestCollections.ManiCollectionMap[tempKey];
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
			tempMatCollListRef->ListMatrix[tempKey] = tempMDJob;
			listJobIterator++;
			collectionListIter2++;

		}
	}

	//std::mutex mutexval;
	// NEW CODE: cycle through factories (10/20/2017)
	std::unordered_map<int, EnclaveManifestFactoryT1>::iterator factoryIterator;		// temporary iterator
	std::vector<EnclaveManifestFactoryT1*> manifestFactoryPtrVector;							// vector of pointers for EnclaveManifestFactoryT1
	std::vector<EnclaveManifestFactoryT1*>::iterator manifestFactoryPtrVectorIterator;
	std::vector<MDListJobMaterializeCollection*> mdListPtrVector;								// vector of pointers for MDListJobMaterializeCollection
	std::vector<MDListJobMaterializeCollection*>::iterator mdListPtrVectorIterator;
	std::vector<MDListJobMaterializeCollection>::iterator mdListVector;							// iterator for going through the MatCollList
	std::vector<std::future<void>> futureList;													// vector of futures (futures will be moved into this vector)
	std::vector<std::future<void>>::iterator futureListIterator;								// iterator for the list of futures
	factoryIterator = OrganicFactoryIndex.FactoryMap.begin();
	mdListVector = MatCollList.MaterializeCollectionList.begin();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		factoryIterator->second.StorageArray[0].VertexArrayCount = 0;			// reset vertex array count? (may not be needed later)

		EnclaveManifestFactoryT1* tempFactoryPtr = &factoryIterator->second;	// create a temporary pointer
		manifestFactoryPtrVector.push_back(tempFactoryPtr);						// push the temporary pointer into the vector
		factoryIterator++;														// increment the factoryIterator

		MDListJobMaterializeCollection* tempListPtr = &*mdListVector;			// get the reference to the list (iterating through list is done in the format *<vector name>)
		mdListPtrVector.push_back(tempListPtr);									// push the temporary pointer into the vector
		mdListVector++;															// increment the list iterator

		//std::future<void> futureToMove = OCList.cellList[x].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(tempListPtr), std::ref(heapmutex), std::ref(tempFactoryPtr), 1);		// submit the job
		//futureList.push_back(std::move(futureToMove));			// push_back the future via move
		//cout << "thread push complete, " << x << endl;
	}

	// submit jobs to threads
	manifestFactoryPtrVectorIterator = manifestFactoryPtrVector.begin();	// set factory iterator
	mdListPtrVectorIterator = mdListPtrVector.begin();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		std::future<void> futureToMove = OCList.cellList[x].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromMM, this, std::ref(*mdListPtrVectorIterator), std::ref(heapmutex), 1);		// submit the job
		heapmutex.lock();
		futureList.push_back(std::move(futureToMove));			// push_back the future via move
		heapmutex.unlock();
		manifestFactoryPtrVectorIterator++;
		mdListPtrVectorIterator++;
	}


	// wait for futures to complete
	futureListIterator = futureList.begin();
	auto lowstart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		std::future<void>* futurePtr = &*futureListIterator;	// get a pointer to the future
		futurePtr->wait();										// wait for the future to be done
		futureListIterator++;									// increment the futureList iterator
																//cout << "future wait complete..." << endl;
	}
	auto lowend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (MM Method):  " << lowelapsed.count() << endl;

	/*
	MDListJobMaterializeCollection* list1 = &MatCollList.MaterializeCollectionList.front();
	MDListJobMaterializeCollection* list2 = &MatCollList.MaterializeCollectionList.back();

	//OrganicSystem::JobMaterializeMultiCollectionFromMM(MDListJobMaterializeCollection mdjob, mutex& mutexval, int ThreadID)
	cout << "before job submit" << endl;
	std::future<void> coll_3 = OCList.cellList[0].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromMM, this, std::ref(list1), std::ref(heapmutex), 1);
	std::future<void> coll_4 = OCList.cellList[1].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromMM, this, std::ref(list2), std::ref(heapmutex), 2);

	auto lowstart = std::chrono::high_resolution_clock::now();

	coll_3.wait();
	coll_4.wait();
	auto lowend = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (High Memory Efficiency):  " << lowelapsed.count() << endl;
	*/
}

void OrganicSystem::MaterializeRenderablesByFactory()
{
	EnclaveCollections.SetOrganicSystem(this);
	//thread_pool *tpref = OCList.cellList[0].threadPtr;
	//thread_pool *tpref2 = OCList.cellList[1].threadPtr;
	int totalCollectionsToRender = 0;																				// will count total number of collections to be rendered from renderCollectionList
	int numberOfThreadsToRun = OCList.numberOfCells;																					// indicates the number of threads that will be running these jobs
	int collectionsPerThread = 0;																					// indicates how many collections each thread will process.
	int oddflag = 0;																								// indicates if the number of collections to render is even or odd
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter = renderCollectionList.KeyVector.begin();	// set iterator to be the beginning of the list.
	std::vector<EnclaveKeyDef::EnclaveKey>::iterator collectionListIter2 = renderCollectionList.KeyVector.begin();

	// determine the number of collections to render.
	for (collectionListIter; collectionListIter != renderCollectionList.KeyVector.end(); ++collectionListIter)
	{
		totalCollectionsToRender++;
	}
	cout << "total collections to render: " << totalCollectionsToRender << endl;

	// split up the collections into MDListJobMaterializeCollection objects; number of MDListJobMaterializeCollection objects will be equal to numberOfThreadsToRun
	collectionsPerThread = totalCollectionsToRender / numberOfThreadsToRun;				// determine the number of collections per thread
	if (totalCollectionsToRender % numberOfThreadsToRun >= 1)
	{
		cout << "odd number of collections!!!" << endl;
		oddflag = 1;
	}

	EnclaveCollectionBlueprintMatrix *passBlueprintMatrixPtr = &BlueprintMatrix;		// set up required pointers that are used as parameters for each MDJob
	EnclaveCollectionMatrix *passEnclaveCollectionPtr = &EnclaveCollections;
	ManifestCollectionMatrix *passManifestCollPtr = &ManifestCollections;
	RenderCollectionMatrix *passRenderCollMatrixPtr = &RenderCollections;
	EnclaveCollection *passCollectionPtrNew;
	ManifestCollection *passManifestPtrNew;

	// add collections to list(s)
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
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
			++collectionListIter2;
			tempMatCollListRef->ListMatrix[tempKey] = tempMDJob;
		}
	}

	// if the number of collections was odd, add the remainining collection to the first list
	if (oddflag == 1)
	{
		int collectionsLeftToProcess = totalCollectionsToRender % numberOfThreadsToRun;
		std::vector<MDListJobMaterializeCollection>::iterator listJobIterator = MatCollList.MaterializeCollectionList.begin();
		for (int x = 0; x < collectionsLeftToProcess; x++)
		{
			MDListJobMaterializeCollection* tempMatCollListRef = &*listJobIterator;			// &MatCollList.MaterializeCollectionList.front();
			EnclaveKeyDef::EnclaveKey tempKey = *collectionListIter2;
			passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[tempKey];
			passManifestPtrNew = &ManifestCollections.ManiCollectionMap[tempKey];
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);
			tempMatCollListRef->ListMatrix[tempKey] = tempMDJob;
			listJobIterator++;
			collectionListIter2++;
		}
	}




	//std::mutex mutexval;


	
	// NEW CODE: cycle through factories (10/20/2017)
	// set up vectors
	std::unordered_map<int, EnclaveManifestFactoryT1>::iterator factoryIterator;		// temporary iterator
	std::vector<EnclaveManifestFactoryT1*> manifestFactoryPtrVector;							// vector of pointers for EnclaveManifestFactoryT1
	std::vector<EnclaveManifestFactoryT1*>::iterator manifestFactoryPtrVectorIterator;
	std::vector<MDListJobMaterializeCollection*> mdListPtrVector;								// vector of pointers for MDListJobMaterializeCollection
	std::vector<MDListJobMaterializeCollection*>::iterator mdListPtrVectorIterator;
	std::vector<MDListJobMaterializeCollection>::iterator mdListVector;							// iterator for going through the MatCollList
	std::vector<std::future<void>> futureList;													// vector of futures (futures will be moved into this vector)
	std::vector<std::future<void>>::iterator futureListIterator;								// iterator for the list of futures
	factoryIterator = OrganicFactoryIndex.FactoryMap.begin();
	mdListVector = MatCollList.MaterializeCollectionList.begin();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		factoryIterator->second.StorageArray[0].VertexArrayCount = 0;			// reset vertex array count? (may not be needed later)

		EnclaveManifestFactoryT1* tempFactoryPtr = &factoryIterator->second;	// create a temporary pointer
		manifestFactoryPtrVector.push_back(tempFactoryPtr);						// push the temporary pointer into the vector
		factoryIterator++;														// increment the factoryIterator

		MDListJobMaterializeCollection* tempListPtr = &*mdListVector;			// get the reference to the list (iterating through list is done in the format *<vector name>)
		mdListPtrVector.push_back(tempListPtr);									// push the temporary pointer into the vector
		mdListVector++;															// increment the list iterator
		//std::future<void> futureToMove = OCList.cellList[x].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(tempListPtr), std::ref(heapmutex), std::ref(tempFactoryPtr), 1);		// submit the job
		//heapmutex.lock();
		//futureList.push_back(std::move(futureToMove));			// push_back the future via move
		//heapmutex.unlock();
		//cout << "thread push complete, " << x << endl;
	}

	// submit jobs to threads
	manifestFactoryPtrVectorIterator = manifestFactoryPtrVector.begin();	// set factory iterator
	mdListPtrVectorIterator = mdListPtrVector.begin();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		std::future<void> futureToMove = OCList.cellList[x].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(*mdListPtrVectorIterator), std::ref(heapmutex), std::ref(*manifestFactoryPtrVectorIterator), 1);		// submit the job
		heapmutex.lock();
		futureList.push_back(std::move(futureToMove));			// push_back the future via move
		heapmutex.unlock();
		manifestFactoryPtrVectorIterator++;
		mdListPtrVectorIterator++;
	}

	// wait for futures to complete
	futureListIterator = futureList.begin();
	auto lowstart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < numberOfThreadsToRun; x++)
	{
		std::future<void>* futurePtr = &*futureListIterator;	// get a pointer to the future
		futurePtr->wait();										// wait for the future to be done
		futureListIterator++;									// increment the futureList iterator
	}
	auto lowend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (Factory Method):  " << lowelapsed.count() << endl;
	
	/*
	// OLD CODE (10/20/2017)
	OrganicFactoryIndex.FactoryMap["Factory 0"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap["Factory 0"];
	//FactoryPtr->TextureDictionaryRef = &TextureDictionary;
	//FactoryPtr->VertexColorDictionaryRef = &VertexColorDictionary;

	OrganicFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr2 = &OrganicFactoryIndex.FactoryMap["Factory 1"];
	//FactoryPtr2->TextureDictionaryRef = &TextureDictionary;
	//FactoryPtr2->VertexColorDictionaryRef = &VertexColorDictionary;

	MDListJobMaterializeCollection* list1 = &MatCollList.MaterializeCollectionList.front();
	MDListJobMaterializeCollection* list2 = &MatCollList.MaterializeCollectionList.back();

	std::future<void> coll_3 = OCList.cellList[0].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(list1), std::ref(heapmutex), std::ref(FactoryPtr), 1);
	std::future<void> coll_4 = OCList.cellList[1].threadPtr->submit5(&OrganicSystem::JobMaterializeMultiCollectionFromFactory2, this, std::ref(list2), std::ref(heapmutex), std::ref(FactoryPtr2), 2);

	auto lowstart = std::chrono::high_resolution_clock::now();

	coll_3.wait();
	coll_4.wait();
	auto lowend = std::chrono::high_resolution_clock::now();
	

	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (Low Memory Efficiency):  " << lowelapsed.count() << endl;
	*/

	//cout << ">>>>>> collection jobs finished..." << endl;
}



void OrganicSystem::CheckForMorphing()
{
	if (OGLM.OrganicBufferManager.shiftFlag == 1)
	{
		// do stuff
		//cout << ">>>>MORPH required. " << endl;
		OGLM.OrganicBufferManager.MorphTerrainBuffers();
		OGLM.OrganicBufferManager.shiftFlag = 0;
	}
}

void OrganicSystem::CheckForT1CollectionPrep()
{

}

void OrganicSystem::WaitForPhase2Promises()
{
	std::vector<std::future<int>>::iterator T1_futureListIterator;								// iterator for the list of futures
	std::vector<std::future<void>>::iterator T2_futureListIterator;
	int hasT2 = 0;


	// check for T2 promises 
	//cout << "checking t2 promises..." << endl;
	if (FL_T2CollectionsProcessed.size() > 0)	// check if there are any futures to wait for
	{
		hasT2 = 1;
		//cout << "T2 size greater than 0, proceeding..." << "(size is " << FL_T2CollectionsProcessed.size() << ") " << endl;
		T2_futureListIterator = FL_T2CollectionsProcessed.begin();
		int collectionsToProcess = int(FL_T2CollectionsProcessed.size());
		for (int x = 0; x < collectionsToProcess; x++)
		{
			//cout << "waiting for future..." << endl;
			//std::future<void>* futurePtr = &*futureListIterator;	// get a pointer to the future
			T2_futureListIterator->wait();
			T2_futureListIterator++;
		}
		//cout << "T2 waits complete..." << endl;
	}

	// check for T1 promises
	// cout << "checking t1 promises..." << endl;
	if (FL_T1CollectionsProcessed.size() > 0)
	{	
		//cout << "T1 size greater than 0, proceeding..." << "(size is " << FL_T1CollectionsProcessed.size() << ") " << endl;
		T1_futureListIterator = FL_T1CollectionsProcessed.begin();
		int collectionsToProcess = int(FL_T1CollectionsProcessed.size());
		for (int x = 0; x < collectionsToProcess; x++)
		{
			T1_futureListIterator->wait();
			
			int resultOutput = T1_futureListIterator->get();
			//cout << "result output is: " << resultOutput << endl;
			if (resultOutput != -1)
			{
				//cout << "hoo doggy its NOT NULL!" << endl;
				EnclaveKeyDef::EnclaveKey currentElementKey = CollectionStateArray.StateMtxPtr[resultOutput].ActualCollectionKey;
				/*
				std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollection, EnclaveKeyDef::KeyHasher>::iterator collectionFindIter;
				collectionFindIter = EnclaveCollections.EnclaveCollectionMap.find(currentElementKey);
				if (collectionFindIter != EnclaveCollections.EnclaveCollectionMap.end())
				{
					cout << "Enclave Collection already found!" << endl;
				}
				*/
				EnclaveCollection* enclaveCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[currentElementKey];
				CollectionStateArray.UpdateCollectionStatus(resultOutput, 1, enclaveCollectionPtr);
			}
			
			//cout << "wait complete..." << endl;
			T1_futureListIterator++;
		}
		//cout << "T1 waits complete..." << endl;
	}


	// send data to OGLM, after waiting for all futures to complete
	//cout << "before queue size acquired..." << endl;
	int organicMorphMetaQueueSize = int(OrganicMorphMetaToSendToBuffer.size());
	//cout << "queue size acquired..." << endl;
	if (organicMorphMetaQueueSize > 0)
	{
		//cout << "MorphMetaQueueSize contains elements..." << endl;
		for (int x = 0; x < organicMorphMetaQueueSize; x++)
		{
			//cout << "attempting send to buffer..." << endl;
			OrganicMorphMeta metaToSend = OrganicMorphMetaToSendToBuffer.front();
			RenderCollection* renderCollectionPtr = &RenderCollections.RenderMatrix[metaToSend.collectionKey];
			//cout << "Sending key:" << metaToSend.collectionKey.x << ", " << metaToSend.collectionKey.y << ", " << metaToSend.collectionKey.z << endl;
			OGLM.sendRenderCollectionDataToBuffer(metaToSend, renderCollectionPtr);				// send the vertex data to buffer
			// NEW PROTOTYPE TESTING -- VC data send needs to be disabled.
			// OGLM.sendRenderCollectionVCDataToBuffer(metaToSend, renderCollectionPtr);			// send the vertex color data to buffer						
			PopOrganicMorphMetaQueue();															// RAII, lock_guard pop
		}
	}

	// cout << "wait check 3 " << endl;
	// empty collection vectors
	// empty T2
	if (FL_T2CollectionsProcessed.size() > 0)	// check if there are any futures to wait for
	{
		//cout << "MorphMetaQueueSize contains elements..." << endl;
		//cout << "clearing t2 promises..." << endl;
		FL_T2CollectionsProcessed.clear();
		OrganicMDJobVectorT2.clear();
		phase2end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> phaseelapsed = phase2end - phase2begin;
		//cout << "Phase 2 completion time: " << phaseelapsed.count() << endl;
	}



	// empty T1
	if (FL_T1CollectionsProcessed.size() > 0)
	{
		cout << "clearing T1 queue..." << endl;
		FL_T1CollectionsProcessed.clear();
		OrganicMDJobVectorT1.clear();
	}

	
	if (hasT2 == 1)
	{
		for (int x = 0; x < OGLM.renderableCollectionList.numberOfRenderableCollections; x++)
		{
			EnclaveKeyDef::EnclaveKey tempKey = OGLM.renderableCollectionList.TT1_CollectionKeys[x];
			// cout << "new render key list: (" << x <<  ") " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << endl;
		}
	}

}

void OrganicSystem::PopOrganicMorphMetaQueue()
{
	std::lock_guard<std::mutex> lock(heapmutex);
	OrganicMorphMetaToSendToBuffer.pop();
}

void OrganicSystem::CheckProcessingQueue()
{
	phase2begin = std::chrono::high_resolution_clock::now();


	auto factorystart = std::chrono::high_resolution_clock::now();
	//EnclaveManifestFactoryT1 *FactoryPtr = &OrganicFactoryIndex.FactoryMap["Factory 1"];		// factory pointer

	// set up vectors
	std::unordered_map<int, EnclaveManifestFactoryT1>::iterator factoryIterator;		// temporary iterator
	std::vector<EnclaveManifestFactoryT1*> manifestFactoryPtrVector;							// vector of pointers for EnclaveManifestFactoryT1
	std::vector<EnclaveManifestFactoryT1*>::iterator manifestFactoryPtrVectorIterator;
	std::vector<std::future<void>> futureList;													// vector of futures (futures will be moved into this vector)
	std::vector<std::future<void>>::iterator futureListIterator;								// iterator for the list of futures
	factoryIterator = OrganicFactoryIndex.FactoryMap.begin();
	// needs to be configured to use OCList
	//thread_pool *tpref = getCell1();															// thread pool pointers
	//thread_pool *tpref2 = getCell2();

	//set up pointers
	EnclaveCollectionBlueprintMatrix *passBlueprintMatrixPtr = &BlueprintMatrix;		// set up required pointers that are used as parameters for each MDJob
	EnclaveCollectionMatrix *passEnclaveCollectionPtr = &EnclaveCollections;
	ManifestCollectionMatrix *passManifestCollPtr = &ManifestCollections;
	RenderCollectionMatrix *passRenderCollMatrixPtr = &RenderCollections;
	EnclaveCollection *passCollectionPtrNew;
	ManifestCollection *passManifestPtrNew;

	// >>>>>>>>>>>>>>>>>>> PHASE 1: prepare variables
	int numberOfThreads1 = int(OCManager.t1CellMap.size());											// get the size of the T1 terrain cell list
	int numberOfThreads2 = int(OCManager.t2CellMap.size());											// get the size of the T2 terrain cell list
	std::map<int, OrganicCell*>::iterator T1terrainCellMapIter = OCManager.t1CellMap.begin();
	std::map<int, OrganicCell*>::iterator T2terrainCellMapIter = OCManager.t2CellMap.begin();	// create and assign an iterator for the T2 cells

	std::vector<MDJobMaterializeCollection>::iterator T1_MDJobVectorIterator;
	std::vector<MDJobMaterializeCollection>::iterator T2_MDJobVectorIterator;
	std::vector<OrganicMorphMeta> T1_OMMVector;													// vector for holding T1 OrganicMorphMeta data
	std::vector<OrganicMorphMeta> T2_OMMVector;													// vector for holding T2 OrganicMorphMeta data
	std::vector<OrganicMorphMeta>::iterator T1_OMMVectorIterator;
	std::vector<OrganicMorphMeta>::iterator T2_OMMVectorIterator;
	int T1_jobCount = 0;					
	int T2_jobCount = 0;

	// >>>>>>>>>>>>>>>>>>> PHASE 2: set up work vectors

	// Type 1 set up
	//cout << "numberOfThreads1: " << numberOfThreads1 << ", numberOfThreads2: " << numberOfThreads2 << endl;
	for (int x = 0; x < numberOfThreads1; x++)
	{
		if (!T1CollectionProcessingQueue.empty())
		{
			//cout << "Preparing to pop! " << endl;
			OrganicMorphMeta popKey = T1CollectionProcessingQueue.front();  // get the front
			T1CollectionProcessingQueue.pop();							    // pop the queue
			if (popKey.needsMMSetup == 1)
			{
				//cout << "||||||||||||||||||||||||||||||||||||||||| >>>>>>>>>>>>>> NEEDS MM SETUP!" << endl;
				SetupFutureCollectionMM(popKey.collectionKey);					
			}
			T1_OMMVector.push_back(popKey);								    // push it to OMMVector
			EnclaveKeyDef::EnclaveKey tempKey = popKey.collectionKey;		// get the collection key of the popKey
			passCollectionPtrNew = &EnclaveCollections.EnclaveCollectionMap[tempKey];
			passManifestPtrNew = &ManifestCollections.ManiCollectionMap[tempKey];	// get the manifest collection pointer to pass (instantiates a new instance of a ManifestCollection if it doesn't exist already)
			EnclaveCollectionStateArray* stateArrayPtr = &CollectionStateArray;
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode, std::ref(stateArrayPtr));	//... use it to make a temp MDJob
			OrganicMDJobVectorT1.push_back(tempMDJob);
			T1_jobCount++;
			//cout << "T1 job added!" << endl;
		}

	}



	// Type 2 set up
	// remove old collections
	if (!T2CollectionRemovalQueue.empty())
	{
		int removalSize = T2CollectionRemovalQueue.size();
		for (int x = 0; x < removalSize; x++)
		{
			OrganicMorphMeta removalKey = T2CollectionRemovalQueue.front();
			T2CollectionRemovalQueue.pop();
			OGLM.OrganicBufferManager.DCMPtr->removeHighLODAndSort(removalKey.collectionKey);
		}
		//cout << "Remove high LOD complete." << endl;
	}

	for (int x = 0; x < numberOfThreads2; x++)
	{
		if (!T2CollectionProcessingQueue.empty())	// only do the following if the queue isn't empty
		{
			OrganicMorphMeta popKey = T2CollectionProcessingQueue.front();	// get the front
			T2CollectionProcessingQueue.pop();								// pop the queue
			T2_OMMVector.push_back(popKey);									// push it to OMMVector
			//if (popKey.containsPreviousKey == 1)
			//{
				//cout << "collection needs to be removed from draw call. (" << popKey.oldCollectionKey.x << ", " << popKey.oldCollectionKey.y << ", " << popKey.oldCollectionKey.z << ") " << endl;
				//OGLM.OrganicBufferManager.DCMPtr->removeHighLODAndSort(popKey.oldCollectionKey);
			//}
			EnclaveKeyDef::EnclaveKey tempKey = popKey.collectionKey;		// get the collection key of the popKey
			//cout << "  OGLM render mode is: " << OGLM.renderMode << endl;
			MDJobMaterializeCollection tempMDJob(tempKey, std::ref(passBlueprintMatrixPtr), std::ref(passEnclaveCollectionPtr), std::ref(passManifestCollPtr), std::ref(passRenderCollMatrixPtr), std::ref(passCollectionPtrNew), std::ref(passManifestPtrNew), OGLM.renderMode);	//... use it to make a temp MDJob
			OrganicMDJobVectorT2.push_back(tempMDJob);
			T2_jobCount++;							// increment workable task counter
			//cout << "T2 job added!" << endl;
		}
	}

	//  >>>>>>>>>>>>>>>>>>> PHASE 3: submit jobs to worker threads
	
	if (T1_jobCount > 0)
	{
		T1_OMMVectorIterator = T1_OMMVector.begin();
		// cout << "T1 OMM vector size: " << T1_OMMVector.size() << endl;
		T1_MDJobVectorIterator = OrganicMDJobVectorT1.begin();
		for (int x = 0; x < T1_jobCount; x++)
		{
			OrganicMorphMeta popKey = *T1_OMMVectorIterator;						// get a copy of the element this iterator points to
			MDJobMaterializeCollection* tempMDJobRef = &*T1_MDJobVectorIterator;	// get a pointer to the current element this iterator points to
			SubmitT1TerrainJob(popKey, T1terrainCellMapIter, tempMDJobRef);
			T1_OMMVectorIterator++;
			T1_MDJobVectorIterator++;
			T1terrainCellMapIter++;
		}
		//cout << "T1 thread size" << int(OCManager.t1CellMap.size()) << endl;
		//cout << ">>>> T1 CheckProcessingQueue end (1)..." << endl;
	}
	
	
	// submit jobs for T2 terrain cell work
	if (T2_jobCount > 0)
	{
		//cout << ">>>> CheckProcessingQueue begin (1)..." << endl;
		T2_OMMVectorIterator = T2_OMMVector.begin();
		T2_MDJobVectorIterator = OrganicMDJobVectorT2.begin();
		for (int x = 0; x < T2_jobCount; x++)
		{
			OrganicMorphMeta popKey = *T2_OMMVectorIterator;						// get a copy of the element this iterator points to
			MDJobMaterializeCollection* tempMDJobRef = &*T2_MDJobVectorIterator;	// get a pointer to the current element this iterator points to
			SubmitT2TerrainJob(popKey, T2terrainCellMapIter, tempMDJobRef);
			T2_OMMVectorIterator++;
			T2_MDJobVectorIterator++;
			T2terrainCellMapIter++;
			//cout << "work added for collection: " << x << endl;
		}
		//cout << ">>>> T2 CheckProcessingQueue end (1)..." << endl;
	}


}

void OrganicSystem::SubmitT1TerrainJob(OrganicMorphMeta in_popKey, std::map<int, OrganicCell*>::iterator in_iterator, MDJobMaterializeCollection* in_tempMDJobRef)
{
	std::lock_guard<std::mutex> lock(heapmutex);
	OrganicMorphMetaToSendToBuffer.push(in_popKey);	// insert a value into the buffer work queue
	std::future<int> pop_1 = in_iterator->second->threadPtr->submit5(&OrganicSystem::JobMaterializeCollectionFromMM, this, in_tempMDJobRef, std::ref(heapmutex));
	FL_T1CollectionsProcessed.push_back(std::move(pop_1));
}

void OrganicSystem::SubmitT2TerrainJob(OrganicMorphMeta in_popKey, std::map<int, OrganicCell*>::iterator in_iterator, MDJobMaterializeCollection* in_tempMDJobRef)
{
	std::lock_guard<std::mutex> lock(heapmutex);
	OrganicMorphMetaToSendToBuffer.push(in_popKey);	// insert a value into the buffer work queue
	std::future<void> pop_1 = in_iterator->second->threadPtr->submit5(&OrganicSystem::JobMaterializeCollectionFromFactoryViaMorph, this, in_tempMDJobRef, std::ref(heapmutex), std::ref(in_iterator->second->factoryPtr));		// SOLUTION TO THIS (10/22/2017): changed input parameter, "tempMDJobRef" to not use std::ref() || std::ref(*manifestFactoryPtrVectorIterator)
	FL_T2CollectionsProcessed.push_back(std::move(pop_1));
}

void OrganicSystem::setPhaseinDCM()
{
	OGLM.OrganicBufferManager.setPhaseInDCM();
}

void OrganicSystem::DivideTickWork()
{
	/*
	// MODE 0: default (normal) = (3 cells) 1 T1 cell, 2 T2 cells
	if (workPriority == 0)
	{
		// first, check for T1 terrain work
		// ...is there terrain to be worked on?
		if (T1CollectionProcessingQueue.empty())
		{
			// second, check for T2 terrain work
			// ...is there terrain to be worked on?
			if (!T2CollectionProcessingQueue.empty())
			{
				cout << ">>>> Queue is not empty!!! dividing work..." << endl;
				// if there is, are there already threads assigned to do it?
				if (OCManager.t2CellMap.size() == 0)	//if there are no threads, assign them to do this work
				{
					cout << ">>>> No terrain cells assigned, attempting assignment..." << endl;
					// function call here
					OCManager.AssignT1TerrainCells();
				}

			}
			// ...is there no terrain to be worked on?
			else if (T2CollectionProcessingQueue.empty())
			{

				// if there were cells working in the last tick, but the terrain queue is empty, they must be freed
				if (OCManager.t2CellMap.size() > 0)
				{
					cout << ">>>> Queue is empty!!! reassigning cells used for terrain during last tick..." << endl;
					// function call here
					OCManager.ReleaseT1TerrainCells();
				}
			}
		}

	}
	*/

	// ****NEW MODE CHECKS****
	// set conditions for priority 0
	//cout << "divide tick work call..." << endl;
	if ((!T1CollectionProcessingQueue.empty() && !T2CollectionProcessingQueue.empty())
		||
		(!T1CollectionProcessingQueue.empty() && T2CollectionProcessingQueue.empty()))		// condition for mode 0: T1 and T2 cannot be empty.
	{
		// check if previously set work priority is equal to 0; if this is true, do 
		if (workPriority == 0)
		{
			//cout << "work priority is already 0!" << endl;
		}
		// do this if the current workPriority is equal to 1
		else if (workPriority == 1)
		{
			cout << "Switching priority from 1 to 0" << endl;
			OCLimitsList.changeToPriority(0, 1);	// change from priority 1 to priority 0
			workPriority = 0;
		}
	}

	/**/
	// set conditions for priority 1
	else if (T1CollectionProcessingQueue.empty() && !T2CollectionProcessingQueue.empty())	// condition for mode 1: T1 is empty, but T2 is not
	{
		if (workPriority == 1)
		{
			//cout << "work priority is already 1!" << endl;
		}
		// do this if the current workPriority is equal to 0
		else if (workPriority == 0)
		{
			cout << "Switching priority from 0 to 1" << endl;
			OCLimitsList.changeToPriority(1, 0);	// change from priority 0 to priority 1
			workPriority = 1;
		}
	}
}

void OrganicSystem::SetWorldCoordinates(float x, float y, float z)
{
	EnclaveKeyDef::EnclaveKey CollectionKey;
	PathTraceContainer XPathTrace = EnclaveCollections.GetCoordTrace(x);	// get x coords for collection/chunk/block
	PathTraceContainer YPathTrace = EnclaveCollections.GetCoordTrace(y);	// get y coords	for collection/chunk/block
	PathTraceContainer ZPathTrace = EnclaveCollections.GetCoordTrace(z);	// get z coords for collection/chunk/block
	CollectionKey.x = XPathTrace.CollectionCoord;					// set coords for the Enclave Collection
	CollectionKey.y = YPathTrace.CollectionCoord;
	CollectionKey.z = ZPathTrace.CollectionCoord;
	cout << "XYZ was: " << x << " " << y << " " << z << endl;
	cout << "OGLM SetWorldCoordinates result: " << CollectionKey.x << ", " << CollectionKey.y << ", " << CollectionKey.z << endl;
	OGLM.OrganicBufferManager.currentCenterCollectionKey = CollectionKey;			// send the key to the OGLM's buffer manager, so it has a center collection key on initial world load
	OGLM.renderableCollectionList.setInitialDynamicArrayData(CollectionKey);		// the renderableCollectionList's dynamic array must be set up with initial values, before any sorting is done
	OGLM.OrganicBufferManager.PopulateOGLMRMCArrays(OGLM.OrganicBufferManager.currentCenterCollectionKey);
}