#pragma once

using namespace std;

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <vector>

#include "EnclaveManifest.h"
#include "RenderCollection.h"

														// alternating bitmask for face operations, initialized to 0, but range will be <= 93. (00111111)


EnclaveManifest::EnclaveManifest(void)
{
	/* Summary: default constructor, required? */
}

void EnclaveManifest::AttachToEnclave(Enclave &in_ptr)	// "attaches" this manifest to a Enclave that resides in memory
{
	/* Summary: Attaches to an existing, valid instance of an Enclave. 3d point data is generated based on the contents of the Enclave. */
	if (IsEnclaveGLPtrLoaded == 1)						// check to see if it was loaded already, on a previous call.
	{
		delete[] EnclaveGLPtr;							// delete old array
	}

	IsEnclaveGLPtrLoaded = 1;
	//cout << "Enclave Attach call..." << endl;
	EnclavePtr = in_ptr;													// set the pointer equal to the reference of the input parameter
	//cout << "Info for Enclave at: " << EnclavePtr.UniqueKey.x << ", " << EnclavePtr.UniqueKey.y << ", " << EnclavePtr.UniqueKey.z << " " << endl;
	//cout << "Total triangles from attached enclave:" << EnclavePtr->GetTotalTrianglesInEnclave() << endl;

	//EnclaveManifestRenderables = new EnclaveManifest::EnclaveManifestTuple[EnclavePtr->GetTotalTrianglesInEnclave()];				// FIX THIS!
	EnclaveGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave())*9];
	TotalEnclaveTriangles = EnclavePtr.GetTotalTrianglesInEnclave();
	//cout << "test: TotalEnclaveTriangles ->" << TotalEnclaveTriangles << endl;
	RenderablePolyCount = EnclavePtr.TotalRenderable;						// don't perform unnecessary loops through all 64 elements, only go x times (where x is number of polygons to render)

	//polyfacebitmask = 32;															// set bitmask to 32 initially.
	//cout << "out test initial " << polyfacebitmask << "; renderable poly count = " << RenderablePolyCount << endl;							// testing

	char testval[36] = { 0, 1, 2, 1, 2, 3,								// negative x			(WEST)		(32)	// this array could be constant?	
						 1, 5, 3, 5, 3, 7,								// negative z			(NORTH)		(16)
						 5, 4, 7, 4, 7, 6,								// positive x			(EAST)		(8)
						 0, 4, 6, 0, 6, 2,								// positive z			(SOUTH)		(4)
						 1, 5, 0, 5, 0, 4,								// positive y			(TOP)		(2)
						 3, 7, 2, 7, 2, 6								// negative y			(BOTTOM)	(1)
						};												// also for testing
	GLfloat GL_x = 0.5f;		// instantiate within stack frame
	GLfloat GL_y = 0.5f;
	GLfloat GL_z = 0.5f;
	int iteratorval, totaltuples = 0;														// more testing

	for (int i = 0; i < RenderablePolyCount; ++i)
	{
		EnclavePolyArrayPtr = EnclavePtr.Sorted.RenderArray[i];			// point to the Enclave's sorted poly array 
		//cout << "debug: RenderablePolyCount before crash: " << RenderablePolyCount << endl;
		int t1_flagsint = EnclavePolyArrayPtr->t1_flags;			
		polyfacebitmask = 32;
		//cout << ">> t1 flags" << t1_flagsint << endl;
		//cout << ">> polyfacebitmask" << polyfacebitmask << endl;
		EnclaveManifestOffset = SingleToMulti(EnclavePtr.Sorted.PolyArrayIndex[i]);			// set the offset values based on the xyz coords
		//cout << "Offset: " << EnclaveManifestOffset.x << endl;
		for (int j = 0; j < 6; ++j)
		{																	// iterate through each bit
			iteratorval = 0;
			if ((t1_flagsint & polyfacebitmask) == polyfacebitmask)			
			{
				//cout << "face will be rendered: " << j << " " << polyfacebitmask << endl;
				for (int k = 0; k < 6; ++k)		// 6 different vertexes = 1 face = 2 triangles = 18 floats
				{
					GL_x = 0.5f;
					GL_y = 0.5f;
					GL_z = 0.5f;
					// below 3 lines: GL_x = (x of vertex at structarray) + (enclave's unique x * 4) + (x offset)
					GL_x = GL_x * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].x) + ((EnclavePtr.UniqueKey.x) * 4) + ((EnclavePtr.CollectionKey.x) * 32) + EnclaveManifestOffset.x;			// multiply by x coord of vertex at structarray[...]	array index of: [(j*6) + some int] 
					GL_y = GL_y * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].y) + ((EnclavePtr.UniqueKey.y) * 4) + ((EnclavePtr.CollectionKey.y) * 32) + EnclaveManifestOffset.y;			// multiply by y coord of vertex at structarray[...]	array index of: [(j*6) + some int]
					GL_z = GL_z * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].z) + ((EnclavePtr.UniqueKey.z)*4) + ((EnclavePtr.CollectionKey.z) * 32) + EnclaveManifestOffset.z;			// multiply by z coord of vertex at structarray[...]	array index of: [(j*6) + some int]
					TempTuple.x = GL_x;
					TempTuple.y = GL_y;
					TempTuple.z = GL_z;

					EnclaveGLPtr[totaltuples++] = TempTuple.x;
					EnclaveGLPtr[totaltuples++] = TempTuple.y;
					EnclaveGLPtr[totaltuples++] = TempTuple.z;
					//EnclaveManifestRenderables[totaltuples++] = TempTuple;	

					// switch out 0 to appropriate value (insert incrementing value?)
					//cout << "{ " << TempTuple.x << ", " << TempTuple.y << ", " << TempTuple.z << " }" << endl;
					//cout << "test contents of actual array: " << EnclaveGLPtr[30] << endl;
					iteratorval++;
				}

			}
			polyfacebitmask >>= 1;													// move bit mask to the right one bit, after one iteration.
			//cout << "out test value " << polyfacebitmask <<  endl;
			//cout << "vertexes for this chunk so far: " << totaltuples << endl;
		}
	}

	if (IsRenderCollectionRefSet == 1)
	{
		//cout << "HOO AHHH" << endl;
		RenderCollectionRef->UpdateManifestArray(this->UniqueKey);
	}
	//delete[] EnclaveManifestRenderables;
	//delete[] EnclaveGLPtr;
}

