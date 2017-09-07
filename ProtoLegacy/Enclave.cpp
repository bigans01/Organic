/*------------------------------------------------------------------------------------------

--Enclave.cpp		(Last Update: 7/5/2017)

Description: Enclave.cpp contains the definitions for the declarations found in Enclave.h.

------------------------------------------------------------------------------------------*/

#pragma once


using namespace std;

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "Enclave.h"
#include "EnclaveCollection.h"

class EnclaveCollection;
typedef unsigned char(&ElevationMapRef)[8][8];
Enclave::Enclave(void)
{
	/* Summary: unused, potentially used for later. */
}

Enclave::Enclave(EnclaveKeyDef::EnclaveKey collectkey, int x, int y, int z)
{
	/* Summary: constructor called when Enclave is to be added into EnclaveCollectionMatrix */
	this->CollectionKey.x = collectkey.x;
	this->CollectionKey.y = collectkey.y;
	this->CollectionKey.z = collectkey.z;
	this->UniqueKey.x = x;
	this->UniqueKey.y = y;
	this->UniqueKey.z = z;
}


int Enclave::testsize()
{
	return sizeof(Enclave::EnclavePolyArray);
}

void Enclave::InitializeRenderArray()
{
	/* Summary: function called when enclave's arrays need to be initialized; this should be called 
	immediately after the enclave is created on the heap, and before any other operations occur. */
	int index = 0, i, j, k;			// declare variables for following loops
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				//cout << "test initialize: " << endl;
				Sorted.PolyArrayIndex[index] = index;			// for storing the true value -- between 1-64 (the individual element number as it exists in the array)? (sp?)
				Sorted.RenderArray[index] = &StorageArray[i][j][k]; // point the pointer to the appropriate elements

				Sorted.RenderArray[index]->otherflags = 0;
				Sorted.RenderArray[index]->blockid = 0;
				Sorted.RenderArray[index]->structarray[0].x = 0;		// vertex 0
				Sorted.RenderArray[index]->structarray[0].y = 2;
				Sorted.RenderArray[index]->structarray[0].z = -2;

				Sorted.RenderArray[index]->structarray[1].x = 0;		// vertex 1
				Sorted.RenderArray[index]->structarray[1].y = 2;
				Sorted.RenderArray[index]->structarray[1].z = 0;

				Sorted.RenderArray[index]->structarray[2].x = 0;		// vertex 2
				Sorted.RenderArray[index]->structarray[2].y = 0;
				Sorted.RenderArray[index]->structarray[2].z = -2;

				Sorted.RenderArray[index]->structarray[3].x = 0;		// vertex 3 (true 0)
				Sorted.RenderArray[index]->structarray[3].y = 0;
				Sorted.RenderArray[index]->structarray[3].z = 0;

				Sorted.RenderArray[index]->structarray[4].x = 2;		// vertex 4
				Sorted.RenderArray[index]->structarray[4].y = 2;
				Sorted.RenderArray[index]->structarray[4].z = -2;

				Sorted.RenderArray[index]->structarray[5].x = 2;		// vertex 5
				Sorted.RenderArray[index]->structarray[5].y = 2;
				Sorted.RenderArray[index]->structarray[5].z = 0;

				Sorted.RenderArray[index]->structarray[6].x = 2;		// vertex 6
				Sorted.RenderArray[index]->structarray[6].y = 0;
				Sorted.RenderArray[index]->structarray[6].z = -2;

				Sorted.RenderArray[index]->structarray[7].x = 2;		// vertex 7
				Sorted.RenderArray[index]->structarray[7].y = 0;
				Sorted.RenderArray[index]->structarray[7].z = 0;


				index++;
			}
		}
	}
}

