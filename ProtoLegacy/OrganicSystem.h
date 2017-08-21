/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 8/14/2017)

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
#include "MDJobMaterializeCollection.h"
#include "MDListJobMaterializeCollection.h"
#include "EnclaveManifestFactoryT1.h"
#include "OrganicGLManager.h"
#include <GL/glew.h>
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
	EnclaveCollectionBlueprintMatrix BlueprintMatrix;							// stores blueprints that will be used to characterize/create collections of enclaves
	EnclaveCollectionMatrix EnclaveCollections;									// matrix of EnclaveCollections (512 chunks, dynamically allocated array)
	ManifestCollectionMatrix ManifestCollections;								// matrix of ManifestCollections (up to 512 chunks, unordered map)
	RenderCollectionMatrix RenderCollections;									// matrix of RenderCollections
	OrganicTextureDictionary TextureDictionary;									// an instance for an OrganicTextureDictionary
	OrganicGLManager OGLM;														// an instance of the OrganicGLManager; will be used to manage OpenGL buffer data and other similiar operations
	thread_pool *Cell1;															// pointer for Cell 1
	thread_pool *Cell2;															// pointer for Cell 2

	OrganicSystem();																			// default constructor


	void InterlockBaseCollections();															// connects the 3 base collections together -- EnclaveCollections, ManifestCollections, RenderCollections.
	void AddAndMaterializeSingleCollectionMM(int x, int y, int z);								/* adds a single new MM-based collection, and renders all top faces in all 64 top level chunks;
																								 this task will run on the same thread on which it is called (will not utilize thread pool)*/
	void MaterializeCollection(EnclaveKeyDef::EnclaveKey Key1, EnclaveKeyDef::EnclaveKey Key2);	// temporary test function only; attempts to run all types of collection materialization jobs
	void ChangeSingleBlockMaterialAtXYZ(int x, int y, int z, int newmaterial);					// changes the material block at an x/y/z location
	void AddBlueprint(int x, int y, int z, EnclaveCollectionBlueprint blueprint);				// adds a blueprint to the OrganicSystem's blueprint matrix.
	void SetupFutureCollectionMM(int x, int y, int z);											// sets up all future data structures that will be needed for an EnclaveCollection that is produced using a ManifestMatrix
	void SetOrganicCell1(thread_pool *thread_pool_ref);											// sets the pointer for Cell1 to be a valid worker thread
	void SetOrganicCell2(thread_pool *thread_pool_ref);											// sets the pointer for Cell2 to be a valid worker thread
	void AddOrganicTextureMetaArray(string mapname);											// adds a new texture meta array, which is a list that is used to map block IDs to texture UV coordinates.
	void SetGraphicsAPI();
	void JobMaterializeSingleCollectionFromMM(	EnclaveKeyDef::EnclaveKey Key1,																									// materializes a single collection from the ground up, utilizing a manifest matrix
												EnclaveCollectionBlueprintMatrix BlueprintMatrixRef, 
												EnclaveCollectionMatrix EnclaveCollectionsRef, 
												ManifestCollectionMatrix ManifestCollectionsRef, 
												RenderCollectionMatrix RenderCollectionsRef, 
												EnclaveCollection *CollectionRef,
												ManifestCollection *ManifestCollectionRef
												);
	void JobMaterializeMultiCollectionFromMM(MDListJobMaterializeCollection mdjob, mutex& mutexval, int ThreadID);																// materializes multiple collections from the ground up, utilizing a manifest matrix.
	void JobMaterializeMultiCollectionFromFactory(MDListJobMaterializeCollection mdjob, mutex& mutexval, EnclaveManifestFactoryT1 *FactoryRef, int ThreadID);					// materializes multiple collections from the ground up, utilizing a factory.
	void JobRematerializeSingleExistingCollectionFromFactory(	EnclaveKeyDef::EnclaveKey Key1,																					// rematerializes a single collection on a currently loaded EnclaveCollection, from a Factory
																EnclaveCollection *CollectionRef, 
																EnclaveManifestFactoryT1 *FactoryRef, 
																RenderCollectionMatrix *RenderCollectionRef, 
																mutex& mutexval);								
	void DummyJob(int value, EnclaveManifestFactoryT1 *FactoryRef, mutex& mutexval);																							// dummy thread pool job, for testing only.
	void RenderGLTerrain();																																					
	void GLCleanup();
	thread_pool* getCell1();																																					// gets a pointer to worker thread (Cell) #1
	thread_pool* getCell2();																																					// gets a pointer to worker thread (Cell) #2

};

#endif