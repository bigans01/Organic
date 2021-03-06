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
#include "CursorPathTraceContainer.h"
#include "EnclavePainter.h"
#include "EnclavePainterList.h"
#include "thread_pool.h"
#include <thread>
#include <memory>

//#define GLEW_STATIC
#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#include "shader.hpp"
#include "common/shader.hpp"
GLFWwindow* window;



#include "ProtoLegacy.h"
#include <chrono>
#include <future>

int main()
{
	int count = 0;

	/*
	EnclaveKeyDef::EnclaveKey EnclaveCollectionTestKey;
	EnclaveCollectionTestKey.x = 0;
	EnclaveCollectionTestKey.y = 0;
	EnclaveCollectionTestKey.z = 0;
	char val;
	int collectcount = 0;

	
	
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
	*/

	// STAGE 2: initialization of enclaves 

	// *********** thread pool set up


	//thread_pool mainthreadpool;
	//thread_pool* mainthreadpoolref = &mainthreadpool;

	//thread_pool mainthreadpool2;
	//thread_pool* mainthreadpoolref2 = &mainthreadpool2;

	//thread_pool mainthreadpool3;
	//thread_pool* mainthreadpoolref3 = &mainthreadpool3;

	
	EnclaveCollectionBlueprint testBlueprint3, flatBlueprint;



	// New blueprint style testing
	auto carvestart = std::chrono::high_resolution_clock::now();

	// ------------------- flat blueprint settings ----------------------------
	flatBlueprint.FlattenToElevation();
	/*
	int flatEastWall = 0;
	int flatEastWallBitShift = 1;
	for (int x = 3; x > 0; x--)
	{
		flatEastWallBitShift <<= x;
		flatEastWall = flatEastWall | flatEastWallBitShift;
		flatEastWallBitShift = 1;
	}

	int flatArrayToPass[32];
	for (int x = 0; x < 32; x++)
	{
		flatArrayToPass[x] = flatEastWall;
	}

	*/
	// ------------------- sloped blueprint settings --------------------------
	testBlueprint3.CarveSlope();
	/*
	//cout << "blueprint size: " << sizeof(testBlueprint3) << endl;
	//auto carveend = std::chrono::high_resolution_clock::now();
	int setEastWall = 0;
	int setEastWallBitShift = 1;
	for (int x = 31; x > 0; x--)
	{
		setEastWallBitShift <<= x;		// shift by x
		setEastWall = setEastWall | setEastWallBitShift;
		setEastWallBitShift = 1;		// reset to 1
	}
	int arrayToPass1[32];

	for (int x = 0; x < 32; x++)
	{
		arrayToPass1[x] = setEastWall;
	}
	
	//testBlueprint3.DetermineBorderWall(8, arrayToPass1);		// set this value for the east wall

	


	int setWestWall = 0;
	int setWestWallBitShift = 1;
	for (int x = 24; x > 0; x--)
	{
		setWestWallBitShift <<= x;		// shift by x
		setWestWall = setWestWall | setWestWallBitShift;
		setWestWallBitShift = 1;		// reset to 1
	}
	int arrayToPass2[32];
	for (int x = 0; x < 32; x++)
	{
		arrayToPass2[x] = setWestWall;
	}
	//testBlueprint3.DetermineBorderWall(32, arrayToPass2);		// set this value for the west wall

	// make north wall values same as west wall (temporary)
	//testBlueprint3.DetermineBorderWall(16, arrayToPass2);
	//testBlueprint3.SetBorderBlockFlags(16, 16, 25);

	// set south as well
	//testBlueprint3.DetermineBorderWall(4, arrayToPass2);
	*/

	auto carveend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> carveelapsed = carveend - carvestart;
	std::cout << "Elapsed time (Carve time): " << carveelapsed.count() << endl;




	
	//OrganicSystem Organic;
	auto STARTUPbegin = std::chrono::high_resolution_clock::now();
	//1024, 768, 
	 //2560, 1440,
	//OrganicSystem Organic(2, 13, 2560, 1440);
	//OrganicSystem Organic(2, 13, 1024, 768);					// number of factories, buffer size, pixel width, pixel height
	//OrganicSystem Organic(3, 13, 1024, 768);					// number of factories, buffer size, pixel width, pixel height
	//Organic.LoadNWChunks();
	//Organic.SetOrganicCell1(mainthreadpoolref);				// set the Organic instance's first worker thread
	//Organic.SetOrganicCell2(mainthreadpoolref2);			// set the Organic instance's second worker thread

	//OrganicSystem Organic(1, 13, 1024, 768);
	//Organic.AddOrganicCell(mainthreadpoolref);

	//OrganicSystem Organic(2, 13, 1024, 768);
	//Organic.AddOrganicCell(mainthreadpoolref);
	//Organic.AddOrganicCell(mainthreadpoolref2);

	OrganicSystem Organic(3, 3, 13, 1024, 768);					// number of factories, T1 buffer size, T2 buffer size, pixel width, pixel height
	//Organic.AddOrganicCell(mainthreadpoolref);
	//Organic.AddOrganicCell(mainthreadpoolref2);
	//Organic.AddOrganicCell(mainthreadpoolref3);

	//Organic.SetupCellMeta();									// set up metadata about the cells
	//cout << "debug test 1" << endl;
	Organic.AddOrganicTextureMetaArray("base");					// set up the texture map; first ever map will be named "base"
	Organic.AddOrganicVtxColorMetaArray("base");
	//Organic.SetGraphicsAPI();												// setup the graphics API (OpenGL context, etc)
	//auto start3 = std::chrono::high_resolution_clock::now();				// benchmark testing only

																			// send all processed collections to OPEN GL	
	//Organic.SetRenderMode(1);


	//cout << "debug test 2" << endl;
	Organic.SetWorldCoordinates(-3.0f, 30.0f, -1.0f);
	//Organic.SetupWorldArea(-3.0f, 30.0f, -1.0f);
	Organic.InitializeCollectionStateArray(-3.0f, 30.0f, -1.0f);			// initialize state array; (world coords required)

	// cout << "debug test 3" << endl;
	EnclaveManifestFactoryT1* factoryRef;
	factoryRef = &Organic.OrganicFactoryIndex.FactoryMap[0];

	EnclaveCollectionBlueprint* flatPtr = &flatBlueprint;
	EnclaveCollectionBlueprint* testBPPtr = &testBlueprint3;
	EnclaveKeyDef::EnclaveKey newCalibrateKey;
	newCalibrateKey.x = 0;
	newCalibrateKey.y = 0;
	newCalibrateKey.z = 0;
	Organic.JobCalibrateBlueprintBordersFromFactory(newCalibrateKey, flatPtr, factoryRef);			// single blueprint border calibrations
	Organic.JobCalibrateBlueprintBordersFromFactory(newCalibrateKey, testBPPtr, factoryRef);
	// add first 4 keys to render
	
	/*
		(9/4/2017)
		RAM USAGE NOTES:   total number of collections	|	before GL	|	post GL
							8							|	 353		|	 470
							64							|	 488		|	 602
							256							|    954        |    1082
	*/
	EnclaveKeyDef::EnclaveKey flatKeyToAdd;

	//auto collectionsSetupBEGIN = std::chrono::high_resolution_clock::now();
	
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

	flatKeyToAdd.x = -1;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 0;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);		// flatBlueprint

	flatKeyToAdd.x = 0;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = -1;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);


	
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

	//EnclaveKeyDef::EnclaveKey flatKeyToAdd;
	

	flatKeyToAdd.x = 0;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 2;
	//Organic.AddKeyToRenderList(flatKeyToAdd);
	//Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	

	flatKeyToAdd.x = 0;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 2;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	auto collectionsSetupBEGIN = std::chrono::high_resolution_clock::now();
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	
	/* Non-central blueprints (should be tested with release mode only, in 2/3 threads mode) */
	
	

	
	
	for (int x = 2; x < 7; x++)
	{
		for (int z = 0; z >= -7; z--)
		{
			EnclaveKeyDef::EnclaveKey key2;
			key2.x = 6;
			key2.y = 0;
			key2.z = 0;
			Organic.AddKeyToRenderList(key2);
			Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
		}
	}
	


	//flatKeyToAdd.x = 2;
	//flatKeyToAdd.y = 0;
	//flatKeyToAdd.z = -6;
	//Organic.AddKeyToRenderList(flatKeyToAdd);
	//Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);



	//flatKeyToAdd.x = -7;
	//flatKeyToAdd.y = 0;
	//flatKeyToAdd.z = -7;
	//Organic.AddKeyToRenderList(flatKeyToAdd);
	//Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
	
	
	
	for (int x = 1; x >= -6; x--)
	{
		for (int z = -2; z >= -7; z--)
		{
			flatKeyToAdd.x = x;
			flatKeyToAdd.y = 0;
			flatKeyToAdd.z = z;
			Organic.AddKeyToRenderList(flatKeyToAdd);
			Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
		}
	}
	

	
	
	for (int z = -1; z <= 0; z++)
	{
		flatKeyToAdd.x = -6;
		flatKeyToAdd.y = 0;
		flatKeyToAdd.z = z;
		Organic.AddKeyToRenderList(flatKeyToAdd);
		Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
	}
	

	
	// add blueprint for morph testing
	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 0;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
	Organic.AddKeyToRenderList(flatKeyToAdd);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 1;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, testBlueprint3);
	Organic.AddKeyToRenderList(flatKeyToAdd);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 2;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, testBlueprint3);
	Organic.AddKeyToRenderList(flatKeyToAdd);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 3;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, testBlueprint3);
	Organic.AddKeyToRenderList(flatKeyToAdd);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 4;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, testBlueprint3);
	Organic.AddKeyToRenderList(flatKeyToAdd);
	
	// DEBUG TESTING BLUEPRINT IS HERE


	// ----- X AXIS TEST
	flatKeyToAdd.x = 6;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 0;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = 6;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = -1;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	// ----- Y AXIS TEST
	flatKeyToAdd.x = -1;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = -8;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = -1;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 6;
	Organic.AddKeyToRenderList(flatKeyToAdd);
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
	

	/*
	for (int x = 0; x < 5000; x++)
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = x;
		tempKeyToAdd.y = 100;
		tempKeyToAdd.z = 100;
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);
	}
	*/

	/*
	// add blueprint for morph testing
	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 0;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 1;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 2;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 3;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);

	flatKeyToAdd.x = -7;
	flatKeyToAdd.y = 0;
	flatKeyToAdd.z = 4;
	Organic.AddBlueprint(flatKeyToAdd.x, flatKeyToAdd.y, flatKeyToAdd.z, flatBlueprint);
	*/


	
	auto collectionsSetupEND = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> collectionsSetupELAPSED = collectionsSetupEND - collectionsSetupBEGIN;
	std::cout << "Elapsed time (Multiple blueprint addition): " << collectionsSetupELAPSED.count() << endl;
	

	EnclaveKeyDef::EnclaveKey calibrateKey;
	calibrateKey.x = 0;
	calibrateKey.y = 0;
	calibrateKey.z = 0;


	auto calibrateBegin = std::chrono::high_resolution_clock::now();
	//Organic.JobCalibrateBlueprintBordersFromFactory(calibrateKey, factoryRef);
	auto calibrateEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> calibrateELAPSED = calibrateEnd - calibrateBegin;
	std::cout << "Elapsed time (Blueprint calibration): " << calibrateELAPSED.count() << endl;

	/*
	// vertical blueprints
	int height = 7;
	for (int y = 0; y < height; y++)
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = 0;
		tempKeyToAdd.y = y;
		//cout << "debug: " << y << endl;
		tempKeyToAdd.z = 2;
		Organic.AddKeyToRenderList(tempKeyToAdd);
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);

	}


	for (int y = 0; y < height; y++)
	{
		EnclaveKeyDef::EnclaveKey tempKeyToAdd;
		tempKeyToAdd.x = 0;
		tempKeyToAdd.y = y;
		//cout << "debug: " << y << endl;
		tempKeyToAdd.z = -2;
		Organic.AddKeyToRenderList(tempKeyToAdd);
		Organic.AddBlueprint(tempKeyToAdd.x, tempKeyToAdd.y, tempKeyToAdd.z, testBlueprint3);

	}
	*/

	int shiftval = 1;
	shiftval <<= 7;
	//cout << "test bit shift: " << shiftval << endl;

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

	Organic.MaterializeAllCollectionsInRenderList();
	//cout << "TESTING OF ELEMENT VALUE: " << OGLM.OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[currentT2BufferElement].ContainsUsedT2Key
	//Organic.MaterializeAllCollectionsInRenderList(0);			// 0 = use a set of Factories, 1 = use a ManifestMatrix style


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
	std::cout << "debug line 2" << endl;
	//RenderCollections.CreateRenderArrayFromManifestCollection(testkey2);										//change here... OLD: testkey
	auto finish7 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed7 = finish7 - start7;
	//std::cout << "Elapsed time (Creating Render Array test: " << collectcount << "): " << elapsed7.count() << endl;


	//ManifestCollections.GetCollectedEnclaveManifestAt(testkey2, 1, 0, 0);									//change here... OLD: testkey

	//Enclave *tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(69, 0, 0);				// MUST be a pointer, to not get a copy!!
	//tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(0, 0, 0);											//change here... OLD: tempEnclave = &EnclaveCollections.GetEnclaveFromXYZ(77, 0, 0);		
	std::cout << "Before new unveil call... " << endl;
	//tempEnclave->UnveilSinglePolyWithMtrl(3, 2, 0, 0, 1, 0, 40, 0);

	auto start2 = std::chrono::high_resolution_clock::now();
	//ManifestCollections.UpdateAttachedManifest(testkey2, 0, 0, 0);				// problem is after this call	// change here... OLD: testkey
	auto finish2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::cout << "Elapsed time (Creating Render Array test 2: , " << count << "): " << elapsed2.count() << endl;


	//ManifestCollections.GetColletedEnclaveManifestAt(testkey, 1, 0, 0);
	//cout << "preparing to clean all enclaves..." << endl;
	//cin >> val;



	//int choice, in_x, in_y, in_z, in_otherflags;

	// ------------------------------------BEGIN OPEN GL SET UP

	// Initialise GLFW
	//Organic.SetGraphicsAPI();												// setup the graphics API (OpenGL context, etc)
	auto start3 = std::chrono::high_resolution_clock::now();				// benchmark testing only

	// send all processed collections to OPEN GL	
	//Organic.SetRenderMode(1);
	//Organic.SendRenderListToGLTerrainBuffer();							
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

	//int dummy = 1;
	//dummy <<= 0;
	//float dummyfloat = 11.36574;
	//int intnum = 43;

	//float resultfloat = fmod(dummyfloat, 4);
	//int resultfloat2 = intnum / 32;

	//CursorPathTraceContainer containerResult = Organic.EnclaveCollections.GetCursorCoordTrace(37.3);
	//cout << "collection: " << containerResult.CollectionCoord << endl;
	//cout << "chunk: " << containerResult.ChunkCoord << endl;
	//cout << "block: " << containerResult.BlockCoord << endl;
	//cout << "distance from point to nearest positive floor/ceiling: " << containerResult.distance_to_pos << endl;
	//cout << "distance from point to nearest negative floor/ceiling: " << containerResult.distance_to_neg << endl;

	//cout << "float test:" << resultfloat2 << endl;

	// ------------------------------------END OPEN GL SET UP


	// ------------------------------------MAIN WORLD LOOP			NOTE: use cout << fixed for exact timestamp values!
	//Organic.SetRenderMode(0);
	// set up pointers
	auto worldsetupbegin = std::chrono::high_resolution_clock::now();
	//Organic.SetupWorldArea(-3.2f, 30.2f, -1.2f);				// sets the exact point of the camera in the world, and initializes other world area meta data (current collection,  chunk, block, vectors for "picking", etc)
	Organic.SetupWorldArea(-3.0f, 30.0f, -1.0f);				// sets the exact point of the camera in the world, and initializes other world area meta data (current collection,  chunk, block, vectors for "picking", etc)
	
	auto worldsetupend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> worldsetupelapsed = worldsetupend - worldsetupbegin;
	std::cout << "Elapsed time (World set up): " << worldsetupelapsed.count() << endl;

	//glm::vec3 originVec(.4,.3,.3);
	//glm::vec3 directionVec(.8,.4,.4);

	//glm::vec3 originVec(.2, .2, .2);
	//glm::vec3 directionVec(.6, .3, .3);

	glm::vec3 originVec(4.52633, 27.9106, 7.51705);

	//glm::vec3 originVec(.4, .3, .3);
	glm::vec3 directionVec(-0.00196488, -.264983, -0.964251);

	glm::vec3* originVecPtr = &originVec;
	glm::vec3* directionVecPtr = &directionVec;
	std::cout << "-----------------------------------cursor target testing..." << endl;
	//Organic.DetermineMouseCursorTargets2(originVecPtr, directionVecPtr, 10);	// Step 3: determine targetable blocks 

	std::cout << "----SMART POINTER TESTING----" << endl;
	auto targetsbegin = std::chrono::high_resolution_clock::now();
	//Organic.DetermineMouseCursorTargets2(originVecPtr, directionVecPtr, 10);
	//unique_ptr<GLfloat[]> testptr = make_unique<GLfloat[]>(1000);
	//unique_ptr<GLfloat[]> testptr(new GLfloat[1000]);
	unique_ptr<GLfloat[]> testptr(nullptr);
	//testptr(new GLfloat[1000]);
	testptr.reset(new GLfloat[1000]);

	auto targetsend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> targetselapsed = targetsend - targetsbegin;
	std::cout << "Smart pointer time: " << targetselapsed.count() << ", testvalue is: " << testptr[0] <<  endl;



	auto targetsbegin2 = std::chrono::high_resolution_clock::now();
	GLfloat* testptr2 = new GLfloat[1000];
	auto targetsend2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> targetselapsed2 = targetsend2 - targetsbegin2;
	std::cout << "Dynamic array pointer time: " << targetselapsed2.count() << ", testvalue is: " << testptr2[0] << endl;
	delete[] testptr2;

	//std::cout << "Elapsed time (Terrain targeting): " << targetselapsed.count() << endl;

	std::cout << "-----------------------collection list: " << endl;
	//Organic.ListEnclaveCollectionsInMatrix();

	Organic.setPhaseinDCM();

	do {
		auto start3 = std::chrono::high_resolution_clock::now();
		// |||||||||||||||||||||| PHASE 1 operations: work division
		Organic.CheckForMorphing();			// determine if there were any OGLM buffer shifts that needed to be done.
		Organic.CheckForT1CollectionPrep();	// checks to see if any T1 collections need to be prepared
		Organic.DivideTickWork();			// split work for the tick here

		// |||||||||||||||||||||| PHASE 2 operations: run terrain jobs in queues

		Organic.CheckProcessingQueue();		// check for work in collection processing queue(s)
		Organic.WaitForPhase2Promises();
		// |||||||||||||||||||||| PHASE 3: Render terrain and targeted blocks

		
		Organic.RenderGLTerrain();		
		// Step 1 (?): perform render frame work	// Step 2: check if camera has moved to another collection; if so shift the OrganicCollectionPointerMatrix(?)
		Organic.DetermineMouseCursorTargets2(Organic.OGLM.positionVecPtr, Organic.OGLM.directionVecPtr, 14);	// Step 3: determine targetable blocks 

		auto finish3 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed3 = finish3 - start3;
		// std::cout << "Frame render time (without draw call) : " << elapsed3.count() << endl;
	} 
	while (glfwGetKey(Organic.OGLM.GLwindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&	// loop until escape key is pressed in OpenGL window
		glfwWindowShouldClose(Organic.OGLM.GLwindow) == 0);
	// -------------------------------------END MAIN WORLD LOOP

	Organic.GLCleanup();			// cleanup OpenGL window, buffers, arrays, etc

	std::cout << "Press any number key to end.";
	int somevalue;
	std::cin >> somevalue;


	
    return 0;
}


