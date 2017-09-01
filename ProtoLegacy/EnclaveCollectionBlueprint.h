/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 8/16/2017)

Description: Header file for EnclaveCollectionBlueprint.cpp

Summary: An EnclaveCollectionBlueprint will store metadata that will tell the Organic system how to "shape" a collection of enclaves. It will include information on which chunks in the collection are solid, which solids are "exposed to the surface (air)",
and has the ability to store EnclavePainters, which have the ability to change the block id of a series of blocks in a chunk.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBLUEPRINT_H
#define ENCLAVECOLLECTIONBLUEPRINT_H
#include "EnclavePainterListMatrix.h"
#include "ECBXAxisCarvePlan.h"
#include "EnclaveUnveilMeta.h"
#include <vector>


class EnclaveCollectionBlueprint {

public:
	int CarveMode = 1;											// determines the current carve mode; default is 1
	typedef unsigned char(&ElevationMapRef)[8][8];				// set up type def for rest of header file
	unsigned char SurfaceChunks[8][8];							// set up the 64 byte array for surface chunks (any chunk exposed to air)
	unsigned char SolidChunks[8][8];							// set up the 64 byte array for non-air chunks
	unsigned char CustomPaintableChunks[8][8];						// stores a list of custom chunks to be painted
	unsigned char StandardPaintableChunks[8][8];
	EnclavePainterListMatrix PaintListMatrix;					// stores the actual paint jobs that will be run
	std::vector<ECBXAxisCarvePlan> XAxisCPVector;				// vector for storing x axis carve plans
	int RenderWestBorder = 1;										// flags for determining if any chunks on this border side will be rendered; default is 1
	int RenderNorthBorder = 1;
	int RenderEastBorder = 1;
	int RenderSouthBorder = 1;
	int RenderTopBorder = 1;
	int RenderBottomBorder = 1;

	void SetSurfaceChunkData(ElevationMapRef surfaceData);		// will load the data for the surface chunk array
	void SetSolidChunkData(ElevationMapRef solidData);			// will load the data for the solid chunk array
	void SetCustomPaintableChunkData(ElevationMapRef paintableData);	
	void AddNewPaintList(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList);
	void AddNewPaintList2(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList);
	int BPKeyToSingle(EnclaveKeyDef::EnclaveKey tempKey);
	int BlockKeyToSingle(int in_x, int in_y, int in_z);
	ElevationMapRef& GetSurfaceChunkData();
	ElevationMapRef& GetSolidChunkData();
	ElevationMapRef& GetCustomPaintableChunkData();
	ElevationMapRef& GetStandardPaintableChunkData();
	void CarveSlope();
	EnclaveUnveilMeta SetupCarvePlan(EnclaveKeyDef::EnclaveKey tempKey);

};

#endif