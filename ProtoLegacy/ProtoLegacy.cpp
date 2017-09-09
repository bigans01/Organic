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

int main()
{

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
			tempSolid[x][z] = 255;			// set up all 255 enclave as being solids
		}
	}

	//auto bluestart = std::chrono::high_resolution_clock::now();
	unsigned char tempPaintables[8][8] = { 0 };
	tempPaintables[0][0] = 64;								// flag a chunk to be paintable, -- in this case it is 0, 6, 0
	tempPaintables[1][0] = 64;								// chunk at 1, 6, 0 will be paintable...
	tempPaintables[2][0] = 64;								// chunk at 2, 6, 0 will be paintable...
	tempPaintables[3][0] = 64;
	tempPaintables[4][0] = 64;
	tempPaintables[5][0] = 64;
	tempPaintables[6][0] = 64;
	tempPaintables[7][0] = 64;

	// set all 512 enclaves up for painting
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			tempPaintables[x][z] = 255;
			//cout << "value: (" << x << ", " << z << "):" << int(tempPaintables[x][z]) << endl;
 		}
	}

	// set up a temporary paint job, add it to a list, and add the list to the blueprint's matrix here
	EnclavePainter testPaint, testPaint2, testPaint3, testPaint4, testPaint5, testPaint6, testPaint7;							
	
	// NOTE: PaintData is a range between 0 and 7; this indicates 64 different blocks.
	testPaint.PaintData[0] = 255;				// sets a target "dirt" block within the chunk (?? check this later)
	testPaint.PaintData[1] = 255;
	testPaint.PaintData[2] = 255;
	testPaint.PaintData[3] = 255;
	testPaint.PaintData[4] = 255;
	testPaint.PaintData[5] = 255;
	testPaint.PaintData[6] = 255;
	testPaint.PaintData[7] = 255;

	testPaint2.PaintData[0] = 32;				// sets another target block "grass" (for a different material) within the chunk (?? check this later)
	testPaint3.PaintData[1] = 32;
	testPaint4.PaintData[1] = 16;

	EnclavePainterList testPaintList;			// create a paint list
	testPaintList.PaintList[2] = testPaint;		// add the paint job for "dirt" to the list; the 2 indicates the block type that will be painted.
	//testPaintList.PaintList[3] = testPaint2;	// add the paint job for "grass" to the list
	//testPaintList.PaintList[4] = testPaint3;
	//testPaintList.PaintList[5] = testPaint3;





	ElevationMapRef SurfaceChunks = tempSurface;
	ElevationMapRef SolidChunks = tempSolid;
	ElevationMapRef PaintableChunks = tempPaintables;
	
	testBlueprint.SetSurfaceChunkData(SurfaceChunks);
	testBlueprint.SetSolidChunkData(SolidChunks);
	testBlueprint.SetCustomPaintableChunkData(PaintableChunks);

	EnclaveKeyDef::EnclaveKey tempPainterKey;
	/*
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
	*/
	auto bpstart = std::chrono::high_resolution_clock::now();
	//auto bpstart = std::chrono::high_resolution_clock::now();
	int bptestcount = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int z = 0; z < 8; z++)
			{
				tempPainterKey.x = x;
				tempPainterKey.y = y;
				tempPainterKey.z = z;
				testBlueprint.AddNewPaintList(tempPainterKey, testPaintList);
				bptestcount++;
			}
		}
	}
	auto bpend = std::chrono::high_resolution_clock::now();
	//cout << "bptestcount: " << bptestcount << endl;



	//Organic.AddBlueprint(bpkeytest.x, bpkeytest.y, bpkeytest.z, testBlueprint2);
	//auto bpend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> bpelapsed = bpend - bpstart;
	std::cout << "Elapsed time (Blueprint loop): " << bpelapsed.count() << endl;

	
	tempPainterKey.x = 7;
	tempPainterKey.y = 6;
	tempPainterKey.z = 0;


	// STAGE 2: initialization of enclaves 

	// *********** thread pool set up


	thread_pool mainthreadpool;
	thread_pool* mainthreadpoolref = &mainthreadpool;

	thread_pool mainthreadpool2;
	thread_pool* mainthreadpoolref2 = &mainthreadpool2;

	
	EnclaveCollectionBlueprint testBlueprint3;



	// New blueprint style testing
	auto carvestart = std::chrono::high_resolution_clock::now();

	testBlueprint3.CarveSlope();
	//cout << "blueprint size: " << sizeof(testBlueprint3) << endl;

	auto carveend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> carveelapsed = carveend - carvestart;
	//std::cout << "Elapsed time (Carve time): " << carveelapsed.count() << endl;




	
	//OrganicSystem Organic;
	auto STARTUPbegin = std::chrono::high_resolution_clock::now();
	OrganicSystem Organic(2, 8);					// number of factories, buffer size
	Organic.LoadNWChunks();
	Organic.SetOrganicCell1(mainthreadpoolref);				// set the Organic instance's first worker thread
	Organic.SetOrganicCell2(mainthreadpoolref2);			// set the Organic instance's second worker thread
	Organic.AddOrganicTextureMetaArray("base");					// set up the texture map; first ever map will be named "base"
	Organic.AddOrganicVtxColorMetaArray("base");

	// add first 4 keys to render
	
	/*
		(9/4/2017)
		RAM USAGE NOTES:   total number of collections	|	before GL	|	post GL
							8							|	 353		|	 470
							64							|	 488		|	 602
							256							|    954        |    1082
	*/

	auto collectionsSetupBEGIN = std::chrono::high_resolution_clock::now();
	
	int cmultiplier = 1;
	for (int x = 0; x < 4*cmultiplier; x++)						
													
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = x;
		tempKeyToAdd.y = 0;
		tempKeyToAdd.z = 0;
		Organic.AddKeyToRenderList(tempKeyToAdd);
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);
	}
	

	
	// add second 4 keys to render
	
	for (int x = 0; x < 4*cmultiplier; x++)						
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = x;
		tempKeyToAdd.y = 0;
		tempKeyToAdd.z = 1;
		Organic.AddKeyToRenderList(tempKeyToAdd);
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);
	}
	auto collectionsSetupEND = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> collectionsSetupELAPSED = collectionsSetupEND - collectionsSetupBEGIN;
	//std::cout << "Elapsed time (Multiple collection instantiation): " << collectionsSetupELAPSED.count() << endl;
	
	int height = 3;
	/* vertical blueprints */
	for (int y = 0; y < height; y++)
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = 0;
		tempKeyToAdd.y = -y;
		cout << "debug: " << y << endl;
		tempKeyToAdd.z = 2;
		Organic.AddKeyToRenderList(tempKeyToAdd);
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);

	}

	/*debug loop*/
	for (int y = 0; y < height; y++)
	{
		EnclaveKeyDef::EnclaveKey keyToFind;
		keyToFind.x = 0;
		keyToFind.y = -y;
		keyToFind.z = 2;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator tempIter = Organic.BlueprintMatrix.BlueprintMap.find(keyToFind);
		if (tempIter != Organic.BlueprintMatrix.BlueprintMap.end())
		{
			cout << "Key was found" << endl;
		}

	}

	/*
	EnclaveKeyDef::EnclaveKey NEWkey1, NEWkey2, NEWkey3, NEWkey4, NEWkey5, NEWkey6, NEWkey7, NEWkey8;
	NEWkey1.x = 0;
	NEWkey1.y = 0;
	NEWkey1.z = 0;

	NEWkey2.x = 1;
	NEWkey2.y = 0;
	NEWkey2.z = 0;

	NEWkey3.x = 2;
	NEWkey3.y = 0;
	NEWkey3.z = 0;

	NEWkey4.x = 3;
	NEWkey4.y = 0;
	NEWkey4.z = 0;

	NEWkey5.x = 0;
	NEWkey5.y = 0;
	NEWkey5.z = 1;

	NEWkey6.x = 1;
	NEWkey6.y = 0;
	NEWkey6.z = 1;

	NEWkey7.x = 2;
	NEWkey7.y = 0;
	NEWkey7.z = 1;

	NEWkey8.x = 3;
	NEWkey8.y = 0;
	NEWkey8.z = 1;
	auto bluestart = std::chrono::high_resolution_clock::now();
	Organic.AddBlueprint(NEWkey1.x, NEWkey1.y, NEWkey1.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey2.x, NEWkey2.y, NEWkey2.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey3.x, NEWkey3.y, NEWkey3.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey4.x, NEWkey4.y, NEWkey4.z, testBlueprint3);

	
	Organic.AddBlueprint(NEWkey5.x, NEWkey5.y, NEWkey5.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey6.x, NEWkey6.y, NEWkey6.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey7.x, NEWkey7.y, NEWkey7.z, testBlueprint3);
	Organic.AddBlueprint(NEWkey8.x, NEWkey8.y, NEWkey8.z, testBlueprint3);
	
	auto blueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> blueelapsed = blueend - bluestart;
	std::cout << "Elapsed time (Blueprint addition): " << blueelapsed.count() << endl;
	*/
	Organic.ArrayTest();
	Organic.MaterializeAllCollectionsInRenderList(0);			// 0 = use a set of Factories, 1 = use a ManifestMatrix style


	//cout << "-------------------------PASS" << endl;
	// *********** Enclave Collection load type 1: add and instantiate a single collection
		// add the test blueprint to the OrganicSystem
	//auto bluestart = std::chrono::high_resolution_clock::now();
	//Organic.AddBlueprint(bpkeytest.x, bpkeytest.y, bpkeytest.z, testBlueprint2);
	//auto blueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> blueelapsed = blueend - bluestart;
	
	//cout << Organic.BlueprintMatrix.BlueprintMap[EnclaveCollectionTestKey].SolidChunks[0][0];
	/*
	cout << "testing of solidChunk data in blueprints found in the OrganicSystem: " << endl;
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			cout << (int)Organic.BlueprintMatrix.BlueprintMap[EnclaveCollectionTestKey].SolidChunks[x][z] << endl;
		}
	}
	*/
	auto orgstart = std::chrono::high_resolution_clock::now();
	//Organic.AddAndMaterializeSingleCollectionMM(0, 0, 0);
	//Organic.AddAndMaterializeSingleCollectionMM(1, 0, 1);
	//Organic.AddAndMaterializeSingleCollectionMM(2, 0, 1);
	auto orgend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> orgelapsed = orgend - orgstart;
	std::cout << "Elapsed time (Organic collection instantiation): " << orgelapsed.count() << endl;

	auto orgstart2 = std::chrono::high_resolution_clock::now();
	//Organic.ChangeSingleBlockMaterialAtXYZ(25, 25, 0, 1);
	auto orgend2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> orgelapsed2 = orgend2 - orgstart2;
	std::cout << "Elapsed time (Changing material of single block): " << orgelapsed2.count() << endl;

	

	/* Manifest Attachment testing */
	 
	// unveil polys for 8 different enclaves

	auto start = std::chrono::high_resolution_clock::now();


	//Organic.AnalyzeRenderArray(2, 0, 1, 0);



	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	//std::cout << "Elapsed time (Massive Unveil Poly call: , " << count << "): " << elapsed.count() << endl;

	auto start7 = std::chrono::high_resolution_clock::now();
	cout << "debug line 2" << endl;
	//RenderCollections.CreateRenderArrayFromManifestCollection(testkey2);										//change here... OLD: testkey
	auto finish7 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed7 = finish7 - start7;
	//std::cout << "Elapsed time (Creating Render Array test: " << collectcount << "): " << elapsed7.count() << endl;


	//ManifestCollections.GetCollectedEnclaveManifestAt(testkey2, 1, 0, 0);									//change here... OLD: testkey

	//Enclave *tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(69, 0, 0);				// MUST be a pointer, to not get a copy!!
	//tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(0, 0, 0);											//change here... OLD: tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(77, 0, 0);		
	cout << "Before new unveil call... " << endl;
	//tempEnclave->UnveilSinglePolyWithMtrl(3, 2, 0, 0, 1, 0, 40, 0);

	auto start2 = std::chrono::high_resolution_clock::now();
	//ManifestCollections.UpdateAttachedManifest(testkey2, 0, 0, 0);				// problem is after this call	// change here... OLD: testkey
	auto finish2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::cout << "Elapsed time (Creating Render Array test 2: , " << count << "): " << elapsed2.count() << endl;


	//ManifestCollections.GetColletedEnclaveManifestAt(testkey, 1, 0, 0);
	//cout << "preparing to clean all enclaves..." << endl;
	//cin >> val;



	int choice, in_x, in_y, in_z, in_otherflags;

	// ------------------------------------BEGIN OPEN GL SET UP

	// Initialise GLFW
	Organic.SetGraphicsAPI();												// setup the graphics API (OpenGL context, etc)
	auto start3 = std::chrono::high_resolution_clock::now();				// benchmark testing only

	// send all processed collections to OPEN GL	
	Organic.SetRenderMode(1);
	Organic.SendRenderListToGLTerrainBuffer();							
	/*
	RenderCollection* newRenderCollPtr;

	newRenderCollPtr = Organic.GetRenderCollectionPtr(0, 0, 0);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(1, 0, 0);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(2, 0, 0);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(3, 0, 0);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(0, 0, 1);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(1, 0, 1);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(2, 0, 1);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);

	newRenderCollPtr = Organic.GetRenderCollectionPtr(3, 0, 1);
	Organic.SendDataFromRenderPtrToGLBuffer(newRenderCollPtr);
	*/



	auto finish3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	std::cout << "Elapsed time, sending data to OpenGL buffer: " << elapsed3.count() <<  endl;						// elapsed time is  hhere


	auto STARTUPend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> STARTUPduration = STARTUPend - STARTUPbegin;
	std::cout << "Total pre-loop startup time: " << STARTUPduration.count() << endl;

	// ------------------------------------END OPEN GL SET UP


	// ------------------------------------MAIN WORLD LOOP			NOTE: use cout << fixed for exact timestamp values!
	//Organic.SetRenderMode(0);
	do {

		auto start3 = std::chrono::high_resolution_clock::now();
		Organic.RenderGLTerrain();	// perform render frame work
		auto finish3 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed3 = finish3 - start3;
		//std::cout << "Frame render time: " << elapsed3.count() << endl;
	} 
	while (glfwGetKey(Organic.OGLM.GLwindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&	// loop until escape key is pressed in OpenGL window
		glfwWindowShouldClose(Organic.OGLM.GLwindow) == 0);
	// -------------------------------------END MAIN WORLD LOOP

	Organic.GLCleanup();			// cleanup OpenGL window, buffers, arrays, etc

	cout << "Press any number key to end.";
	int somevalue;
	cin >> somevalue;


	
    return 0;
}


