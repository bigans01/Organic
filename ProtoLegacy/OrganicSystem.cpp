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

OrganicSystem::OrganicSystem()
{
	/* Summary: default constructor for the OrganicSystem */
	InterlockBaseCollections();
}

void OrganicSystem::InterlockBaseCollections()
{
	/* Summary: manually sets the required pointers for each of the base collections (EnclaveCollections, ManifestCollections, RenderCollections */
	ManifestCollections.SetCollectionMatrixRef(&EnclaveCollections);		// point the ManifestCollection to EnclaveCollection
	RenderCollections.SetManifestCollectionMatrixPtr(&ManifestCollections); // point the RenderCollection to the ManifestCollection
	RenderCollections.SetEnclaveCollectionMatrixPtr(&EnclaveCollections);
}

void OrganicSystem::AddAndMaterializeCollection(int x, int y, int z)
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
	EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrix.BlueprintMap[tempKey];
	// multithreaded testing begins here
	//EnclaveCollections.AddNewCollectionWithBlueprint(tempKey, blueprintptr);
	//EnclaveCollections.MultiAddNewCollectionWithBlueprint(4, tempKey, blueprintptr);
	//
	EnclaveCollections.SetOrganicSystem(this);
	auto start1 = std::chrono::high_resolution_clock::now();
	EnclaveCollections.MultiAddNewCollectionWithBlueprint(2, tempKey, blueprintptr);
	//EnclaveCollections.MultiAddNewCollectionWithBlueprint(1, tempKey, blueprintptr);
	auto finish1 = std::chrono::high_resolution_clock::now();
	EnclaveCollection *collectionPtr = &EnclaveCollections.EnclaveCollectionMap[tempKey];
															// optional, for debugging
	std::chrono::duration<double> elapsed1 = finish1 - start1;
	//cout << "Organic system phase 1: (Add collection, instantiate enclaves, determine solids, determine surface, perform painting, unveil polys): " << elapsed1.count() << endl;



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

