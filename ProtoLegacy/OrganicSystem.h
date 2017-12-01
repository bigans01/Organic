/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 10/10/2017)

Description: Header file for OrganicSystem.cpp

Summary: An OrganicSystem is an object that has several base Collections (EnclaveCollections, ManifestCollections, RenderCollections) interconnected together, and serves as the core object on which higher level commands 
-- such as adding a new collection of enclaves and rendering them -- will be run. Various functions are provided to facilitate quick and easy manipulation of Enclaves, EnclaveManifests, and other associated objects. The 
OrganicSystem object contains all objects necessary to preserve information on the 3d data, at a minimum, with functionality for optional collections such as Textures and other features to be included at a later date.



------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICSYSTEM_H
#define ORGANICSYSTEM_H

#include "ManifestCollectionMatrix.h"
#include "EnclaveCollectionMatrix.h"
#include "RenderCollectionMatrix.h"
#include "EnclaveCollectionBlueprint.h"
#include "EnclaveCollectionBlueprintMatrix.h"
#include "OrganicTextureDictionary.h"
#include "OrganicVtxColorDictionary.h"
#include "OrganicCellList.h"
#include "OrganicCellManager.h"
#include "OrganicCellLimits.h"
#include "OrganicCellLimitsList.h"
#include "OrganicMorphMeta.h"
#include "MDJobMaterializeCollection.h"
#include "MDListJobMaterializeCollection.h"
#include "EnclaveManifestFactoryT1.h"
#include "OrganicGLManager.h"
#include "EnclaveKeyDef.h"
#include "EnclaveManifestFactoryT1Index.h"
#include "EnclaveKeyContainer.h"
#include "CursorPathTraceContainer.h"
#include "MaterializeCollectionListContainer.h"
#include "EnclaveBlockRayTracker.h"
#include "TestList.h"
#include "EnclaveCollectionStateArray.h"
#include <GL/glew.h>
#include <queue>
#include <chrono>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//#include "shader.hpp"
#include "common/shader.hpp"
#include <mutex>

class thread_pool;
class OrganicSystem {

public:
	//friend class OGLMBufferManager;
	EnclaveCollectionBlueprintMatrix BlueprintMatrix;							// stores blueprints that will be used to characterize/create collections of enclaves
	EnclaveCollectionMatrix EnclaveCollections;									// matrix of EnclaveCollections (512 chunks, dynamically allocated array)
	ManifestCollectionMatrix ManifestCollections;								// matrix of ManifestCollections (up to 512 chunks, unordered map)
	RenderCollectionMatrix RenderCollections;									// matrix of RenderCollections
	OrganicTextureDictionary TextureDictionary;									// an instance for an OrganicTextureDictionary
	OrganicVtxColorDictionary VertexColorDictionary;							// an instance for an OrganicVtxColorDictionary
	OrganicBlockTarget blockTargetMeta;											// meta data about the currently targeted block
	EnclaveManifestFactoryT1Index OrganicFactoryIndex;							// FactoryIndex for this OrganicSystem
	EnclaveKeyContainer renderCollectionList;									// contains a a list of Keys that determine which EnclaveCollections to render and/or process	
	MaterializeCollectionListContainer MatCollList;
	EnclaveCollectionStateArray CollectionStateArray;
	std::mutex heapmutex;														// global heap mutexval; used by any thread when it must alter memory (vectors, using new/delete[], etc)
	OrganicGLManager OGLM;														// an instance of the OrganicGLManager; will be used to manage OpenGL buffer data and other similiar operations
	OrganicSystem(int numberOfFactories, int T1_bufferCubeSize, int T2_bufferCubeSize, int windowWidth, int windowHeight);					// default constructor: number of factories, plus the size of the buffer cube
	~OrganicSystem();

