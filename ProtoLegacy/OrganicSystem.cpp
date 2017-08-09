#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <functional>
#include "OrganicSystem.h"
#include "OrganicTextureMeta.h"
#include "thread_pool.h"

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


	//ManifestCollection *tempMCollectionRef = &ManifestCollectionsRef.GetRefManiCollectionMap(Key1);
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
	cout << "HOO BOY!" << ManifestCollectionRef->ManMatrix[newTempKey].TotalEnclaveTriangles;				// RENAME THIS TO SOMETHING ELSE!
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
	EnclaveCollection *passCollectionPtr = &EnclaveCollections.EnclaveCollectionMap[Key1];
	ManifestCollection *passManifestPtr = &ManifestCollections.ManiCollectionMap[Key1];

	EnclaveCollection *passCollectionPtr2 = &EnclaveCollections.EnclaveCollectionMap[Key2];
	ManifestCollection *passManifestPtr2 = &ManifestCollections.ManiCollectionMap[Key2];
	//EnclaveCollections.EnclaveCollectionMap[Key1];
	std::future<void> coll_1 = tpref->submit5(&OrganicSystem::JobMaterializeCollection, this, Key1, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr), std::ref(passManifestPtr)); // , std::ref(blueprintptrpass)
	//auto start4 = std::chrono::high_resolution_clock::now();
	//std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeCollection, this, Key2, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections), std::ref(passCollectionPtr2), std::ref(passManifestPtr2)); // , std::ref(blueprintptrpass)
	//std::future<void> coll_2 = tpref2->submit5(&OrganicSystem::JobMaterializeCollection, this, Key2, std::ref(BlueprintMatrix), std::ref(EnclaveCollections), std::ref(ManifestCollections), std::ref(RenderCollections));
	//auto finish4 = std::chrono::high_resolution_clock::now();
	auto start4 = std::chrono::high_resolution_clock::now();
	coll_1.wait();
	auto finish4 = std::chrono::high_resolution_clock::now();
	//auto start4 = std::chrono::high_resolution_clock::now();
	//coll_2.wait();
	//auto finish4 = std::chrono::high_resolution_clock::now();															// optional, for debugging
	std::chrono::duration<double> elapsed4 = finish4 - start4;
	cout << "Dual coollection instantiation speed:  " << elapsed4.count() << endl;
}

void OrganicSystem::SetupFutureCollection(int x, int y, int z)
{
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