void Enclave::InitializeRenderArray(int blockid)
{
	/* Summary: sets up an enclave's meta data, and sets the material of all blocks to be the input parameter "blockid" */
	int index = 0, i, j, k;			// declare variables for following loops
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				//cout << "test initialize: " << endl;
				Sorted.PolyArrayIndex[index] = index;			// for storing the true value -- between 1-64 (the individual element number as it exists in the array)? (sp?)
				Sorted.RenderArray[index] = &StorageArray[i][j][k]; // point the pointer to the appropriate elements

				Sorted.RenderArray[index]->otherflags = 0;
				Sorted.RenderArray[index]->blockid = blockid;
				Sorted.RenderArray[index]->structarray[0].x = 0;		// vertex 0
				Sorted.RenderArray[index]->structarray[0].y = 2;
				Sorted.RenderArray[index]->structarray[0].z = -2;

				Sorted.RenderArray[index]->structarray[1].x = 0;		// vertex 1
				Sorted.RenderArray[index]->structarray[1].y = 2;
				Sorted.RenderArray[index]->structarray[1].z = 0;

				Sorted.RenderArray[index]->structarray[2].x = 0;		// vertex 2
				Sorted.RenderArray[index]->structarray[2].y = 0;
				Sorted.RenderArray[index]->structarray[2].z = -2;

				Sorted.RenderArray[index]->structarray[3].x = 0;		// vertex 3 (true 0)
				Sorted.RenderArray[index]->structarray[3].y = 0;
				Sorted.RenderArray[index]->structarray[3].z = 0;

				Sorted.RenderArray[index]->structarray[4].x = 2;		// vertex 4
				Sorted.RenderArray[index]->structarray[4].y = 2;
				Sorted.RenderArray[index]->structarray[4].z = -2;

				Sorted.RenderArray[index]->structarray[5].x = 2;		// vertex 5
				Sorted.RenderArray[index]->structarray[5].y = 2;
				Sorted.RenderArray[index]->structarray[5].z = 0;

				Sorted.RenderArray[index]->structarray[6].x = 2;		// vertex 6
				Sorted.RenderArray[index]->structarray[6].y = 0;
				Sorted.RenderArray[index]->structarray[6].z = -2;

				Sorted.RenderArray[index]->structarray[7].x = 2;		// vertex 7
				Sorted.RenderArray[index]->structarray[7].y = 0;
				Sorted.RenderArray[index]->structarray[7].z = 0;


				index++;
			}
		}
	}
}

void Enclave::SortRenderArray() // this function performs sorting on RenderArray -- will be used after InitializeRenderArray() is called, to sort for the first time (Insertion sort?)
{
	/* Summary: sorts blocks in the enclave based on if the otherflags member variable is >= 1; 0 indicates that the block will not be rendered */

	//cout << "SORT CALLED" << endl;
	int  element_found = 0, j = 0;				// declare variables for following loops
	int tempInt;
	int sortcount = 0;
	EnclavePolyArray *tempPtr = Sorted.RenderArray[0];
	int i = 1;														// set index of the first element to check during sort
	for (i = 1; i < 64; ++i)
	{
		element_found = 0;					// stores total number of renderable elements (used later to determine the # of elements to look for in the for loop
		for (j = i; j >= 1; --j)
		{
			if ((Sorted.RenderArray[j]->otherflags == 1) && (Sorted.RenderArray[j - 1]->otherflags == 0)) // if the current element = value of 0, and the next one is a value of 1, swap. NOTE: Check == vs =.  // && (Sorted.RenderArray[i - 1]->otherflags == 0)
			{
				/*if (j <= 20)
				{
				cout << "Last set: " << Sorted.RenderArray[j]->otherflags << " | " << Sorted.RenderArray[j - 1]->otherflags << " | " << j << endl;
				}*/
				// sort the values first
				tempInt = Sorted.PolyArrayIndex[j];
				Sorted.PolyArrayIndex[j] = Sorted.PolyArrayIndex[j - 1];
				Sorted.PolyArrayIndex[j - 1] = tempInt;

				// sort the pointers second
				tempPtr = Sorted.RenderArray[j]; // Questionable...??
				Sorted.RenderArray[j] = Sorted.RenderArray[j - 1];
				Sorted.RenderArray[j - 1] = tempPtr;

				element_found = 1;
				//cout << "sort triggered! " << sortcount++ << endl;


			}
		}
		//cout << endl;
		if (element_found == 1)
		{
			TotalRenderable += 1;
		}

	}
	//cout << "Total sorted polygons:" << TotalRenderable << endl;
}