	void DetermineMouseCursorTargets(glm::vec3* originVector, glm::vec3* directionVector, int length);
	void DetermineMouseCursorTargets2(glm::vec3* originVector, glm::vec3* directionVector, int length);
	void JobCalibrateBlueprintBordersFromFactory(EnclaveKeyDef::EnclaveKey Key1, EnclaveManifestFactoryT1 *FactoryRef);															// for a blueprint in the blueprint matrix
	void JobCalibrateBlueprintBordersFromFactory(EnclaveKeyDef::EnclaveKey Key1, EnclaveCollectionBlueprint* inBlueprintPtr, EnclaveManifestFactoryT1 *FactoryRef);				// for a stand-alone blueprint that hasn't been added to a matrix
	void MaterializeCollection(EnclaveKeyDef::EnclaveKey Key1, EnclaveKeyDef::EnclaveKey Key2);	// temporary test function only; attempts to run all types of collection materialization jobs
	void MaterializeAllCollectionsInRenderList(int renderProcess);								// will attempt to materialize all collections in renderCollectionList; 0 = Factory mode, 1 = MM mode
	void MaterializeAllCollectionsInRenderList();												// will attempt to materialize all collections; will divide up work between Collections that need to be processed via MM and Factory modes
	void ChangeSingleBlockMaterialAtXYZ(int x, int y, int z, int newmaterial);					// changes the material block at an x/y/z location
	void AddBlueprint(int x, int y, int z, EnclaveCollectionBlueprint blueprint);				// adds a blueprint to the OrganicSystem's blueprint matrix.
	void AddKeyToRenderList(EnclaveKeyDef::EnclaveKey tempKey);									// adds 1 key to the renderCollectionList.
	void SetupFutureCollectionMM(int x, int y, int z);											// sets up all future data structures that will be needed for an EnclaveCollection that is produced using a ManifestMatrix
	void SetupFutureCollectionMM(EnclaveKeyDef::EnclaveKey tempKey);							// does the same as SetupFuturecollectionMM, but uses an EnclaveKey as input.
	void AddOrganicTextureMetaArray(string mapname);											// adds a new texture meta array, which is a list that is used to map block IDs to texture UV coordinates.
	void AddOrganicVtxColorMetaArray(string mapname);											// adds a new vertex color meta array, which is a list that is used to color individual vertexes.
	void SetGraphicsAPI();																		// sets up initial data for OpenGL functionality						
	RenderCollection* GetRenderCollectionPtr(int x, int y, int z);								// returns a pointer to a RenderCollection
	void SendDataFromRenderPtrToGLBuffer(RenderCollection* renderCollectionPtr);				// sends all vertex data from the RenderCollection to the OGLM's OpenGL vertex data buffer (data buffer name: OrganicGLVertexBufferID)
	void LoadVCDataToGLBuffer(RenderCollection* renderCollectionPtr);							// sends all vertex color data from the RenderCollection to the OGLM's OpenGL vertex color data buffer (data buffer name: OrganicGLVertexColorBufferID)
	void AnalyzeRenderArray(int x, int y, int z, int xyz);										// reserved for future use.
	void SetRenderMode(int x);																																					// sets the RenderMode variable in the OGLM object
	void RenderGLTerrain();																																						// renders everything in the Terrain buffer
	void GLCleanup();																																							// for deallocating and/or turning off OpenGL components
	void SendRenderListToGLTerrainBuffer();																																		// will send all renderable enclaves listed in the renderCollectionList to the OpenGL buffer																																				// currently for testing: "moves the world" by preparing buffers to load data for RenderCollections +1 chunk NW of current camera position
	void SetupWorldArea(float x, float y, float z);
	void SetWorldCameraPosition(float x, float y, float z);
	void CheckForMorphing();																																					// checks to see if there is any buffer morphing to be done
	void CheckProcessingQueue();																																				// checks to see if there are any collections to process
	void SetWorldCoordinates(float x, float y, float z);
	void DivideTickWork();																																						// determines how to divide the work among worker threads for this game tick
	void WaitForPhase2Promises();																																				// waits for promises to finish in phase 2

private:

	thread_pool* organicThreadIndex[16];										// contains an array of up to 16 thread pool pointers
	short threadIndexAmount;													// contains the amount of thread pointers in organicThreadIndex that actually point to valid threads
	short isThreadIndexInitialized = 0;											// determines whether or not the pointers in organicThreadIndex have valid thread_pool references
	friend class OGLMBufferManager;												// friend declaration for OGLMBufferManager
	friend class OrganicCellLimits;												// ""
	friend class OrganicCellLimitsList;											// ""
	friend class EnclaveCollectionMatrix;										// ""
	friend class OrganicCellManager;											// ""
	EnclaveKeyDef::EnclaveKey PreviousCCKey;									// will store the previous Camera Collection key from the previous frame here
	EnclaveKeyDef::EnclaveKey CameraCollectionKey;								// the current collection that the camera is in
	EnclaveKeyDef::EnclaveKey CameraChunkKey;									// the curent chunk of the collection the camera is in
	EnclaveKeyDef::EnclaveKey CameraBlockKey;									// the current block in the chunk the camera is in
	OrganicCellList OCList;														// the OrganicSystem's instance of OrganicCellList
	OrganicCellManager OCManager;												// an instance of OCManager that is responsible for determining what things to work on for OrganicCells
	OrganicCellLimitsList OCLimitsList;											// as list of OrganicCellLimits
	std::queue<OrganicMorphMeta> T1CollectionProcessingQueue;					// a queue that stores Type 1 collection keys that need to be processed
	std::queue<OrganicMorphMeta> T2CollectionProcessingQueue;					// a queue that stores Type 2 collection keys that need to be processed
	std::vector<std::future<void>> FL_T1CollectionsProcessed;					// a vector of futures for any processed (completed) T1 collections
	std::vector<std::future<void>> FL_T2CollectionsProcessed;					// a vector of futures for any processed (completed) T2 collections
	std::vector<MDJobMaterializeCollection> OrganicMDJobVectorT1;				// contains a list of T1 materialize collection jobs
	std::vector<MDJobMaterializeCollection> OrganicMDJobVectorT2;				// contains a list of T2 materialize collection jobs
	std::queue<OrganicMorphMeta> OrganicMorphMetaToSendToBuffer;				// stores a list of OrganicMorphMeta that are ready to send to buffer
	std::chrono::high_resolution_clock::time_point phase2begin, phase2end;		// used to determine amount of time for Phase 2 run completion time
	int workPriority = 0;														// work priority mode for per-tick splitting up of jobs for OrganicCells
	int oldWorkPriority = 0;													// the last priority mode that was set


