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
	ElevationMapRef SurfaceChunks = tempSurface;
	ElevationMapRef SolidChunks = tempSolid;

	testBlueprint.SetSurfaceChunkData(SurfaceChunks);
	testBlueprint.SetSolidChunkData(SolidChunks);
	
	//ElevationMapRef SolidChunks;



	
	

	// STAGE 2: initialization of enclaves 
	OrganicSystem Organic;
	Organic.AddBlueprint(EnclaveCollectionTestKey.x, EnclaveCollectionTestKey.y, EnclaveCollectionTestKey.z, testBlueprint);	// add the test blueprint to the OrganicSystem
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
	Organic.AddAndMaterializeCollection(0, 0, 0);
	auto orgend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> orgelapsed = orgend - orgstart;
	std::cout << "Elapsed time (Organic collection instantiation): " << orgelapsed.count() << endl;

	auto orgstart2 = std::chrono::high_resolution_clock::now();
	Organic.ChangeSingleBlockMaterialAtXYZ(25, 25, 0, 1);
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
	ManifestCollections.AddNewCollection(testkey);							// Step 1: create a collection that has same Key as an existing EnclaveCollection
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 1, 0, 0);	// Step 2: add a single(or multiple) manifest(s) to the ManifestCollection (same key as EnclaveCollection). This only needs to be called once per Enclave. (for parallelism/multi-threading safety)
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 2, 0, 0);	// attaches a manifest to a manifest collection that has a key of "testkey," and x/y/z coord of following 3 arguments
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 3, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 4, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 5, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 6, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey, 7, 0, 0);

	ManifestCollections.AddNewCollection(testkey2);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 0, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 1, 0, 0);
	ManifestCollections.AttachManifestToCollectedEnclave(testkey2, 2, 0, 0);

	

	// set up final array(s) for OpenGL rendering here
	//ManifestCollections.GetColletedEnclaveManifestAt(testkey, 3, 0, 0);
	auto start7 = std::chrono::high_resolution_clock::now();
	RenderCollections.CreateRenderArrayFromManifestCollection(testkey2);										//change here... OLD: testkey
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
	ManifestCollections.UpdateAttachedManifest(testkey2, 0, 0, 0);				// problem is after this call	// change here... OLD: testkey
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
	if (!glfwInit())
	{
		fprintf(stderr, "||||||||||||||||Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "----------------Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLfloat *g_vertex_buffer_data2 = new GLfloat[9];
	g_vertex_buffer_data2[0] = -1.0f;
	g_vertex_buffer_data2[1] = -1.0f;
	g_vertex_buffer_data2[2] = 0.0f;
	g_vertex_buffer_data2[3] = 1.0f;
	g_vertex_buffer_data2[4] = -1.0f;
	g_vertex_buffer_data2[5] = 0.0f;
	g_vertex_buffer_data2[6] = 0.0f;
	g_vertex_buffer_data2[7] = 1.0f;
	g_vertex_buffer_data2[8] = 0.0f;

	cout << "test of buffer data: " << sizeof(g_vertex_buffer_data) << endl;

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 36, g_vertex_buffer_data2, GL_STATIC_DRAW);		// note, for 2nd argument: must be exact length of the array being used; cannot do sizeof(pointer to array).



	// ------------------------------------END OPEN GL SET UP


	// ------------------------------------MAIN WORLD LOOP			NOTE: use cout << fixed for exact timestamp values!
	do {
		// OPEN GL TEST
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle


		//std::cout << "RUN MULTI JOB 1 ELAPSED ITERATOR TIME: " << elapsed4.count() << std::endl;

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		// END OPEN GL TEST




		//AddEnclaveToMatrix(0, 0, 1);
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
	} while (choice != 0);
	// -------------------------------------END MAIN WORLD LOOP

	cout << "Press any number key to end.";
	int somevalue;
	cin >> somevalue;

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	
    return 0;
}