void Enclave::UnveilSinglePolyWithMtrl(int x, int y, int z, int in_readorder, int in_otherflags, short in_blockid, char in_t1, char in_t2)		// reveals ("unveils") and changes the material of one block at xyz, 
{
	/* Summary: changes a polygon's render state to true if the otherflags member variable >= 1; also changes the block's material during the unveiling */
	int OldFlags = StorageArray[x][y][z].otherflags;			// store the old data in OldFlags
	StorageArray[x][y][z].otherflags = in_otherflags;		    // set other flags
	StorageArray[x][y][z].blockid = in_blockid;					// set block id
	StorageArray[x][y][z].t1_flags = in_t1;						// set type 1 renderable faces
	StorageArray[x][y][z].t2_flags = in_t2;						// set type 2 renderable faces

	//cout << "TEST:::" << OldFlags << endl;															// set up loop to find this polygon's position in Sorted (which contains a sorted array of pointers to StorageArray, sorted by renderables first)
	int i, j, tempInt, zeroindexcheck;
	EnclavePolyArray *tempPtr = Sorted.RenderArray[0];
	int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
																// do the following, if the new form of the cube is set to renderable
	if (in_otherflags >= 1)
	{
		//std::cout << "debug:" << StorageArray[x][y][z].otherflags << endl;
		zeroindexcheck = 0;
		for (i = 0; i < 64; ++i)
		{
			//cout << "test:" << Sorted.PolyArrayIndex[i] << endl;
			//cout << "test: multi_to_single: " << multi_to_single << endl;
			if (Sorted.PolyArrayIndex[i] == multi_to_single) // find the index of the element to change, by finding the
															 // match index that was converted from 3d to 1d to its appropriate index in Sorted.PolyArrayindex
			{


				//std::cout << "debug (sorted entry):" << StorageArray[x][y][z].otherflags << endl;

				// do sorting for this one polygon, if the previous polygon wasn't already renderable
				if (OldFlags == 0)
				{
					//cout << "TEST2:::" << OldFlags << endl;

					if (i != 0)
					{
						for (j = i; j >= 1; --j)
						{
							//cout << "value of j: " << j << endl;
							//cout << " test" << Sorted.RenderArray[j]->otherflags << endl;
							if ((Sorted.RenderArray[j]->otherflags >= 1) && (Sorted.RenderArray[j - 1]->otherflags == 0)) // swap values first
							{
								//cout << "TEST" << i << ": " << OldFlags << endl;
								// sort the values first
								tempInt = Sorted.PolyArrayIndex[j];
								Sorted.PolyArrayIndex[j] = Sorted.PolyArrayIndex[j - 1];
								Sorted.PolyArrayIndex[j - 1] = tempInt;

								// sort the pointers second
								tempPtr = Sorted.RenderArray[j]; // Questionable...??
								Sorted.RenderArray[j] = Sorted.RenderArray[j - 1];
								Sorted.RenderArray[j - 1] = tempPtr;
								//cout << "single sort count:" << ++sortcount << endl;
							}
							else
							{
								break;
							}
							//cout << "for loop count" << ++sortcount << endl;
						}
						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
						//cout << "Chunk (" << this->UniqueKey.x << ", " << this->UniqueKey.y << ", " << this->UniqueKey.z << ") Total triangles in chunk so far: " << total_triangles << endl;


						break;
					}
					else
					{

						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
						//cout << "Chunk (" << this->UniqueKey.x << ", " << this->UniqueKey.y << ", " << this->UniqueKey.z << ") Total triangles in chunk so far: " << total_triangles << endl;
					}
				}

			}

			//testcount++;
		}
		if ((zeroindexcheck == 0) && (OldFlags == 0))	// do this, if the very first block to be sorted is the one at 0, 0, 0
		{
			++TotalRenderable;
			total_triangles += GetTotalTrianglesInBlock(in_t1);
			//cout << "Very first block is 0: " << total_triangles << endl;
		}
		//cout << "test count value: " << testcount;
	}

	// perform post unveil operations; notify neighboring enclaves and blocks of required changes

}