EnclaveManifest::EnclaveManifestTuple EnclaveManifest::SingleToMulti(int input)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
	EnclaveManifest::EnclaveManifestTuple ReturnTuple;

	int x = input / 16;
	int remainder_x = input % 16;

	int y = remainder_x / 4;
	int remainder_y = remainder_x % 4;

	int z = remainder_y;

	//cout << x << " " << y << " " << z << " " << endl;

	ReturnTuple.x = x;
	ReturnTuple.y = y;
	ReturnTuple.z = z;

	return ReturnTuple;
}

int EnclaveManifest::ReturnEnclaveTriangles()
{
	/* Summary: returns the total number of triangles contained within the enclave */

	return TotalEnclaveTriangles;
}

EnclaveManifest::EnclaveManifest(int x, int y, int z)		// declares the enclave's position in the world space (x, y, z)
{
	/* Summary: constructor used when adding a manifest to a manifest collection */

	//cout << "CONSTRUCTOR CALLED " << endl;
	this->UniqueKey.x = x;
	this->UniqueKey.y = y;
	this->UniqueKey.z = z;
	//InitializeRenderArray();
	//TestTopLayer();
	//SortRenderArray();
}
;

EnclaveManifest::~EnclaveManifest()
{
	/* Summary: destructor called when its time for the enclave manifest to be destroyed; required for C array cleanup. */

	//cout << "destructor call.... (Enclave Manifest): " << IsEnclaveGLPtrLoaded << endl;
	if (IsEnclaveGLPtrLoaded == 1)
	{
		//cout << "delete flag entry in destructor..." << endl;
		delete[] EnclaveGLPtr;
		IsEnclaveGLPtrLoaded = 0;
	}
}

