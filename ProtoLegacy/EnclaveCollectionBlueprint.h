/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 9/15/2017)

Description: Header file for EnclaveCollectionBlueprint.cpp

Summary: An EnclaveCollectionBlueprint will store metadata that will tell the Organic system how to "shape" a collection of enclaves. It will include information on which chunks in the collection are solid, which solids are "exposed to the surface (air)",
and has the ability to store EnclavePainters, which have the ability to change the block id of a series of blocks in a chunk.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBLUEPRINT_H
#define ENCLAVECOLLECTIONBLUEPRINT_H

#include "EnclavePainterListMatrix.h"
#include "ECBXAxisCarvePlan.h"
#include "ECBCollectionPainter.h"
#include "ECBOutwardFaceFlags.h"
#include "EnclaveUnveilMeta.h"
#include <iostream>
#include <string>
#include <vector>

//class EnclaveCollection;
//class OrganicSystem;
//class Enclave;
//class EnclaveCollectionMatrix;
//class EnclaveCollectionBlueprintMatrix;
class EnclaveCollectionBlueprint {

public:
	void FlattenToElevation();																	// Blueprint modelling: "flattens" the blueprint, so that it is aligned to be flat horizontally. (i.e., on the x and z axis)
	void CarveSlope();																			// Blueprint modelling: sets a carved slope.
private:
	friend class EnclaveCollection;
	friend class OrganicSystem;
	friend class Enclave;
	friend class EnclaveCollectionMatrix;
	friend class EnclaveCollectionBlueprintMatrix;
	int CarveMode = 1;											// determines the current carve mode; default is 1
	typedef unsigned char(&ElevationMapRef)[8][8];				// set up type def for rest of header file
	unsigned char SurfaceChunks[8][8] = { 0 } ;							// set up the 64 byte array for surface chunks (any chunk exposed to air)
	unsigned char SolidChunks[8][8] = { 0 } ;							// set up the 64 byte array for non-air chunks
	unsigned char AirtightChunks[8][8] = { 0 };							// stores a list of all airtight chunks (airtight meaning no air blocks exist in the chunk)
	unsigned char CustomPaintableChunks[8][8] = { 0 };					// stores a list of custom chunks to be painted
	unsigned char StandardPaintableChunks[8][8] = { 0 };				// stores a list of standard chunks ot be painted (for basic painting operations where entire sides of an Enclave will be painted)
	ECBOutwardFaceFlags WestBorderBlocks;						// stores bitwise flags for the west border blocks
	ECBOutwardFaceFlags NorthBorderBlocks;						// stores bitwise flags for the north border blocks
	ECBOutwardFaceFlags EastBorderBlocks;						// stores bitwise flags for the east border blocks
	ECBOutwardFaceFlags SouthBorderBlocks;						// stores bitwise flags for the south border blocks
	ECBOutwardFaceFlags TopBorderBlocks;						// stores bitwise flags for the top border blocks
	ECBOutwardFaceFlags BottomBorderBlocks;						// stores bitwise flags for the bottom border blocks
	EnclavePainterListMatrix PaintListMatrix;					// stores the actual paint jobs that will be run
	std::vector<ECBXAxisCarvePlan> XAxisCPVector;				// vector for storing x axis carve plans
	std::unordered_map<std::string, ECBCollectionPainter> CollectionPainterMap;
	int CurrentCollectionPainterMapIndex = 0;
	int RenderWestBorder = 1;									// flags for determining if any chunks on this border side will be rendered; default is 1	(1 means there is no border)
	int RenderNorthBorder = 1;
	int RenderEastBorder = 1;
	int RenderSouthBorder = 1;
	int RenderTopBorder = 1;
	int RenderBottomBorder = 1;

	void SetSurfaceChunkData(ElevationMapRef surfaceData);										// will load the data for the surface chunk array
	void SetSolidChunkData(ElevationMapRef solidData);											// will load the data for the solid chunk array
	void SetCustomPaintableChunkData(ElevationMapRef paintableData);							// will load the data for the custom chunk array
	void AddNewPaintList(EnclaveKeyDef::EnclaveKey InKey, EnclavePainterList InPaintList);		// adds a new paint list to this blueprint 								
	int BlockKeyToSingle(int in_x, int in_y, int in_z);											// transforms an x/y/z block key to its associated single value
	ElevationMapRef& GetSurfaceChunkData();														// returns a reference to the surface chunk array
	ElevationMapRef& GetSolidChunkData();														// returns a reference to the solid chunk array
	ElevationMapRef& GetCustomPaintableChunkData();												// returns a reference to the custom chunk array
	ElevationMapRef& GetStandardPaintableChunkData();											// returns a reference to the standard chunk array
	EnclaveUnveilMeta SetupCarvePlan(EnclaveKeyDef::EnclaveKey tempKey);						// Blueprint modelling: sets up a 32 x 32 array that an Enclave may reference to find the y values to set itself at; this will be continually changed
	EnclaveUnveilMeta ReturnBorderChunkFacesToRender(int x,		// used when there is a neighboring blueprint. It compares blueprint A to the border wall in blueprint B, to determine what to render.
													 int y, 
													 int z, 
													 EnclaveCollectionBlueprint* originBlueprint, 
													 EnclaveCollectionBlueprint* comparedBlueprint, 
													 int directionOfNeighbor);
	void CalibrateBlueprintBorders(EnclaveCollection* collectionPtr);
	void SetBorderBlockFlags(int direction, int slice, int slice_offset);		// sets the border block flags, which are integers where each bit represents a single block on either the x, y, or z axis. 
	void DetermineBorderWall(int direction, int valuearray[32]);				// sets up the border wall for any given direction.

};

#endif