void OrganicSystem::JobMaterializeCollection(	EnclaveKeyDef::EnclaveKey Key1, 
												EnclaveCollectionBlueprintMatrix BlueprintMatrixRef, 
												EnclaveCollectionMatrix EnclaveCollectionsRef,
												ManifestCollectionMatrix ManifestCollectionsRef, 
												RenderCollectionMatrix RenderCollectionsRef, 
												EnclaveCollection *CollectionRef,
												ManifestCollection *ManifestCollectionRef)			// change this name...to something better!
{
	

	//Phase 1: EnclaveCollection set up
	//EnclaveKeyDef::EnclaveKey BPKey;
	//BPKey.x = 0;
	//BPKey.y = 0;
	//BPKey.z = 0;
	auto truestart = std::chrono::high_resolution_clock::now();
	auto start4 = std::chrono::high_resolution_clock::now();
	EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef.BlueprintMap[Key1];				// hardcoded blueprint (for testing)


	EnclaveCollectionActivateListT2 listT2_1;
	EnclaveCollection *collectionMapRef = &EnclaveCollectionsRef.EnclaveCollectionMap[Key1];
	EnclaveCollectionsRef.JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1)); //EnclaveCollectionMap[Key]
	int chunkbitmask = 1;
	int bitmaskval = 0;
	//int dumbvalcheck = 0;
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
					CollectionRef->ActivateEnclaveForRendering(tempKey);
					//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					//dumbvalcheck++;
				}

			}
			//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
			chunkbitmask <<= 1;
			bitmaskval++;

		}
	}
	auto finish4 = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed4 = finish4 - start4;
	//cout << "Phase 1 time::  " << elapsed4.count() << " :" << dumbvalcheck << endl;

	//cout << "number of renderable polys" <<  CollectionRef->EnclaveArray[0][6][0].GetTotalTrianglesInEnclave() << endl;

	// Phase 2: ManifestCollection set up
	//cout << "test value of renderable enclaves: " << CollectionRef->totalRenderableEnclaves << endl;

	
	//auto start5 = std::chrono::high_resolution_clock::now();
	EnclaveCollection *collectionPtr = &EnclaveCollectionsRef.EnclaveCollectionMap[Key1];
	int manifestCounter = CollectionRef->totalRenderableEnclaves;
	auto start5 = std::chrono::high_resolution_clock::now();
	//auto finish5 = std::chrono::high_resolution_clock::now();
	//cout << "begin true test..." << manifestCounter << endl;
	EnclaveKeyDef::EnclaveKey innerTempKey;
	//ManifestCollection *tempMCollectionRef = &ManifestCollectionsRef.ManiCollectionMap[Key1];


	//cout << "Attempting manifest adds..." << endl;
	ManifestCollection *tempMCollectionRef = &ManifestCollectionsRef.GetRefManiCollectionMap(Key1);				// temp collection ref fails when using on line 169.
	for (int a = 0; a < manifestCounter; a++)
	{
		//start5 = std::chrono::high_resolution_clock::now();
		innerTempKey = CollectionRef->RenderableEnclaves[a];
		//cout << "test of key values: [" << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << "]" << endl;
		ManifestCollectionsRef.AttachManifestToCollectedEnclave2(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z, ManifestCollectionRef);
		//ManifestCollectionsRef.AttachManifestToCollectedEnclave(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z);
		//finish5 = std::chrono::high_resolution_clock::now();
		ManifestCollectionsRef.heaptest[0]++;
	}
	//cout << "Manifest adds complete..." << endl;
	auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
	std::chrono::duration<double> elapsed5 = finish5 - start5;
	//cout << "Phase 2 time:  " << elapsed5.count() << " :" << manifestCounter << endl;
	//ManifestCollectionRef->ManMatrix
	//ManifestCollection tempManiRef = std::ref(ManifestCollectionsRef.ManiCollectionMap[Key1]);
	std::unordered_map<EnclaveKeyDef::EnclaveKey, ManifestCollection, EnclaveKeyDef::KeyHasher>::iterator ManiCollectionIter;
	ManiCollectionIter = ManifestCollectionsRef.ManiCollectionMap.begin();
	//int tempVal = std::ref(ManifestCollectionsRef.heaptest);
	EnclaveKeyDef::EnclaveKey newTempKey;
	newTempKey.x = 0;
	newTempKey.y = 6;
	newTempKey.z = 0;
	//cout << "test of tempVal:" << ManifestCollectionsRef.heaptest[0];
	//cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[newTempKey].TotalEnclaveTriangles;				// RENAME THIS TO SOMETHING ELSE! (ManifestCollectionRef)
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<EnclaveManifest>, EnclaveKeyDef::KeyHasher>::iterator testiter;
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, reference_wrapper<ManifestCollection>, EnclaveKeyDef::KeyHasher>::iterator ManiCollectionIter2;

	//testiter = ManifestCollectionsRef.ManiCollectionMap[Key1].testmap2.find(newTempKey);
	//ManiCollectionIter2 = ManifestCollectionsRef.ManiCollectionMapRef.find(Key1);
	//reference_wrapper<EnclaveManifest> maniPtr = ManifestCollectionsRef.ManiCollectionMap[Key1].testmap2[newTempKey];
	//reference_wrapper<ManifestCollection> maniPtr = ManifestCollectionsRef.ManiCollectionMapRef[Key1];
	//EnclaveManifest maniPtr = testiter->second;

	//for (ManiCollectionIter; ManiCollectionIter != ManifestCollectionsRef.ManiCollectionMap.end(); ++ManiCollectionIter)
	//{
	//cout << "what" << ManiCollectionIter->second.GetManifestFromMatrix(0, 6, 0).TotalEnclaveTriangles  << endl;

	//}

	//ManifestCollection manicollPtr = ManiCollectionIter2->second;
	//testiter = manicollPtr.testmap2.find(newTempKey);
	//EnclaveManifest maniPtr = testiter->second;
	//cout << "test of maniPtr:" << maniPtr.TotalEnclaveTriangles << endl; // DOES NOT WORK!
	//cout << "test of otherval: " << maniPtr.TotalEnclaveTriangles << endl;
	
	
	//ManifestCollection tempManiRef = std::ref(ManifestCollectionsRef.GetRefManiCollectionMap(Key1));
	//cout << "test:::" << tempManiRef.GetManifestFromMatrix(0, 6, 0).TotalEnclaveTriangles << endl;
	// Phase 3: Render collection set up

	RenderCollectionsRef.CreateRenderArrayFromManifestCollection(Key1);
	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//cout << "Total time:  " << trueelapsed.count() << " :" << endl;
}

