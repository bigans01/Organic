/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 7/17/2017)

Description: Header file for EnclaveCollectionBlueprint.cpp

Summary: An EnclaveCollectionBlueprint will store metadata that will tell the Organic system how to "shape" a collection of enclaves. It will include information on which chunks in the collection are solid, which solids are "exposed to the surface (air)",
and has the ability to store EnclavePainters, which have the ability to change the block id of a series of blocks in a chunk.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBLUEPRINT_H
#define ENCLAVECOLLECTIONBLUEPRINT_H
#include "EnclavePainterListMatrix.h"

class EnclaveCollectionBlueprint {

public:
	typedef unsigned char(&ElevationMapRef)[8][8];				// set up type def for rest of header file
	unsigned char SurfaceChunks[8][8];							// set up the 64 byte array for surface chunks (any chunk exposed to air)
	unsigned char SolidChunks[8][8];							// set up the 64 byte array for non-air chunks
	unsigned char PaintableChunks[8][8];						// stores a list of chunks to be painted
	EnclavePainterListMatrix PaintListMatrix;					// stores the actual paint jobs that will be run
	void SetSurfaceChunkData(ElevationMapRef surfaceData);	// will load the data for the surface chunk array
	void SetSolidChunkData(ElevationMapRef solidData);		// will load the data for the solid chunk array
	void SetPaintableChunkData(ElevationMapRef paintableData);
	void AddNewPaintList(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList);
	ElevationMapRef& GetSurfaceChunkData();
	ElevationMapRef& GetSolidChunkData();
	ElevationMapRef& GetPaintableChunkData();

};

#endif