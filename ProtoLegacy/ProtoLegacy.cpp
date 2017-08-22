// ProtoLegacy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BasicEntity.h"
#include "GoalMoveToLocation.h"
#include <iostream>
#include <unordered_map>
#include "Enclave.h"
#include "EnclaveManifest.h"
#include "EnclaveKeyDef.h"
#include "EnclaveMatrix.h"
#include "EnclaveMultiJob.h"
#include "EnclaveCollectionMesh.h"
#include "ManifestCollection.h"
#include "ManifestCollectionMatrix.h"
#include "EnclaveCollectionMatrix.h"
#include "RenderCollectionMatrix.h"
#include "OrganicSystem.h"
#include "EnclaveCollectionBlueprint.h"
#include "EnclavePainter.h"
#include "EnclavePainterList.h"
#include "thread_pool.h"
#include <thread>

//#define GLEW_STATIC
#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>

//#include "shader.hpp"
#include "common/shader.hpp"
GLFWwindow* window;



#include "ProtoLegacy.h"
#include <chrono>
#include <future>

std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher> m;								// enclave matrix

//std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator m_iter;				// iterator
std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> m_itermap1, m_itermap2;
std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>::iterator m_itermapiter, m_itermapiter2;
std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> *m_itermapptr;

/* Future/promise experimentation*/
std::promise<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> promise1;
std::future<std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher>> future1 = promise1.get_future();

//std::unordered_map<EnclaveKeyDef::EnclaveKey, std::unordered_map<EnclaveKeyDef::EnclaveKey, Enclave, EnclaveKeyDef::KeyHasher>::iterator, EnclaveKeyDef::KeyHasher> *m_itermap1ptr, *m_itermap2ptr;