void OrganicSystem::MaterializeCollection(EnclaveKeyDef::EnclaveKey Key1, EnclaveKeyDef::EnclaveKey Key2)
{
	EnclaveCollections.SetOrganicSystem(this);
	thread_pool *tpref = getpool();
	thread_pool *tpref2 = getpool2();
	EnclaveKeyDef::EnclaveKey dumbtempkey;
	dumbtempkey.x = 0;
	dumbtempkey.y = 0;
	dumbtempkey.z = 0;
	EnclaveCollectionBlueprint *blueprintptrpass = &BlueprintMatrix.BlueprintMap[dumbtempkey];
	//auto start4 = std::chrono::high_resolution_clock::now();

	// temporary

	EnclaveKeyDef::EnclaveKey key3, key4, key5, key6, key7, key8;
	key3.x = 7;
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

	////////////////////////////////////////////////////////////////////////////// BEGIN HIGH MEMORY EFFICIENCY TEST


	// -------------------- For first list to be added to queue

	MDListJobMaterializeCollection tempJobList;

	EnclaveCollection *passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[Key1];
	ManifestCollection *passManifestPtr = &ManifestCollections.ManiCollectionMap[Key1];

	MDJobMaterializeCollection MDJob1(Key1, std::ref(BlueprintMatrix), std::ref(EnclaveCollections),  std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr));
	tempJobList.ListMatrix[Key1] = MDJob1;

	
	
	
	

	passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[Key2];
	passManifestPtr = &ManifestCollections.ManiCollectionMap[Key2];
	MDJobMaterializeCollection MDJob2(Key2, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr));
	tempJobList.ListMatrix[Key2] = MDJob2;

	passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[key5];
	passManifestPtr = &ManifestCollections.ManiCollectionMap[key5];
	MDJobMaterializeCollection MDJob5(key5, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr));
	tempJobList.ListMatrix[key5] = MDJob2;

	passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[key6];
	passManifestPtr = &ManifestCollections.ManiCollectionMap[key6];
	MDJobMaterializeCollection MDJob6(key6, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr));
	tempJobList.ListMatrix[key6] = MDJob2;

	/////////////////////////////////////////////////////////////////

	// -------------------- For third collection to be added to queue
	MDListJobMaterializeCollection tempJobList2;

	EnclaveCollection *passCollectionPtr2 = &EnclaveCollections.EnclaveCollectionMap[key3];
	ManifestCollection *passManifestPtr2 = &ManifestCollections.ManiCollectionMap[key3];

	MDJobMaterializeCollection MDJob3(key3, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2));
	tempJobList2.ListMatrix[key3] = MDJob1;

	// -------------------- For fourth collection to be added to queue
	passCollectionPtr2 = &EnclaveCollections.EnclaveCollectionMap[key4];
	passManifestPtr2 = &ManifestCollections.ManiCollectionMap[key4];
	MDJobMaterializeCollection MDJob4(key4, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2));
	tempJobList2.ListMatrix[key4] = MDJob2;

	passCollectionPtr2 = &EnclaveCollections.EnclaveCollectionMap[key7];
	passManifestPtr2 = &ManifestCollections.ManiCollectionMap[key7];
	MDJobMaterializeCollection MDJob7(key7, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2));
	tempJobList2.ListMatrix[key7] = MDJob2;

	passCollectionPtr2 = &EnclaveCollections.EnclaveCollectionMap[key8];
	passManifestPtr2 = &ManifestCollections.ManiCollectionMap[key8];
	MDJobMaterializeCollection MDJob8(key8, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2));
	tempJobList2.ListMatrix[key8] = MDJob2;

	std::mutex mutexval;



	//EnclaveCollections.EnclaveCollectionMap[Key1];
	//std::future<void> coll_1 = tpref->submit5(&OrganicSystem::JobMaterializeCollection, this, Key1, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr)); // , std::ref(blueprintptrpass)
	//std::future<void> coll_1 = tpref->submit5(&OrganicSystem::JobMaterializeCollection2, this, std::ref(MDJob1));
	std::future<void> coll_1 = tpref->submit5(&OrganicSystem::JobMaterializeCollection2, this, std::ref(tempJobList), std::ref(mutexval), 1);
	//auto start4 = std::chrono::high_resolution_clock::now();
	//std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeCollection, this, Key2, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2)); // , std::ref(blueprintptrpass)
	std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeCollection2, this, std::ref(tempJobList2), std::ref(mutexval), 2);
	//std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeCollection, this, Key2, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections));
	//auto finish4 = std::chrono::high_resolution_clock::now();
	auto start4 = std::chrono::high_resolution_clock::now();
	coll_1.wait();
	//auto finish4 = std::chrono::high_resolution_clock::now();
	//auto start4 = std::chrono::high_resolution_clock::now();
	coll_2.wait();
	auto finish4 = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed4 = finish4 - start4;
	cout << "Dual coollection instantiation speed (High Memory Efficiency):  " << elapsed4.count() << endl;

	////////////////////////////////////////////////////////////////////////////// END HIGH MEMORY EFFICIENCY TEST


	////////////////////////////////////////////////////////////////////////////// BEGIN LOW MEMORY EFFICIENCY TEST
	EnclaveManifestFactoryT1Index MainFactoryIndex;

	MainFactoryIndex.FactoryMap["Factory 1"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr = &MainFactoryIndex.FactoryMap["Factory 1"];
	FactoryPtr->TextureDictionaryRef = &TextureDictionary;

	MainFactoryIndex.FactoryMap["Factory 2"].StorageArray[0].VertexArrayCount = 0;
	EnclaveManifestFactoryT1 *FactoryPtr2 = &MainFactoryIndex.FactoryMap["Factory 2"];
	FactoryPtr2->TextureDictionaryRef = &TextureDictionary;
	
	
	std::future<void> coll_3 = tpref->submit5(&OrganicSystem::JobMaterializeCollection3, this, std::ref(tempJobList), std::ref(mutexval), std::ref(FactoryPtr), 1);
	std::future<void> coll_4 = tpref->submit5(&OrganicSystem::JobMaterializeCollection3, this, std::ref(tempJobList2), std::ref(mutexval), std::ref(FactoryPtr2), 2);
	auto lowstart = std::chrono::high_resolution_clock::now();
	coll_3.wait();
	coll_4.wait();
	auto lowend = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> lowelapsed = lowend - lowstart;
	cout << "Dual coollection instantiation speed (Low Memory Efficiency):  " << lowelapsed.count() << endl;
	////////////////////////////////////////////////////////////////////////////// END LOW MEMORY EFFICIENCY TEST

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

void OrganicSystem::SetupFutureCollection(int x, int y, int z)
{
	cout << "future collection call: " << endl;
	//MaterializeCollection(x, y, z);
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;
	tempKey.y = y;
	tempKey.z = z;
	EnclaveCollections.AddNewCollectionSkeleton(tempKey);			// set up some value
	ManifestCollections.AddNewCollection(tempKey);					// set up some value
	ManifestCollection& tempRef = ManifestCollections.ManiCollectionMap[tempKey];
	ManifestCollections.ManiCollectionMapRef.emplace(tempKey, tempRef);

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
	tempEnclave->UnveilSinglePoly(2, 2, 0, 0, 1, 0, 40, 0);

	ManifestCollections.UpdateAttachedManifest(CollectionKey, EnclaveKey.x, EnclaveKey.y, EnclaveKey.z);
	//tempEnclave->ChangePolyMaterial(0, 3, 0, 1);												// very fast operation; doing all material blocks to be changed in the current chunk is orders more efficient...
	//tempEnclave->ChangePolyMaterial(1, 3, 0, 1);
	//tempEnclave->ChangePolyMaterial(2, 3, 0, 1);
											
	//tempEnclave->ChangePolyMaterial(3, 3, 0, 1);
	//tempEnclave->ChangePolyMaterial(0, 3, 1, 1);
	//tempEnclave->ChangePolyMaterial(1, 3, 2, 1);
	//tempEnclave->ChangePolyMaterial(2, 3, 3, 1);
											
	//tempEnclave->ChangePolyMaterial(0, 3, 0, 1);
	//tempEnclave->ChangePolyMaterial(1, 3, 1, 1);
	//tempEnclave->ChangePolyMaterial(2, 3, 2, 1);
	//tempEnclave->ChangePolyMaterial(3, 3, 3, 1);
											
	//tempEnclave->ChangePolyMaterial(0, 3, 0, 1);
	//tempEnclave->ChangePolyMaterial(1, 3, 1, 1);
	//tempEnclave->ChangePolyMaterial(2, 3, 2, 1);

}

void OrganicSystem::AddBlueprint(int x, int y, int z, EnclaveCollectionBlueprint blueprint)
{
	EnclaveKeyDef::EnclaveKey tempKey;
	tempKey.x = x;										// set temp key to input of x
	tempKey.y = y;										// set temp key to input of y
	tempKey.z = z;										// set temp key to input of z
	BlueprintMatrix.BlueprintMap[tempKey] = blueprint;	// add blueprint to the BlueprintMatrix's unordered map -- BlueprintMap
}

thread_pool* OrganicSystem::getpool()
{
	return testpool2;
}

thread_pool* OrganicSystem::getpool2()
{
	return testpool3;
}

void OrganicSystem::SetOrganicPool(thread_pool *thread_pool_ref)
{
	testpool2 = thread_pool_ref;
}

void OrganicSystem::SetOrganicPool2(thread_pool *thread_pool_ref)
{
	testpool3 = thread_pool_ref;
}

void OrganicSystem::AddOrganicTextureMetaArray(string mapname)
{
	OrganicTextureMeta tempMeta(0);
	TextureDictionary.Dictionary[mapname].Index[1] = tempMeta;								// set up the texture meta for a block id of 1.
	OrganicTextureMeta *tempMetaRef = &TextureDictionary.Dictionary[mapname].Index[1];		// set up a reference to the new texture data for the block

	// set up some data 
	tempMetaRef->BlockData.FaceIndex[0].FaceData[0].U = 2;
}

void OrganicSystem::JobMaterializeCollection2(MDListJobMaterializeCollection mdjob, mutex& mutexval, int ThreadID)
{
	//std::mutex tempmutex;

	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;

	
	JobIterator = mdjob.ListMatrix.begin();
	JobIteratorEnd = mdjob.ListMatrix.end();
	

	//mutexval.lock();
	auto lockstart = std::chrono::high_resolution_clock::now();
	auto lockend = std::chrono::high_resolution_clock::now();
	auto truestart = std::chrono::high_resolution_clock::now();
	//mutexval.unlock();
	int dumboutput = 1;

	for (JobIterator = mdjob.ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)
	//for (auto it = mdjob.ListMatrix.begin(); it != mdjob.ListMatrix.end(); ++it)
	{
		auto initstart = std::chrono::high_resolution_clock::now();

		//MDJobMaterializeCollection tempList = JobIterator->second;
		//EnclaveKeyDef::EnclaveKey Key1 = tempList.MDKey;
		//EnclaveCollectionBlueprintMatrix BlueprintMatrixRef = tempList.MDBlueprintMatrixRef;
		//EnclaveCollectionMatrix EnclaveCollectionsRef = tempList.MDEnclaveCollectionsRef;
		//ManifestCollectionMatrix ManifestCollectionsRef = tempList.MDManifestCollectionsRef;
		//RenderCollectionMatrix RenderCollectionsRef = tempList.MDRenderCollectionsRef;
		//EnclaveCollection *CollectionRef = tempList.MDEnclaveCollectionPtr;
		//ManifestCollection *ManifestCollectionRef = tempList.MDManifestCollectionPtr;
		
		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;
		EnclaveCollectionBlueprintMatrix BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef.BlueprintMap[Key1];
		EnclaveCollectionMatrix EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;
		//ManifestCollectionMatrix ManifestCollectionsRef = JobIterator->second.MDManifestCollectionsRef;
		RenderCollectionMatrix RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;
		ManifestCollection *ManifestCollectionRef = JobIterator->second.MDManifestCollectionPtr;

		//auto initstart = std::chrono::high_resolution_clock::now();
		mutexval.lock();
		auto initend = std::chrono::high_resolution_clock::now();
																			// will cause heap corruption if removed (race condition to access map)?
					// hardcoded blueprint (for testing)
		//auto initend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> initelapsed = initend - initstart;
		//cout << "(THREAD_" << ThreadID <<") Init time (High Efficiency):  " << initelapsed.count() << " :" << endl;
		mutexval.unlock();																		// will cause heap corruption if removed (race condition to access map)?

		EnclaveCollectionActivateListT2 listT2_1;
		//EnclaveCollection *collectionMapRef = &EnclaveCollectionsRef.EnclaveCollectionMap[Key1];
		EnclaveCollectionsRef.JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1)); //EnclaveCollectionMap[Key]
		int chunkbitmask = 1;
		int bitmaskval = 0;
		//int dumbvalcheck = 0;

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
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = bitmaskval;
						tempKey.z = z;
						CollectionRef->ActivateEnclaveForRendering(tempKey);
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;
				bitmaskval++;

			}
		}



		
		// Phase 2: ManifestCollection set up
		mutexval.lock();
		int manifestCounter = CollectionRef->totalRenderableEnclaves;
		//cout << "(THREAD_" << ThreadID << ") Phase 2 Lock acquisition..." << endl;
		auto start5 = std::chrono::high_resolution_clock::now();
		EnclaveKeyDef::EnclaveKey innerTempKey;
		for (int a = 0; a < manifestCounter; a++)
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			//cout << "test of key values: [" << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << "]" << endl;
			//ManifestCollectionsRef.AttachManifestToCollectedEnclave2(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z, ManifestCollectionRef, dumboutput);
			ManifestCollectionRef->AddManifestToMatrix(innerTempKey.x, innerTempKey.y, innerTempKey.z, Key1);
		}
		auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
		std::chrono::duration<double> elapsed5 = finish5 - start5;									// ""
		//cout << "(THREAD_" << ThreadID << ") Phase 2 time (High Efficiency):  " << elapsed5.count() << " :" << manifestCounter << endl;			// ""
		mutexval.unlock();




		//cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[newTempKey].TotalEnclaveTriangles;				// RENAME THIS TO SOMETHING ELSE! (ManifestCollectionRef)

		// Phase 3: Render collection set up
		//mutexval.lock();
		//RenderCollectionsRef.CreateRenderArrayFromManifestCollection(Key1);
		//mutexval.unlock();
		//dumboutput++;
		
		
	}

	mutexval.lock();
	std::chrono::duration<double> truelocktime = lockstart - lockend;
	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	cout << "Total time:  " << trueelapsed.count() << endl;
	mutexval.unlock();
}

