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
#include "EnclaveCollectionNeighborList.h"

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
				Sorted.RenderArray[index]->structarray[0].x = 0;		// vertex 0			(UPPER SOUTHWEST)
				Sorted.RenderArray[index]->structarray[0].y = 2;
				Sorted.RenderArray[index]->structarray[0].z = 2;		// old: -2

				Sorted.RenderArray[index]->structarray[1].x = 0;		// vertex 1			(UPPER NORTHWEST)
				Sorted.RenderArray[index]->structarray[1].y = 2;
				Sorted.RenderArray[index]->structarray[1].z = 0;

				Sorted.RenderArray[index]->structarray[2].x = 0;		// vertex 2			(LOWER SOUTHWEST)
				Sorted.RenderArray[index]->structarray[2].y = 0;
				Sorted.RenderArray[index]->structarray[2].z = 2;		// old -2 

				Sorted.RenderArray[index]->structarray[3].x = 0;		// vertex 3 (true 0)	(LOWER NORTHWEST)
				Sorted.RenderArray[index]->structarray[3].y = 0;
				Sorted.RenderArray[index]->structarray[3].z = 0;

				Sorted.RenderArray[index]->structarray[4].x = 2;		// vertex 4			(UPPER SOUTHEAST)
				Sorted.RenderArray[index]->structarray[4].y = 2;
				Sorted.RenderArray[index]->structarray[4].z = 2;		// old : -2

				Sorted.RenderArray[index]->structarray[5].x = 2;		// vertex 5			(UPPER NORTHEAST)
				Sorted.RenderArray[index]->structarray[5].y = 2;
				Sorted.RenderArray[index]->structarray[5].z = 0;

				Sorted.RenderArray[index]->structarray[6].x = 2;		// vertex 6			(LOWER SOUTHEAST)
				Sorted.RenderArray[index]->structarray[6].y = 0;
				Sorted.RenderArray[index]->structarray[6].z = 2;		// old: -2

				Sorted.RenderArray[index]->structarray[7].x = 2;		// vertex 7			(LOWER NORTHEAST)
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
				Sorted.RenderArray[index]->structarray[0].x = 0;		// vertex 0		(UPPER SOUTHWEST)
				Sorted.RenderArray[index]->structarray[0].y = 2;
				Sorted.RenderArray[index]->structarray[0].z = 2;		// old: -2

				Sorted.RenderArray[index]->structarray[1].x = 0;		// vertex 1		(UPPER NORTHWEST)
				Sorted.RenderArray[index]->structarray[1].y = 2;
				Sorted.RenderArray[index]->structarray[1].z = 0;

				Sorted.RenderArray[index]->structarray[2].x = 0;		// vertex 2		(LOWER SOUTHWEST)
				Sorted.RenderArray[index]->structarray[2].y = 0;
				Sorted.RenderArray[index]->structarray[2].z = 2;		// old -2 

				Sorted.RenderArray[index]->structarray[3].x = 0;		// vertex 3 (true 0) (LOWER NORTHWEST)
				Sorted.RenderArray[index]->structarray[3].y = 0;
				Sorted.RenderArray[index]->structarray[3].z = 0;

				Sorted.RenderArray[index]->structarray[4].x = 2;		// vertex 4		(UPPER SOUTHEAST)
				Sorted.RenderArray[index]->structarray[4].y = 2;
				Sorted.RenderArray[index]->structarray[4].z = 2;		// old : -2

				Sorted.RenderArray[index]->structarray[5].x = 2;		// vertex 5		(UPPER NORTHEAST)
				Sorted.RenderArray[index]->structarray[5].y = 2;
				Sorted.RenderArray[index]->structarray[5].z = 0;

				Sorted.RenderArray[index]->structarray[6].x = 2;		// vertex 6		(LOWER SOUTHEAST)
				Sorted.RenderArray[index]->structarray[6].y = 0;
				Sorted.RenderArray[index]->structarray[6].z = 2;		// old: -2

				Sorted.RenderArray[index]->structarray[7].x = 2;		// vertex 7		(LOWER NORTHEAST)
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
	int bitDifference = 0;
	int old_t1_flags = StorageArray[x][y][z].t1_flags;
	StorageArray[x][y][z].otherflags = in_otherflags;		    // set other flags
	StorageArray[x][y][z].blockid = in_blockid;					// set block id
	StorageArray[x][y][z].t1_flags = StorageArray[x][y][z].t1_flags | in_t1;						// set type 1 renderable faces
	StorageArray[x][y][z].t2_flags = StorageArray[x][y][z].t1_flags | in_t2;						// set type 2 renderable faces
	//StorageArray[x][y][z].t1_flags =  in_t1;						// set type 1 renderable faces
	//StorageArray[x][y][z].t2_flags =  in_t2;						// set type 2 renderable faces


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
						break;
					}
					else
					{

						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
					}
				}
				if (OldFlags >= 1)
				{
					int bitmask = 5;
					int currentmask = 1;
					//int currentmask = 0;
					for (int f = 0; f < 6; f++)
					{
						currentmask = 1;
						currentmask <<= bitmask;
						if
							(
								((old_t1_flags & currentmask) != currentmask)
								&&
								((StorageArray[x][y][z].t1_flags & currentmask) == currentmask)			//	if these conditions are true,it indicates that this polygon face was not previously being rendered, 
																										//	even though the rest of the polygon may have been-- so increment the triangle count by 2					
							)

						{
							total_triangles += 2;
						}
						bitmask--;
					}
					//total_triangles += GetTotalTrianglesInBlock(in_t1);
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
	int old_t1_flags = StorageArray[x][y][z].t1_flags;
	StorageArray[x][y][z].otherflags = in_otherflags;		    // set other flags
	StorageArray[x][y][z].t1_flags = StorageArray[x][y][z].t1_flags | in_t1;						// set type 1 renderable faces
	StorageArray[x][y][z].t2_flags = StorageArray[x][y][z].t1_flags | in_t2;						// set type 2 renderable faces
	//StorageArray[x][y][z].t1_flags = in_t1;						// set type 1 renderable faces
	//StorageArray[x][y][z].t2_flags = in_t2;						// set type 2 renderable faces

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
						break;
					}
					else
					{

						++TotalRenderable;
						// loop for each bit of in_t1 here
						total_triangles += GetTotalTrianglesInBlock(in_t1);
						zeroindexcheck = 1;
					}
				}
				if (OldFlags >= 1)
				{
					int bitmask = 5;
					int currentmask = 1;
					//int currentmask = 0;
					for (int f = 0; f < 6; f++)
					{
						currentmask = 1;
						currentmask <<= bitmask;
						if
						(
							((old_t1_flags & currentmask) != currentmask)
							&&
							((StorageArray[x][y][z].t1_flags & currentmask) == currentmask)				//	if these conditions are true,it indicates that this polygon face was not previously being rendered, 
							)																			//	even though the rest of the polygon may have been-- so increment the triangle count by 2					
						{
							total_triangles += 2;
						}
						bitmask--;
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
	int old_t1_flags = StorageArray[x][y][z].t1_flags;
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

void Enclave::UnveilMultipleAndNotifyNeighbors(EnclaveUnveilMeta metaArray, EnclaveCollectionBorderFlags* borderflagsref, ElevationMapRef mapRefVal, EnclaveCollection* collectionRefPtr, EnclaveCollectionNeighborList neighborList, int filldirection)
{
	/* Summary: this function calls GenerateNeighborMeta to determine which blueprints to look at, based on the Enclave's unique key. 
	   It then uses the result of this to populate all faces for the enclave.
	*/

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	//EnclaveKeyDef::EnclaveKey tempKey;
	EnclaveNeighborMeta neighborMeta = GenerateNeighborMeta(collectionRefPtr, neighborList);
	for (int x = 0; x < metaArray.numberOfBlocks; x++) // iterate for the number of blocks found in the metaArray
	{
		EnclaveKeyDef::Enclave2DKey retrieved2d = SingleTo2d(x);								// get 2d value
		EnclaveKeyDef::EnclaveKey tempBlockKey = SingleToEnclaveKey(metaArray.EnclaveBlockLocation[retrieved2d.a][retrieved2d.b]);		// convert the single value at the 2d value to the appropriate enclave key;

		

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

								fillflag = fillflag | 16;		// x = 2 is the top, so set the top face + 16 (the north face) this one time
							}
							else
							{
								fillflag = 16;
							}
							//fillflag = fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]);

							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
						}
						if (borderflagsref->North == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
							}
						}

						/* --------------------------------------
						Need logic for adding additional blocks to unveil, based on data in EnclaveNeighborMeta 
						--------------------------------------*/

						/*
						if (this->UniqueKey.x == 1 && this->UniqueKey.y == 6 && this->UniqueKey.z == 0)
						{
							if (tempBlockKey.x == 0 && tempBlockKey.y == 1 && tempBlockKey.z == 0)
							{
								cout << "bit value: " << int(neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]) << endl;
							}
							
						}
						*/
						//UnveilSinglePoly(tempBlockKey.x, y-1, tempBlockKey.z, 0, 1, fillflag, 0);
						//UnveilSinglePoly(tempBlockKey.x, y-1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y-1][tempBlockKey.z]), 0);


					}
				}

				else
				{
					//fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][tempBlockKey.y][tempBlockKey.z])
					UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][tempBlockKey.y][tempBlockKey.z]), 0);
				}


			}


			//************************************************************
			// determine South faces for the chunk, if it's on the south border
			//************************************************************
			if (this->UniqueKey.z == 7)
			{
				if (tempBlockKey.z == 3)
				{
					int tempval = (tempBlockKey.y + 1);		// set the tempval equal to the number of blocks, including this one and any below it.
					for (int y = tempval; y > 0; y--)		// iterate from the top most block ( the top face) downward; 0 = the top block
					{
						//cout << "test" << endl;
						fillflag = 2;

						// check if south border flag is set; only render north faces if it is set
						if (borderflagsref->South == 1)
						{
							if (y == tempval)
							{

								fillflag = fillflag | 4;		// x = 2 is the top, so set the top face + 4 (the south face) this one time
							}
							else
							{
								fillflag = 4;
							}
							//fillflag = fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]);

							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
						}
						if (borderflagsref->South == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
								//UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag, 0);
							}
						}


						//UnveilSinglePoly(tempBlockKey.x, y-1, tempBlockKey.z, 0, 1, fillflag, 0);

					}
				}

				else
				{
					UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][tempBlockKey.y][tempBlockKey.z]), 0);
				}
			}

			//************************************************************
			// determine East faces for the chunk, if it's on the east border
			//************************************************************
			
			if (this->UniqueKey.x == 7)
			{

				if (tempBlockKey.x == 3)
				{
					int tempval = (tempBlockKey.y + 1);
					for (int y = tempval; y > 0; y--)		// iterate from the top most block ( the top face) downward; 0 = the top block
					{
						//cout << "test" << endl;
						fillflag = 2;

						// check if east border flag is set; only render north faces if it is set
						if (borderflagsref->East == 1)
						{
							if (y == tempval)
							{

								fillflag = fillflag | 8;		// fillflag = 2 is the top, so set the fillflag + 8 (the east face) this one time
							}
							else
							{
								fillflag = 8;
							}

							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
						}
						if (borderflagsref->East == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
							}
						}
					}
				}
			}

			//************************************************************
			// determine West faces for the chunk, if it's on the east border
			//************************************************************
			if (this->UniqueKey.x == 0)
			{
				if (tempBlockKey.x == 0)
				{
					int tempval = (tempBlockKey.y + 1);
					for (int y = tempval; y > 0; y--)		// iterate from the top most block ( the top face) downward; upper most y = the top block
					{
						fillflag = 2;
						if (borderflagsref->West == 1)		// check if east border flag is set; only render north faces if it is set
						{
							if (y == tempval)
							{
								fillflag = fillflag | 32;
							}
							else
							{
								fillflag = 32;
							}
							UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
						}

						if (borderflagsref->West == 0)
						{
							if (y == tempval)
							{
								UnveilSinglePoly(tempBlockKey.x, y - 1, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][y - 1][tempBlockKey.z]), 0);
							}
						}
					}
				}
			}


			// do this for all non-border chunks
			if (this->UniqueKey.z != 0 && this->UniqueKey.z != 7)
			{
				//UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag, 0);
				UnveilSinglePoly(tempBlockKey.x, tempBlockKey.y, tempBlockKey.z, 0, 1, fillflag | (neighborMeta.NeighborBlockData[tempBlockKey.x][tempBlockKey.y][tempBlockKey.z]), 0);
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

EnclaveNeighborMeta Enclave::GenerateNeighborMeta(EnclaveCollection* enclaveCollectionRef, EnclaveCollectionNeighborList neighborList)
{
	EnclaveNeighborMeta returnMeta;
	// required: initialize array to all 0s
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int z = 0; z < 4; z++)
			{
				returnMeta.NeighborBlockData[x][y][z] = 0;
			}
		}
	}
	returnMeta.NeighborBlockData[0][0][0] = 0;
	/* -------------------------NON BORDER CHECKS (?) -------------------------*/
	//check west, if not a border chunk
	if (this->UniqueKey.x != 0)	// check only if it isn't a border chunk, first
	{
		Enclave* enclaveNeighborPtr = &enclaveCollectionRef->EnclaveArray[(this->UniqueKey.x)-1][this->UniqueKey.y][this->UniqueKey.z];	// get the neighboring enclave at -1 x
		for (int y = 0; y < 4; y++)		// iterate on y axis
		{
			for (int z = 0; z < 4; z++)	// iterate on z axis
			{
				if (enclaveNeighborPtr->StorageArray[3][y][z].otherflags == 0)		// x is static = 3; this is equal to the east 
				{
					returnMeta.NeighborBlockData[0][y][z] = returnMeta.NeighborBlockData[0][y][z] | 32;
				}
			}
		}
	}

	// if it is a border chunk,

	/* -------------------------WEST CHECKS-------------------------*/
	if (this->UniqueKey.x == 0)
	{
		if (neighborList.WestNeighborExists == 0)			// there is no neighboring collection to the west; all west sides of border chunks need to be painted
		{
			//cout << "no west neighbor!" << endl;
			for (int y = 0; y < 4; y++)		// iterate on y axis
			{
				for (int z = 0; z < 4; z++)	// iterate on z axis
				{
						returnMeta.NeighborBlockData[0][y][z] = returnMeta.NeighborBlockData[0][y][z] | 32;
								
				}
			}
		}
		if (neighborList.WestNeighborExists == 1)
		{
			ECBOutwardFaceFlags* faceFlagsRef = &neighborList.westPtr->EastBorderBlocks;
			int zoffset = (this->UniqueKey.z) * 4;											// will store which of the EastBorderBlock integers to start at
			int yoffset = (this->UniqueKey.y) * 4;											// will store which bits in the integer to start at




			//cout << "west chunk found!" << endl;
		}
	}

	/* ------------------------NORTH CHECKS--------------------------*/
	if (this->UniqueKey.z == 0)
	{
		if (neighborList.NorthNeighborExists == 0)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					returnMeta.NeighborBlockData[x][y][0] = returnMeta.NeighborBlockData[x][y][0] | 16;
				}
			}
		}
	}

	/* -------------------------EAST CHECKS-------------------------*/
	// check east, if not a border chunk
	if (this->UniqueKey.x == 7)
	{
		if (neighborList.EastNeighborExists == 0)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int z = 0; z < 4; z++)
				{
					returnMeta.NeighborBlockData[3][y][z] = returnMeta.NeighborBlockData[3][y][z] | 8;
				}
			}
		}


	}

	/* ------------------------SOUTH CHECKS--------------------------*/
	if (this->UniqueKey.z == 7)
	{
		if (neighborList.SouthNeighborExists == 0)
		{
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					returnMeta.NeighborBlockData[x][y][3] = returnMeta.NeighborBlockData[x][y][3] | 4;
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
	return x + y + in_z;
}