int main()
{
	/* Matrix creation area*/
	EnclaveCollectionMatrix EnclaveCollections;									// matrix of enclave collections (512 chunks, dynamically allocated array)
	ManifestCollectionMatrix ManifestCollections(&EnclaveCollections);		// matrix of manifest collections (up to 512 chunks, unordered map)
	RenderCollectionMatrix RenderCollections(&ManifestCollections);

	EnclaveKeyDef::EnclaveKey EnclaveCollectionTestKey;
	EnclaveCollectionTestKey.x = 0;
	EnclaveCollectionTestKey.y = 0;
	EnclaveCollectionTestKey.z = 0;
	char val;
	int count = 0;
	int collectcount = 0;

	/* Organic testing. */
	
	// STAGE 1: blueprint set up
	EnclaveCollectionBlueprint testBlueprint;
	typedef unsigned char(&ElevationMapRef)[8][8];
	unsigned char tempSurface[8][8] = { 0 };				// temp array for surface data
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			tempSurface[x][z] = 64;			// error if anything other than 128?

			int chunkbitmask = 128;								// set initial value of bitmask to be 128 (which is the top chunk)
			if ((tempSurface[x][z] & chunkbitmask) == chunkbitmask)
			{

				cout << "TEST of chunkbitmask... " << (int)tempSurface[x][z] << endl;
			}
 		}
	}
	unsigned char tempSolid[8][8] = { 0 };					// temp array for solid data
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			tempSolid[x][z] = 127;
		}
	}

	//auto bluestart = std::chrono::high_resolution_clock::now();
	unsigned char tempPaintables[8][8] = { 0 };
	tempPaintables[0][0] = 64;								// flag a chunk to be paintable, -- in this case it is 0, 6, 0
	tempPaintables[1][0] = 64;								// following lines are performance testing only
	tempPaintables[2][0] = 64;
	tempPaintables[3][0] = 64;
	tempPaintables[4][0] = 64;
	tempPaintables[5][0] = 64;
	tempPaintables[6][0] = 64;
	tempPaintables[7][0] = 64;

	// set up a temporary paint job, add it to a list, and add the list to the blueprint's matrix here
	EnclavePainter testPaint, testPaint2, testPaint3, testPaint4, testPaint5, testPaint6, testPaint7;								
	testPaint.PaintData[0] = 64;				// sets a target "dirt" block within the chunk (?? check this later)
	testPaint2.PaintData[0] = 32;				// sets another target block "grass" (for a different material) within the chunk (?? check this later)
	testPaint3.PaintData[1] = 32;
	testPaint4.PaintData[1] = 16;

	EnclavePainterList testPaintList;			// create a paint list
	testPaintList.PaintList[2] = testPaint;		// add the paint job for "dirt" to the list
	testPaintList.PaintList[3] = testPaint2;	// add the paint job for "grass" to the list
	testPaintList.PaintList[4] = testPaint3;
	testPaintList.PaintList[5] = testPaint3;





	ElevationMapRef SurfaceChunks = tempSurface;
	ElevationMapRef SolidChunks = tempSolid;
	ElevationMapRef PaintableChunks = tempPaintables;

	testBlueprint.SetSurfaceChunkData(SurfaceChunks);
	testBlueprint.SetSolidChunkData(SolidChunks);
	testBlueprint.SetPaintableChunkData(PaintableChunks);

	EnclaveKeyDef::EnclaveKey tempPainterKey;
	tempPainterKey.x = 0;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 1;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 2;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 3;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 4;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 5;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	tempPainterKey.x = 6;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);

	
	tempPainterKey.x = 7;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;
	//auto bluestart = std::chrono::high_resolution_clock::now();
	//testBlueprint.AddNewPaintList2(tempPainterKey, testPaintList);









	//auto blueend = std::chrono::high_resolution_clock::now();
	//ElevationMapRef SolidChunks;

	EnclaveCollectionBlueprint testBlueprint2;
	EnclaveKeyDef::EnclaveKey bpkeytest;
	bpkeytest.x = 0;
	bpkeytest.y = 0;
	bpkeytest.z = 1;
	
	

	// STAGE 2: initialization of enclaves 

	// *********** thread pool set up
	thread_pool mainthreadpool;
	thread_pool* mainthreadpoolref = &mainthreadpool;

	thread_pool mainthreadpool2;
	thread_pool* mainthreadpoolref2 = &mainthreadpool2;

	




	
	OrganicSystem Organic;
	Organic.SetOrganicCell1(mainthreadpoolref);				// set the Organic instance's first worker thread
	Organic.SetOrganicCell2(mainthreadpoolref2);			// set the Organic instance's second worker thread
	Organic.AddOrganicTextureMetaArray("base");					// set up the texture map; first ever map will be named "base"

	// *********** Enclave Collection load type 2: instantiate a set of collections

	EnclaveKeyDef::EnclaveKey key1, key2, key3, key4, key5, key6, key7, key8;
	key1.x = 5;
	key1.y = 0;
	key1.z = 1;

	key2.x = 6;
	key2.y = 0;
	key2.z = 1;

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

	// add/instantiate the new collections here
	Organic.AddBlueprint(EnclaveCollectionTestKey.x, EnclaveCollectionTestKey.y, EnclaveCollectionTestKey.z, testBlueprint);
	Organic.AddBlueprint(key1.x, key1.y, key1.z, testBlueprint);
	Organic.AddBlueprint(key2.x, key2.y, key2.z, testBlueprint);
	Organic.AddBlueprint(key3.x, key3.y, key3.z, testBlueprint);
	Organic.AddBlueprint(key4.x, key4.y, key4.z, testBlueprint);
	Organic.AddBlueprint(key5.x, key5.y, key5.z, testBlueprint);
	Organic.AddBlueprint(key6.x, key6.y, key6.z, testBlueprint);
	Organic.AddBlueprint(key7.x, key7.y, key7.z, testBlueprint);
	Organic.AddBlueprint(key8.x, key8.y, key8.z, testBlueprint);


	Organic.SetupFutureCollectionMM(key1.x, key1.y, key1.z);
	Organic.SetupFutureCollectionMM(key2.x, key2.y, key2.z);
	Organic.SetupFutureCollectionMM(key4.x, key4.y, key4.z);
	Organic.SetupFutureCollectionMM(key5.x, key5.y, key5.z);
	Organic.SetupFutureCollectionMM(key6.x, key6.y, key6.z);
	Organic.SetupFutureCollectionMM(key7.x, key7.y, key7.z);
	Organic.SetupFutureCollectionMM(key8.x, key8.y, key8.z);
	Organic.SetupFutureCollectionMM(key3.x, key3.y, key3.z);

	Organic.MaterializeCollection(key1, key2);

	cout << "-------------------------PASS" << endl;
	// *********** Enclave Collection load type 1: add and instantiate a single collection
		// add the test blueprint to the OrganicSystem
	auto bluestart = std::chrono::high_resolution_clock::now();
	Organic.AddBlueprint(bpkeytest.x, bpkeytest.y, bpkeytest.z, testBlueprint2);
	auto blueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> blueelapsed = blueend - bluestart;
	std::cout << "Elapsed time (Blueprint addition): " << blueelapsed.count() << endl;
	//cout << Organic.BlueprintMatrix.BlueprintMap[EnclaveCollectionTestKey].SolidChunks[0][0];
	cout << "testing of solidChunk data in blueprints found in the OrganicSystem: " << endl;
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			cout << (int)Organic.BlueprintMatrix.BlueprintMap[EnclaveCollectionTestKey].SolidChunks[x][z] << endl;
		}
	}

	auto orgstart = std::chrono::high_resolution_clock::now();
	Organic.AddAndMaterializeSingleCollectionMM(0, 0, 0);
	auto orgend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> orgelapsed = orgend - orgstart;
	std::cout << "Elapsed time (Organic collection instantiation): " << orgelapsed.count() << endl;

	auto orgstart2 = std::chrono::high_resolution_clock::now();
	//Organic.ChangeSingleBlockMaterialAtXYZ(25, 25, 0, 1);
	auto orgend2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> orgelapsed2 = orgend2 - orgstart2;
	std::cout << "Elapsed time (Changing material of single block): " << orgelapsed2.count() << endl;












	/* Collection creation testing*/
	EnclaveCollections.AddNewCollection(3, 0, 0);		//	???? when commented (Google std::unordered_map::operator[] for reason)
	EnclaveCollections.AddNewCollection(2, 0, 0);
	EnclaveCollections.AddNewCollection(1, 0, 0);
	EnclaveCollections.AddNewCollection(0, 0, 0);	// adds a new collection with a key of 0,0,0 <<----------used for testing
	EnclaveCollections.AddNewCollection(-1, 0, 0);	// adds a new collection with a key of 0,0,0
	EnclaveCollections.AddNewCollection(-2, 0, 0);	// adds a new collection with a key of 0,0,0
	EnclaveCollections.AddNewCollection(-3, 0, 0);	// adds a new collection with a key of 0,0,0
	

	/* Manifest Attachment testing */
	 
	// unveil polys for 8 different enclaves

	auto start = std::chrono::high_resolution_clock::now();

	Enclave *tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(69, 0, 0);				// MUST be a pointer, to not get a copy!!
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 32, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 32, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);

	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(73, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 32, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 32, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);

	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(77, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 32, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 32, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);
	//
	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(81, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);
	//
	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(85, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);
	//
	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(89, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);
	//
	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(93, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(1, 0, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 0, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 0, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 0, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 0, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 0, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 0, 1, 0, 1, 0, 40, 0);
	//tempEnclave->UnveilSinglePoly(3, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(2, 1, 1, 0, 1, 0, 40, 0);						// 8 total triangles after this call
	//tempEnclave->UnveilSinglePoly(1, 1, 1, 0, 1, 0, 40, 0);													// reveal the polygon to the world	
	//tempEnclave->UnveilSinglePoly(0, 1, 1, 0, 1, 0, 40, 0);


	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(0, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);

	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(4, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);

	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(8, 0, 0);
	tempEnclave->UnveilSinglePoly(3, 0, 0, 0, 1, 0, 40, 0);





	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time (Massive Unveil Poly call: , " << count << "): " << elapsed.count() << endl;

	cout << "re test of enclave: " << tempEnclave->GetTotalTrianglesInEnclave() << endl;
	
	EnclaveKeyDef::EnclaveKey testkey;
	testkey.x = 3;
	testkey.y = 0;
	testkey.z = 0;

	EnclaveKeyDef::EnclaveKey testkey2;
	testkey2.x = 0;
	testkey2.y = 0;
	testkey2.z = 0;
	// set up manifest collection
	//ManifestCollections.AddNewCollection(testkey);							// Step 1: create a collection that has same Key as an existing EnclaveCollection
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 1, 0, 0);	// Step 2: add a single(or multiple) manifest(s) to the ManifestCollection (same key as EnclaveCollection). This only needs to be called once per Enclave. (for parallelism/multi-threading safety)
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 2, 0, 0);	// attaches a manifest to a manifest collection that has a key of "testkey," and x/y/z coord of following 3 arguments
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 3, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 4, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 5, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 6, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey, 7, 0, 0);
	//
	//ManifestCollections.AddNewCollection(testkey2);
	//cout << "debug line" << endl;
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 0, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 1, 0, 0);
	//ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 2, 0, 0);

	

	// set up final array(s) for OpenGL rendering here
	//ManifestCollections.GetColletedEnclaveManifestAt(testkey, 3, 0, 0);
	auto start7 = std::chrono::high_resolution_clock::now();
	cout << "debug line 2" << endl;
	//RenderCollections.CreateRenderArrayFromManifestCollection(testkey2);										//change here... OLD: testkey
	auto finish7 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed7 = finish7 - start7;
	std::cout << "Elapsed time (Creating Render Array test: " << collectcount << "): " << elapsed7.count() << endl;


	//ManifestCollections.GetCollectedEnclaveManifestAt(testkey2, 1, 0, 0);									//change here... OLD: testkey
	// NEW TEST: (7/2/2017) -- check if RenderCollection::UpdateManifestArray is called
	//Enclave *tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(69, 0, 0);				// MUST be a pointer, to not get a copy!!
	tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(0, 0, 0);											//change here... OLD: tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(77, 0, 0);		
	cout << "Before new unveil call... " << endl;
	tempEnclave->UnveilSinglePoly(3, 2, 0, 0, 1, 0, 40, 0);

	auto start2 = std::chrono::high_resolution_clock::now();
	//ManifestCollections.UpdateAttachedManifest(testkey2, 0, 0, 0);				// problem is after this call	// change here... OLD: testkey
	auto finish2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::cout << "Elapsed time (Creating Render Array test 2: , " << count << "): " << elapsed2.count() << endl;


	//ManifestCollections.GetColletedEnclaveManifestAt(testkey, 1, 0, 0);
	//RenderCollections.CreateRenderArrayFromManifestCollection(testkey);

	//auto finish7 = std::chrono::high_resolution_clock::now();


			// elapsed time is  hhere
																																	//std::cout << "Size of MEMatrix: " << sizeof(MEMatrix) << endl;



				// elapsed time is  hhere



	


	
	auto start3 = std::chrono::high_resolution_clock::now();
	auto finish3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	std::cout << "Elapsed time (MeshMatrix- adding pointer to array, " << elapsed3.count() << "): " << endl;						// elapsed time is  hhere



	cout << "preparing to clean all enclaves..." << endl;
	cin >> val;



	int choice, in_x, in_y, in_z, in_otherflags;

	// ------------------------------------BEGIN OPEN GL SET UP

	// Initialise GLFW

	//Organic.SetGraphicsAPI();

	Organic.SetGraphicsAPI();

	GLfloat* tempTestPtr = Organic.GetVertexDataFromRenderCollection(0,0,0);
	Organic.SendDataFromCollectionToGLBuffer(tempTestPtr, 73728);
	// ------------------------------------END OPEN GL SET UP


	// ------------------------------------MAIN WORLD LOOP			NOTE: use cout << fixed for exact timestamp values!
	do {

		Organic.RenderGLTerrain();	// perform render frame work

		/*
		auto start3 = std::chrono::high_resolution_clock::now();
		float testfloats[18432] = { 0.0f };							// 288 = 32 triangles ( 1 chunk whole face)
																	// 4608 = 512 triangles (16 chunk whole faces)
																	// 18432 = 2048 triangles (64 chunk whole faces) = 3.8 X e -06
		auto finish3 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed3 = finish3 - start3;
		cout << "Float initialization time = " << elapsed3.count() << endl;

		cout << "enter x,y,z: " << endl;
		cin >> in_x;
		cin >> in_y;
		cin >> in_z;
		cout << "enter value for in_otherflags:  " << endl;
		cin >> in_otherflags;

		switch (in_otherflags)
		{
			case 0:
				
				//EnclavePtr->VeilSinglePoly(in_x, in_y, in_z, 0, in_otherflags, 0, 40, 0); // hide the polygon  from rendering
				cout << "case 0 hit" << endl;
				break;
			case 1:
				auto start4 = std::chrono::high_resolution_clock::now();
				//EnclavePtr->UnveilSinglePoly(in_x, in_y, in_z, 0, in_otherflags, 0, 40, 0); // reveal the polygon to the world	
				auto finish4 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed4 = finish4 - start4;
				cout << "case 1 hit; duration = " << elapsed4.count() << endl;
				break;
		}

		cout << "after single poly sort: " << endl;







		auto start6 = std::chrono::high_resolution_clock::now();
		auto finish6 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed6 = finish6 - start6;
		cout << "Attachment process time  = " << elapsed6.count() << endl;
		
		cout << "continue testing?" << endl;
		cin >> choice; // must be a number!
		*/
	} 
	while (glfwGetKey(Organic.OGLM.GLwindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&	// loop until escape key is pressed in OpenGL window
		glfwWindowShouldClose(Organic.OGLM.GLwindow) == 0);
	// -------------------------------------END MAIN WORLD LOOP

	Organic.GLCleanup();			// cleanup OpenGL window, buffers, arrays, etc

	cout << "Press any number key to end.";
	int somevalue;
	cin >> somevalue;

	// Cleanup VBO
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteVertexArrays(1, &VertexArrayID);
	//glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	//glfwTerminate();

	
    return 0;
}