void OrganicSystem::JobMaterializeCollection3(MDListJobMaterializeCollection mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID)
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIterator;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher>::iterator JobIteratorEnd;

	auto truestart = std::chrono::high_resolution_clock::now();
	JobIterator = mdjob.ListMatrix.begin();
	JobIteratorEnd = mdjob.ListMatrix.end();

	//mutexval.lock();
	auto lockstart = std::chrono::high_resolution_clock::now();
	auto lockend = std::chrono::high_resolution_clock::now();
	//auto truestart = std::chrono::high_resolution_clock::now();
	//mutexval.unlock();

	for (JobIterator = mdjob.ListMatrix.begin(); JobIterator != JobIteratorEnd; ++JobIterator)
		//for (auto it = mdjob.ListMatrix.begin(); it != mdjob.ListMatrix.end(); ++it)
	{
		auto initstart = std::chrono::high_resolution_clock::now();

		//MDJobMaterializeCollection tempList = JobIterator->second;
		//EnclaveKeyDef::EnclaveKey Key1 = tempList.MDKey;
		//EnclaveCollectionBlueprintMatrix BlueprintMatrixRef = tempList.MDBlueprintMatrixRef;
		//EnclaveCollectionMatrix EnclaveCollectionsRef = tempList.MDEnclaveCollectionsRef;
		//ManifestCollectionMatrix ManifestCollectionsRef = tempList.MDManifestCollectionsRef;
		//RenderCollectionMatrix RenderCollectionsRef = tempList.MDRenderCollectionsRef;
		//EnclaveCollection *CollectionRef = tempList.MDEnclaveCollectionPtr;
		//ManifestCollection *ManifestCollectionRef = tempList.MDManifestCollectionPtr;

		EnclaveKeyDef::EnclaveKey Key1 = JobIterator->second.MDKey;
		EnclaveCollectionBlueprintMatrix BlueprintMatrixRef = JobIterator->second.MDBlueprintMatrixRef;
		EnclaveCollectionBlueprint *blueprintptr = &BlueprintMatrixRef.BlueprintMap[Key1];
		EnclaveCollectionMatrix EnclaveCollectionsRef = JobIterator->second.MDEnclaveCollectionsRef;
		//ManifestCollectionMatrix ManifestCollectionsRef = JobIterator->second.MDManifestCollectionsRef;
		RenderCollectionMatrix RenderCollectionsRef = JobIterator->second.MDRenderCollectionsRef;
		EnclaveCollection *CollectionRef = JobIterator->second.MDEnclaveCollectionPtr;
		//ManifestCollection *ManifestCollectionRef = JobIterator->second.MDManifestCollectionPtr;

		//auto initstart = std::chrono::high_resolution_clock::now();

		auto initend = std::chrono::high_resolution_clock::now();
		//mutexval.lock();																		
		std::chrono::duration<double> initelapsed = initend - initstart;
		//cout << "Init time (Low Efficiency):  " << initelapsed.count() << " :" << endl;
		//mutexval.unlock();																		

		EnclaveCollectionActivateListT2 listT2_1;
		//EnclaveCollection *collectionMapRef = &EnclaveCollectionsRef.EnclaveCollectionMap[Key1];
		EnclaveCollectionsRef.JobInstantiateAndPopulateEnclaveAlpha(0, 7 + 1, std::ref(*CollectionRef), Key1, blueprintptr, std::ref(listT2_1)); //EnclaveCollectionMap[Key]
		int chunkbitmask = 1;
		int bitmaskval = 0;
		//int dumbvalcheck = 0;

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
						EnclaveKeyDef::EnclaveKey tempKey;
						tempKey.x = x;
						tempKey.y = bitmaskval;
						tempKey.z = z;
						CollectionRef->ActivateEnclaveForRendering(tempKey);
						//cout << "value of key 1::: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << "||" << int(listT2_1.flagArray[x][z]) << endl;
					}

				}
				//cout << "chunkbitmask: (" << y << ")" << chunkbitmask << endl;
				chunkbitmask <<= 1;
				bitmaskval++;

			}
		}

		// Phase 2: Factory work
		int manifestCounter = CollectionRef->totalRenderableEnclaves;
		//auto start5 = std::chrono::high_resolution_clock::now();
		EnclaveKeyDef::EnclaveKey innerTempKey;
		FactoryRef->CurrentStorage = 0;					// reset storage location.
		for (int a = 0; a < manifestCounter; a++)
		{
			innerTempKey = CollectionRef->RenderableEnclaves[a];
			//cout << "test of key values: [" << innerTempKey.x << ", " << innerTempKey.y << ", " << innerTempKey.z << "]" << endl;
			//ManifestCollectionsRef.AttachManifestToCollectedEnclave2(Key1, innerTempKey.x, innerTempKey.y, innerTempKey.z, ManifestCollectionRef, dumboutput);
			Enclave *tempEnclavePtr = &CollectionRef->GetEnclaveByKey(innerTempKey);
			FactoryRef->AttachManifestToEnclave(tempEnclavePtr);

		}
		//auto finish5 = std::chrono::high_resolution_clock::now();									// optional, for debugging
		//std::chrono::duration<double> elapsed5 = finish5 - start5;									// ""

		//mutexval.lock();
		//cout << "Phase 2 time (Low efficiency):  " << elapsed5.count() << " :" << manifestCounter << endl;
		//mutexval.unlock();
	}

	mutexval.lock();
	std::chrono::duration<double> truelocktime = lockstart - lockend;
	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	cout << "Total time:  " << trueelapsed.count() << endl;
	mutexval.unlock();
}