void Enclave::UnveilSinglePoly(int x, int y, int z, int in_readorder, int in_otherflags, char in_t1, char in_t2)
{
	/* Summary: changes a polygon's render state to true if the otherflags member variable >= 1*/
	int OldFlags = StorageArray[x][y][z].otherflags;			// store the old data in OldFlags
	StorageArray[x][y][z].otherflags = in_otherflags;		    // set other flags
	StorageArray[x][y][z].t1_flags = in_t1;						// set type 1 renderable faces
	StorageArray[x][y][z].t2_flags = in_t2;						// set type 2 renderable faces

																//cout << "TEST:::" << OldFlags << endl;															// set up loop to find this polygon's position in Sorted (which contains a sorted array of pointers to StorageArray, sorted by renderables first)
	int i, j, tempInt, zeroindexcheck;
	EnclavePolyArray *tempPtr = Sorted.RenderArray[0];
	int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
																// do the following, if the new form of the cube is set to renderable
	if (in_otherflags >= 1)
	{
		//std::cout << "debug:" << StorageArray[x][y][z].otherflags << endl;
		zeroindexcheck = 0;
		for (i = 0; i < 64; ++i)
		{
			//cout << "test:" << Sorted.PolyArrayIndex[i] << endl;
			//cout << "test: multi_to_single: " << multi_to_single << endl;
			if (Sorted.PolyArrayIndex[i] == multi_to_single) // find the index of the element to change, by finding the
															 // match index that was converted from 3d to 1d to its appropriate index in Sorted.PolyArrayindex
			{


				//std::cout << "debug (sorted entry):" << StorageArray[x][y][z].otherflags << endl;

				// do sorting for this one polygon, if the previous polygon wasn't already renderable
				if (OldFlags == 0)
				{
					//cout << "TEST2:::" << OldFlags << endl;

					if (i != 0)
					{
						for (j = i; j >= 1; --j)
						{
							//cout << "value of j: " << j << endl;
							//cout << " test" << Sorted.RenderArray[j]->otherflags << endl;
							if ((Sorted.RenderArray[j]->otherflags >= 1) && (Sorted.RenderArray[j - 1]->otherflags == 0)) // swap values first
							{
								//cout << "TEST" << i << ": " << OldFlags << endl;
								// sort the values first
								tempInt = Sorted.PolyArrayIndex[j];
								Sorted.PolyArrayIndex[j] = Sorted.PolyArrayIndex[j - 1];
								Sorted.PolyArrayIndex[j - 1] = tempInt;

								// sort the pointers second
								tempPtr = Sorted.RenderArray[j]; // Questionable...??
								Sorted.RenderArray[j] = Sorted.RenderArray[j - 1];
								Sorted.RenderArray[j - 1] = tempPtr;
								//cout << "single sort count:" << ++sortcount << endl;
							}
							else
							{
								break;
							}
							//cout << "for loop count" << ++sortcount << endl;
						}
						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
						//cout << "Chunk (" << this->UniqueKey.x << ", " << this->UniqueKey.y << ", " << this->UniqueKey.z << ") Total triangles in chunk so far: " << total_triangles << endl;


						break;
					}
					else
					{

						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
						//cout << "Chunk (" << this->UniqueKey.x << ", " << this->UniqueKey.y << ", " << this->UniqueKey.z << ") Total triangles in chunk so far: " << total_triangles << endl;
					}
				}

			}

			//testcount++;
		}
		if ((zeroindexcheck == 0) && (OldFlags == 0))	// do this, if the very first block to be sorted is the one at 0, 0, 0
		{
			++TotalRenderable;
			total_triangles += GetTotalTrianglesInBlock(in_t1);
			//cout << "Very first block is 0: " << total_triangles << endl;
		}
		//cout << "test count value: " << testcount;
	}

	// perform post unveil operations; notify neighboring enclaves and blocks of required changes
}

void Enclave::VeilSinglePoly(int x, int y, int z, int in_readorder, int in_otherflags, short in_blockid, char in_t1, char in_t2)		// reveals ("unveils") and changes one block at xyz, 
{
	/* Summary: changes a polygon's render state to false if the otherflags member variable == 0*/
	int OldFlags = StorageArray[x][y][z].otherflags;			// store the old data in OldFlags
	StorageArray[x][y][z].otherflags = in_otherflags;		    // set other flags
	StorageArray[x][y][z].blockid = in_blockid;					// set block id
	StorageArray[x][y][z].t1_flags = in_t1;						// set type 1 renderable faces
	StorageArray[x][y][z].t2_flags = in_t2;						// set type 2 renderable faces

	//cout << "TEST:::" << OldFlags << endl;															// set up loop to find this polygon's position in Sorted (which contains a sorted array of pointers to StorageArray, sorted by renderables first)
	int i, j, tempInt, zeroindexcheck;
	EnclavePolyArray *tempPtr = Sorted.RenderArray[0];
	int multi_to_single = (x * 16) + (y * 4) + z;
	if (in_otherflags == 0)
	{
		//cout << "TEST (otherflags == 0) :::" << OldFlags << endl;
		for (i = 0; i < 63; ++i)
		{
			if (Sorted.PolyArrayIndex[i] == multi_to_single) // find the index of the element to change, by finding the
															 // match index that was converted from 3d to 1d to its appropriate index in Sorted.PolyArrayindex
			{
				if (OldFlags >= 1)
				{

					if (i != 0)
					{
						for (j = i; j < 63; ++j) // don't touch the last element, stop at element 63 (which is at index 62)
						{
							if ((Sorted.RenderArray[j]->otherflags == 0) && (Sorted.RenderArray[j + 1]->otherflags >= 1)) // swap values first
							{
								// sort the values first
								tempInt = Sorted.PolyArrayIndex[j];
								Sorted.PolyArrayIndex[j] = Sorted.PolyArrayIndex[j + 1];
								Sorted.PolyArrayIndex[j + 1] = tempInt;

								// sort the pointers second
								tempPtr = Sorted.RenderArray[j]; // Questionable...??
								Sorted.RenderArray[j] = Sorted.RenderArray[j + 1];
								Sorted.RenderArray[j + 1] = tempPtr;
								//cout << "single sort count:" << ++sortcount << endl;
							}
							else
							{
								break;
							}
						}
						--TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles -= GetTotalTrianglesInBlock(in_t1);
						cout << "Total triangles in chunk so far: " << total_triangles << endl;

						break;
					}
					else
					{

						--TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles -= GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
						//cout << "Chunk (" << this->UniqueKey.x << ", " << this->UniqueKey.y << ", " << this->UniqueKey.z << ") Total triangles in chunk so far: " << total_triangles << endl;
					}
				}

			}
		}
	}
}