	void SetupCellMeta();														// cycles through each cell and adds the appropriate factory pointer to each
	int CreateThreads(int in_numberOfThreads);
	void AllocateFactories(int noOfFactories);													// sets up Factories for future use. 
	void AddOrganicCell(thread_pool* thread_pool_ref);
	void InterlockBaseCollections();															// connects the 3 base collections together -- EnclaveCollections, ManifestCollections, RenderCollections.
	void MaterializeRenderablesByMM();						// this function will attempt to render all RenderCollections having keys found in this list, regardless of their status, by using a ManifestMatrix (ManifestCollectionMatrix)
	void MaterializeRenderablesByFactory();					// this function  will attempt to render all RenderCollections having keys found in this list, regardless of their status, by using one or more Factories. (EnclaveManifestFactoryT1)
	void AddAndMaterializeSingleCollectionMM(int x, int y, int z);								/* adds a single new MM-based collection, and renders all top faces in all 64 top level chunks;
																								this task will run on the same thread on which it is called (will not utilize thread pool)*/

	// Job declarations begin here
	void JobMaterializeMultiCollectionFromMM(MDListJobMaterializeCollection* mdjob, mutex& mutexval, int ThreadID);																// materializes multiple collections from the ground up, utilizing a manifest matrix.
	void JobMaterializeMultiCollectionFromFactory(MDListJobMaterializeCollection mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID);					// materializes multiple collections from the ground up, utilizing a factory.
	void JobMaterializeMultiCollectionFromFactory2(MDListJobMaterializeCollection* mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID);					// materializes multiple collections from the ground up, utilizing a factory. (testing only, may be erased)
	void JobMaterializeCollectionFromFactoryViaMorph(MDJobMaterializeCollection* mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef);
	void JobMaterializeCollectionFromMM(MDJobMaterializeCollection* mdjob, mutex& mutexval);
	void JobRematerializeSingleExistingCollectionFromFactory(EnclaveKeyDef::EnclaveKey Key1,																					// rematerializes a single collection on a currently loaded EnclaveCollection, from a Factory
		EnclaveCollection *CollectionRef,
		EnclaveManifestFactoryT1 *FactoryRef,
		RenderCollectionMatrix *RenderCollectionRef,
		mutex& mutexval);
	void DummyJob(int value, EnclaveManifestFactoryT1 *FactoryRef, mutex& mutexval);																							// dummy thread pool job, for testing only.
	void JobMaterializeSingleCollectionFromMM(EnclaveKeyDef::EnclaveKey Key1,																									// materializes a single collection from the ground up, utilizing a manifest matrix
		EnclaveCollectionBlueprintMatrix BlueprintMatrixRef,
		EnclaveCollectionMatrix EnclaveCollectionsRef,
		ManifestCollectionMatrix ManifestCollectionsRef,
		RenderCollectionMatrix RenderCollectionsRef,
		EnclaveCollection *CollectionRef,
		ManifestCollection *ManifestCollectionRef
	);

	void SubmitT1TerrainJob(OrganicMorphMeta in_popKey, std::map<int, OrganicCell*>::iterator in_iterator, MDJobMaterializeCollection* in_tempMDJobRef);						// RAII enforced function for submitting a T! terrain job
	void SubmitT2TerrainJob(OrganicMorphMeta in_popKey, std::map<int, OrganicCell*>::iterator in_iterator, MDJobMaterializeCollection* in_tempMDJobRef);
	void PopOrganicMorphMetaQueue();

};

#endif