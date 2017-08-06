/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 7/14/2017)

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
//#include "thread_pool.h"

class thread_pool;
class OrganicSystem {

public:
	EnclaveCollectionBlueprintMatrix BlueprintMatrix;							// stores blueprints that will be used to characterize/create collections of enclaves
	EnclaveCollectionMatrix EnclaveCollections;									// matrix of EnclaveCollections (512 chunks, dynamically allocated array)
	ManifestCollectionMatrix ManifestCollections;								// matrix of ManifestCollections (up to 512 chunks, unordered map)
	RenderCollectionMatrix RenderCollections;									// matrix of RenderCollections
	OrganicTextureDictionary TextureDictionary;
	thread_pool *testpool2;
	thread_pool *testpool3;
	int flermp = 0;
	OrganicSystem();															// default constructor
	void InterlockBaseCollections();											// connects the 3 base collections together -- EnclaveCollections, ManifestCollections, RenderCollections.
	void AddAndMaterializeCollection(int x, int y, int z);						// adds a new collection, and renders all top faces in all 64 top level chunks 
	void ChangeSingleBlockMaterialAtXYZ(int x, int y, int z, int newmaterial);					// changes the material block at an x/y/z location
	void AddBlueprint(int x, int y, int z, EnclaveCollectionBlueprint blueprint);
	void SetOrganicPool(thread_pool *thread_pool_ref);
	void SetOrganicPool2(thread_pool *thread_pool_ref);
	void AddOrganicTextureMetaArray(string mapname);
	thread_pool* getpool();
	thread_pool* getpool2();
};

#endif