void Enclave::TestTopLayer()
{
	/* Summary: sets the otherflags for a pre-set number of blocks to 1*/
	int i, j, k = 0;
	for (i = 2; i < 4; ++i)			// 2 is for testing, would be 8 sorted polygons.
	{
		for (j = 0; j < 4; ++j)
		{
			Sorted.RenderArray[(i * 4) + j]->otherflags = 1;
			//cout << "whoa!" << endl;
		}
	}
}

void Enclave::ViewOtherFlags()
{
	/* Summary: loops and outputs the other flags value for each individual block */
	int index = 0, i, j, k;			// declare variables for following loops
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				cout << Sorted.RenderArray[index]->otherflags << ":" << Sorted.PolyArrayIndex[index] << " "; // working
				index++;
			}
			cout << endl;
		}
	}
}

int Enclave::GetTotalTrianglesInBlock(char in_char)
{
	/* Summary: gets the total number of triangles per block */
	int bitmask = 32;
	int return_triangles = 0;
	for (int z = 0; z < 6; ++z)
	{
		if ((in_char &  bitmask) == bitmask)
		{
			return_triangles += 2;
		}
		bitmask >>= 1;				// shift bitmask to the right by one bit
	}
	return return_triangles;
}

int Enclave::GetTotalTrianglesInEnclave()
{
	/* Summary: returns the current number of triangles that will be rendered in the enclave */
	return total_triangles;
}

void Enclave::ChangePolyMaterial(int x, int y, int z, int newmaterial)
{
	/* Summary: updates the material of the block at x/y/z */
	StorageArray[x][y][z].blockid = newmaterial;
}

Enclave::Enclave(int x,	int y, int z)		// declares the enclave's position in the world space (x, y, z)
{
	/* Summary: default constructor; should NOT be used when creating enclave that goes into a collection. */

	//cout << "CONSTRUCTOR CALLED " << endl;
	this->UniqueKey.x = x;
	this->UniqueKey.y = y;
	this->UniqueKey.z = z;
	

}

EnclaveKeyDef::EnclaveKey Enclave::SingleToEnclaveKey(int input)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
	EnclaveKeyDef::EnclaveKey tempkey;

	int x = input / 16;
	int remainder_x = input % 16;

	int y = remainder_x / 4;
	int remainder_y = remainder_x % 4;

	int z = remainder_y;

	//cout << x << " " << y << " " << z << " " << endl;
	tempkey.x = x;
	tempkey.y = y;
	tempkey.z = z;
	return tempkey;
}

void Enclave::UnveilMultipleAndNotifyNeighbors(EnclaveUnveilMeta metaArray, EnclaveCollectionBorderFlags* borderflagsref, ElevationMapRef mapRefVal, EnclaveCollection* collectionRefPtr, int filldirection)
{
	//cout << "test: " << metaArray.numberOfBlocks;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	EnclaveKeyDef::EnclaveKey tempKey;
	for (int x = 0; x < metaArray.numberOfBlocks; x++) // iterate for the number of blocks found in the metaArray
	{
		EnclaveKeyDef::Enclave2DKey retrieved2d = SingleTo2d(x);								// get 2d value
		EnclaveKeyDef::EnclaveKey tempBlockKey = SingleToEnclaveKey(metaArray.EnclaveBlockLocation[retrieved2d.a][retrieved2d.b]);		// convert the single value at the 2d value to the appropriate enclave key;

		EnclaveNeighborMeta neighborMeta = GenerateNeighborMeta(collectionRefPtr);

		if (filldirection == 0) // negative y jobs
		{
			char fillflag = 2;			// the initial flag value, should be equal to top face value (2); top face should always be rendered for filldirection of 0 (- y)
			//if (this->UniqueKey.z == 0) 


			//************************************************************
			// determine North faces for the chunk, if it's on the border north border
			//************************************************************
			if (this->UniqueKey.z == 0) 	// if the block is a border block on the enclave, do this 
			{
				
				if (tempBlockKey.z == 0)
				{
					int tempval = (tempBlockKey.y + 1);		// set the tempval equal to the number of blocks, including this one and any below it.
					for (int y = tempval; y > 0; y--)		// iterate from the top most block ( the top face) downward; 0 = the top block
					{
						//cout << "test" << endl;
						fillflag = 2;

						// check if north border flag is set; only render north faces if it is set
						if (borderflagsref->North == 1)
						{
							if (y == tempval)
							{

								fillflag = fillflag | 16;		// x = 2 is the top, so set the top face this one time
							}
							else
							{
								fillflag = 16;
							}
							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag, 0);
						}
						if (borderflagsref->North == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag, 0);
							}
						}


						//UnveilSinglePoly(tempBlockKey.x, y-1, tempBlockKey.z, 0, 1, fillflag, 0);

					}
				}

				else
				{
					UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag, 0);
				}


			}


			//************************************************************
			// determine South faces for the chunk, if it's on the south border
			//************************************************************
			else if (this->UniqueKey.z == 7)
			{
				if (tempBlockKey.z == 3)
				{
					int tempval = (tempBlockKey.y + 1);		// set the tempval equal to the number of blocks, including this one and any below it.
					for (int y = tempval; y > 0; y--)		// iterate from the top most block ( the top face) downward; 0 = the top block
					{
						//cout << "test" << endl;
						fillflag = 2;

						// check if north border flag is set; only render north faces if it is set
						if (borderflagsref->South == 1)
						{
							if (y == tempval)
							{

								fillflag = fillflag | 4;		// x = 2 is the top, so set the top face this one time
							}
							else
							{
								fillflag = 4;
							}
							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag, 0);
						}
						if (borderflagsref->South == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag, 0);
							}
						}


						//UnveilSinglePoly(tempBlockKey.x, y-1, tempBlockKey.z, 0, 1, fillflag, 0);

					}
				}

				else
				{
					UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag, 0);
				}
			}


			// do this for all non-border chunks
			else  if (this->UniqueKey.z != 0 && this->UniqueKey.z != 7)
			{
				UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag, 0);
			}
		}

	}
}

EnclaveKeyDef::Enclave2DKey Enclave::SingleTo2d(int input)
{
	EnclaveKeyDef::Enclave2DKey returnKey;

	int x = input / 4;
	int remainder_x = input % 4;

	int y = remainder_x;

	returnKey.a = x;
	returnKey.b = y;

	return returnKey;
}

EnclaveNeighborMeta Enclave::GenerateNeighborMeta(EnclaveCollection* enclaveCollectionRef)
{
	EnclaveNeighborMeta returnMeta;

	//check west first
	if (this->UniqueKey.x != 0)	// check only if it isn't a border chunk, first
	{
		Enclave* enclavePtr = &enclaveCollectionRef->EnclaveArray[(this->UniqueKey.x)-1][this->UniqueKey.y][this->UniqueKey.z];	// get the neighboring enclave at -1 x
		for (int y = 0; y < 4; y++)		// iterate on y axis
		{
			for (int z = 0; z < 4; z++)	// iterate on z axis
			{
				if (enclavePtr->StorageArray[3][y][z].otherflags == 0)		// x is static = 3; this is equal to the east 
				{
					// index 0 = object for West unveil meta data
					returnMeta.MetaArrays[0].unveilMeta.EnclaveBlockLocation[y][z] = EnclaveCoordsToSingle(0, y, z);
					returnMeta.MetaArrays[0].unveilMeta.BlockFacesToRender[y][z] = 32;									// set west face bit
				}
			}
		}
	}

	return returnMeta;
}

int Enclave::EnclaveCoordsToSingle(int in_x, int in_y, int in_z)
{
	int x = in_x * 16;
	int y = in_y * 4;
	//int z = InKey.z;
	return x + y + in_z;
}