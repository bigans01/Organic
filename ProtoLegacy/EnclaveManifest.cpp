#pragma once

using namespace std;

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <chrono>

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
		delete[] VertexColorGLPtr;
		delete[] TextureGLPtr;
	}

	IsEnclaveGLPtrLoaded = 1;
	cout << "Enclave Attach call..." << endl;
	EnclavePtr = in_ptr;													// set the pointer equal to the reference of the input parameter
	//cout << "Info for Enclave at: " << EnclavePtr.UniqueKey.x << ", " << EnclavePtr.UniqueKey.y << ", " << EnclavePtr.UniqueKey.z << " " << endl;
	//cout << "Total triangles from attached enclave:" << EnclavePtr->GetTotalTrianglesInEnclave() << endl;


	//TextureDictionary.Dictionary[mapname].Index[1] = tempMeta;		
	//OrganicTextureMeta *tempMetaRef = &TextureDictionary.Dictionary[mapname].Index[1];		// set up a reference to the new texture data for the block
	//tempMetaRef->BlockData.FaceIndex[0].FaceData[0].U = 2;
	//Organic.AddAndMaterializeCollection(0, 0, 0);

	OrganicTextureMeta *textureMetaRef;
	//OrganicTextureMeta textureMetaCopy;
	OrganicTextureMetaArray *textureMetaArrayRef;
	textureMetaArrayRef = &TextureDictionaryRef->Dictionary["base"];

	OrganicVtxColorMeta *vertexColorMetaRef;
	OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
	vertexColorMetaArrayRef = &VertexColorDictionaryRef->Dictionary["base"];

	//auto orgstart = std::chrono::high_resolution_clock::now();
	//int testval2 = TextureDictionaryRef->Dictionary["base"].Index[1].BlockData.FaceIndex[0].FaceData[0].U;
	//auto orgend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> orgelapsed = orgend - orgstart;
	//std::cout << "Elapsed time (Organic collection instantiation): " << orgelapsed.count() << endl;
	//cout << "testing of new Texture data: " << testval2 << endl;

	//EnclaveManifestRenderables = new EnclaveManifest::EnclaveManifestTuple[EnclavePtr->GetTotalTrianglesInEnclave()];				// FIX THIS!
	//auto orgstart = std::chrono::high_resolution_clock::now();
	EnclaveGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave())*9];
	VertexColorGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave()) * 9];
	TextureGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave()) * 6];						// new array would be GetTotalTrianglesInEnclave*6 (a pair of UV coordinates per vertex)
	//auto orgend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> orgelapsed = orgend - orgstart;
	//std::cout << "Elapsed time (2x dynamic array allocation time): " << orgelapsed.count() << endl;
	TotalEnclaveTriangles = EnclavePtr.GetTotalTrianglesInEnclave();
	//cout << "test: TotalEnclaveTriangles ->" << TotalEnclaveTriangles << endl;
	RenderablePolyCount = EnclavePtr.TotalRenderable;						// don't perform unnecessary loops through all 64 elements, only go x times (where x is number of polygons to render)

	//polyfacebitmask = 32;															// set bitmask to 32 initially.
	//cout << "out test initial " << polyfacebitmask << "; renderable poly count = " << RenderablePolyCount << endl;							// testing

	char testval[36] = { 0, 1, 2, 1, 2, 3,								// negative x			(WEST)		(32)	// this array could be constant?	
						 1, 5, 3, 5, 3, 7,								// negative z			(NORTH)		(16)	// OLD:  1, 5, 3, 5, 3, 7,
						 5, 4, 7, 4, 7, 6,								// positive x			(EAST)		(8)
						 4, 0, 6, 0, 6, 2,								// positive z			(SOUTH)		(4)		// OLD: 0, 4, 6, 0, 6, 2,
						 1, 5, 0, 5, 0, 4,								// positive y			(TOP)		(2)
						 3, 7, 2, 7, 2, 6								// negative y			(BOTTOM)	(1)
						};												// also for testing
	GLfloat GL_x = 0.5f;		// instantiate within stack frame
	GLfloat GL_y = 0.5f;
	GLfloat GL_z = 0.5f;
	int iteratorval, totaltuples = 0, texturetuples = 0, colorindex = 0;														

	for (int i = 0; i < RenderablePolyCount; ++i)
	{
		EnclavePolyArrayPtr = EnclavePtr.Sorted.RenderArray[i];			// point to the Enclave's sorted poly array 
		//cout << "debug: RenderablePolyCount before crash: " << RenderablePolyCount << endl;
		int t1_flagsint = EnclavePolyArrayPtr->t1_flags;			
		polyfacebitmask = 32;
		//cout << ">> t1 flags" << t1_flagsint << endl;
		//cout << ">> polyfacebitmask" << polyfacebitmask << endl;

		//textureMetaCopy = TextureDictionaryRef->Dictionary["base"].Index[1];
		//textureMetaRef = &TextureDictionaryRef->Dictionary["base"].Index[1];
		//textureMetaArrayRef = &TextureDictionaryRef->Dictionary["base"];
		textureMetaRef = &textureMetaArrayRef->Index[1];
		vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

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

					TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
					TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
					//EnclaveManifestRenderables[totaltuples++] = TempTuple;	

					VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].red;
					VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].green;
					VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].blue;

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

void EnclaveManifest::AttachToEnclave(Enclave &in_ptr, mutex& heapmutex)
{
	EnclavePtr = in_ptr;
	if (EnclavePtr.GetTotalTrianglesInEnclave() != 0)
	{
		/* Summary: Attaches to an existing, valid instance of an Enclave. 3d point data is generated based on the contents of the Enclave. */
		
		if (IsEnclaveGLPtrLoaded == 1)						// check to see if it was loaded already, on a previous call
		{
			heapmutex.lock();
			delete[] EnclaveGLPtr;							// delete old array
			delete[] VertexColorGLPtr;
			delete[] TextureGLPtr;
			heapmutex.unlock();
		}

		IsEnclaveGLPtrLoaded = 1;
		//cout << "Enclave Attach call..." << endl;
		//EnclavePtr = in_ptr;													// set the pointer equal to the reference of the input parameter
																				//cout << "Info for Enclave at: " << EnclavePtr.UniqueKey.x << ", " << EnclavePtr.UniqueKey.y << ", " << EnclavePtr.UniqueKey.z << " " << endl;
																				//cout << "Total triangles from attached enclave:" << EnclavePtr->GetTotalTrianglesInEnclave() << endl;


																				//TextureDictionary.Dictionary[mapname].Index[1] = tempMeta;		
																				//OrganicTextureMeta *tempMetaRef = &TextureDictionary.Dictionary[mapname].Index[1];		// set up a reference to the new texture data for the block
																				//tempMetaRef->BlockData.FaceIndex[0].FaceData[0].U = 2;
																				//Organic.AddAndMaterializeCollection(0, 0, 0);
		//cout << "pre-triangle check test 1 " << endl;
		//cout << "before dictionary..." << endl;
		OrganicTextureMeta *textureMetaRef;
		//OrganicTextureMeta textureMetaCopy;
		OrganicTextureMetaArray *textureMetaArrayRef;
		//cout << "pre-triangle check test 2 " << endl;
		//cout << "dumbval test: " << &TextureDictionaryRef->dumbval << endl;
		textureMetaArrayRef = &TextureDictionaryRef->Dictionary["base"];
		OrganicVtxColorMeta *vertexColorMetaRef;
		OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
		vertexColorMetaArrayRef = &VertexColorDictionaryRef->Dictionary["base"];
		//cout << "pre-triangle check test 3 " << endl;
		//auto orgstart = std::chrono::high_resolution_clock::now();
		//int testval2 = TextureDictionaryRef->Dictionary["base"].Index[1].BlockData.FaceIndex[0].FaceData[0].U;
		//auto orgend = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> orgelapsed = orgend - orgstart;
		//std::cout << "Elapsed time (Organic collection instantiation): " << orgelapsed.count() << endl;
		//cout << "testing of new Texture data: " << testval2 << endl;
		
		if (EnclavePtr.GetTotalTrianglesInEnclave() == 0)
		{
			cout << "Enclave location: " << EnclavePtr.UniqueKey.x << ", " << EnclavePtr.UniqueKey.y << ", " << EnclavePtr.UniqueKey.z << " : " << IsEnclaveGLPtrLoaded << endl;
			cout << "POTENTIAL ERROR!" << endl;
		}
		//EnclaveManifestRenderables = new EnclaveManifest::EnclaveManifestTuple[EnclavePtr->GetTotalTrianglesInEnclave()];				// FIX THIS!
		//auto orgstart = std::chrono::high_resolution_clock::now();
		heapmutex.lock();
		EnclaveGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave()) * 9];
		VertexColorGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave()) * 9];
		TextureGLPtr = new GLfloat[(EnclavePtr.GetTotalTrianglesInEnclave()) * 6];						// new array would be GetTotalTrianglesInEnclave*6 (a pair of UV coordinates per vertex)
		heapmutex.unlock();																								//auto orgend = std::chrono::high_resolution_clock::now();
																										//std::chrono::duration<double> orgelapsed = orgend - orgstart;
		EnclaveGLPtr[0] = 1;																				//std::cout << "Elapsed time (2x dynamic array allocation time): " << orgelapsed.count() << endl;
			//cout << "after new (2)..." << endl;
		TotalEnclaveTriangles = EnclavePtr.GetTotalTrianglesInEnclave();
		//cout << "test: TotalEnclaveTriangles ->" << TotalEnclaveTriangles << endl;
		RenderablePolyCount = EnclavePtr.TotalRenderable;						// don't perform unnecessary loops through all 64 elements, only go x times (where x is number of polygons to render)

																				//polyfacebitmask = 32;															// set bitmask to 32 initially.
																				//cout << "out test initial " << polyfacebitmask << "; renderable poly count = " << RenderablePolyCount << endl;							// testing

		char testval[36] = { 0, 1, 2, 1, 2, 3,								// negative x			(WEST)		(32)	// this array could be constant?		
			1, 5, 3, 5, 3, 7,								// negative z			(NORTH)		(16)	// OLD:  1, 5, 3, 5, 3, 7,
			5, 4, 7, 4, 7, 6,								// positive x			(EAST)		(8)
			4, 0, 6, 0, 6, 2,								// positive z			(SOUTH)		(4)		// OLD: 0, 4, 6, 0, 6, 2,
			1, 5, 0, 5, 0, 4,								// positive y			(TOP)		(2)
			3, 7, 2, 7, 2, 6								// negative y			(BOTTOM)	(1)
		};										// also for testing
		GLfloat GL_x = 0.5f;		// instantiate within stack frame
		GLfloat GL_y = 0.5f;
		GLfloat GL_z = 0.5f;
		int iteratorval, totaltuples = 0, texturetuples = 0, colorindex = 0;
		//cout << "after new... (2)" << endl;
		for (int i = 0; i < RenderablePolyCount; ++i)
		{
			EnclavePolyArrayPtr = EnclavePtr.Sorted.RenderArray[i];			// point to the Enclave's sorted poly array 
																			//cout << "debug: RenderablePolyCount before crash: " << RenderablePolyCount << endl;
			int t1_flagsint = EnclavePolyArrayPtr->t1_flags;
			polyfacebitmask = 32;
			//cout << ">> t1 flags" << t1_flagsint << endl;
			//cout << ">> polyfacebitmask" << polyfacebitmask << endl;

			//textureMetaCopy = TextureDictionaryRef->Dictionary["base"].Index[1];
			//textureMetaRef = &TextureDictionaryRef->Dictionary["base"].Index[1];
			//textureMetaArrayRef = &TextureDictionaryRef->Dictionary["base"];
			textureMetaRef = &textureMetaArrayRef->Index[1];
			vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];
			

			

			EnclaveManifestOffset = SingleToMulti(EnclavePtr.Sorted.PolyArrayIndex[i]);			// set the offset values based on the xyz coords
																								//cout << "Offset: " << EnclaveManifestOffset.x << endl;
			for (int j = 0; j < 6; ++j)
			{																	// iterate through each bit
				iteratorval = 0;
				//cout << "face will be rendered: " << j << " " << polyfacebitmask << endl;
				if ((t1_flagsint & polyfacebitmask) == polyfacebitmask)
				{

					for (int k = 0; k < 6; ++k)		// 6 different vertexes = 1 face = 2 triangles = 18 floats
					{
						GL_x = 0.5f;
						GL_y = 0.5f;
						GL_z = 0.5f;
						// below 3 lines: GL_x = (x of vertex at structarray) + (enclave's unique x * 4) + (x offset)
						GL_x = GL_x * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].x) + ((EnclavePtr.UniqueKey.x) * 4) + ((EnclavePtr.CollectionKey.x) * 32) + EnclaveManifestOffset.x;			// multiply by x coord of vertex at structarray[...]	array index of: [(j*6) + some int] 
						GL_y = GL_y * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].y) + ((EnclavePtr.UniqueKey.y) * 4) + ((EnclavePtr.CollectionKey.y) * 32) + EnclaveManifestOffset.y;			// multiply by y coord of vertex at structarray[...]	array index of: [(j*6) + some int]
						GL_z = GL_z * (EnclavePtr.Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].z) + ((EnclavePtr.UniqueKey.z) * 4) + ((EnclavePtr.CollectionKey.z) * 32) + EnclaveManifestOffset.z;			// multiply by z coord of vertex at structarray[...]	array index of: [(j*6) + some int]
						TempTuple.x = GL_x;
						TempTuple.y = GL_y;
						TempTuple.z = GL_z;
						//cout << "cool;" << GL_x << ", " << GL_y << "," << GL_z << ":" << texturetuples << endl;
						EnclaveGLPtr[totaltuples++] = TempTuple.x;			// EnclaveGLPtr[totaltuples++] = TempTuple.x;
						EnclaveGLPtr[totaltuples++] = TempTuple.y;			//EnclaveGLPtr[totaltuples++] = TempTuple.y;
						EnclaveGLPtr[totaltuples++] = TempTuple.z;			//EnclaveGLPtr[totaltuples++] = TempTuple.z;
						if (totaltuples > ((EnclavePtr.GetTotalTrianglesInEnclave()) * 9))
						{
							cout << "Enclave location: " << EnclavePtr.UniqueKey.x << ", " << EnclavePtr.UniqueKey.y << ", " << EnclavePtr.UniqueKey.z << " : " << IsEnclaveGLPtrLoaded << endl;
							cout << "DANGER!! Should be " << (EnclavePtr.GetTotalTrianglesInEnclave()) * 9 << ", but is " << totaltuples << endl;
						}
						TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
						TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
						//EnclaveManifestRenderables[totaltuples++] = TempTuple;	

						VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].red;
						VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].green;
						VertexColorGLPtr[colorindex++] = vertexColorMetaRef->BlockData.FaceIndex[j].FaceMeta[k].blue;

						// switch out 0 to appropriate value (insert incrementing value?)
						if (EnclavePtr.CollectionKey.x == 0 && EnclavePtr.CollectionKey.y == 0 && EnclavePtr.CollectionKey.z == 0)
						{
							if (EnclavePtr.UniqueKey.x == 3 && EnclavePtr.UniqueKey.y == 1 && EnclavePtr.UniqueKey.z == 0)
							{
								//cout << "{ " << TempTuple.x << ", " << TempTuple.y << ", " << TempTuple.z << " }" << endl;
								//cout << "||||||||||||||||||||test" << endl;
							}
						}
					
						//cout << "{ " << TempTuple.x << ", " << TempTuple.y << ", " << TempTuple.z << " }" << endl;
						//cout << "test contents of actual array: " << EnclaveGLPtr[30] << endl
						iteratorval++;

					}

				}
				polyfacebitmask >>= 1;													// move bit mask to the right one bit, after one iteration.
																						//cout << "out test value " << polyfacebitmask <<  endl;
																						//cout << "vertexes for this chunk so far: " << totaltuples << endl;
			}
		}
		//cout << "after new... (3)" << endl;
		if (IsRenderCollectionRefSet == 1)
		{
			//cout << "HOO AHHH" << endl;
			//cout << "after new... (4)" << endl;
			RenderCollectionRef->UpdateManifestArray(this->UniqueKey);
		}
		//delete[] EnclaveManifestRenderables;
		//delete[] EnclaveGLPtr;
		/*
		if (EnclavePtr.CollectionKey.x == 0 && EnclavePtr.CollectionKey.y == 0 && EnclavePtr.CollectionKey.z == 0)
		{
			if (EnclavePtr.UniqueKey.x == 3 && EnclavePtr.UniqueKey.y == 1 && EnclavePtr.UniqueKey.z == 0)
			{
				cout << "total triangles in manifest (debug): " << TotalEnclaveTriangles << endl;
				cout << "total triangles in target enclave (debug): " << EnclavePtr.GetTotalTrianglesInEnclave() << endl;
				//cout << "||||||||||||||||||||test" << endl;
			}
		}
		*/
	}
}

FloatTupleXYZ EnclaveManifest::SingleToMulti(int input)
{
	/* Summary: takes a single value between 0 to 63, and returns the x/y/z of the block within the chunk */

	// int multi_to_single = (x * 16) + (y * 4) + z;				// convert from 3d array coords to single array
	FloatTupleXYZ ReturnTuple;

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

EnclaveManifest::EnclaveManifest(int x, int y, int z, OrganicTextureDictionary *texturedictionaryptr, OrganicVtxColorDictionary *vertexcolordictionaryptr)
{
	this->UniqueKey.x = x;
	this->UniqueKey.y = y;
	this->UniqueKey.z = z;
	this->TextureDictionaryRef = texturedictionaryptr;
	this->VertexColorDictionaryRef = vertexcolordictionaryptr;
}

EnclaveManifest::~EnclaveManifest()
{
	/* Summary: destructor called when its time for the enclave manifest to be destroyed; required for C array cleanup. */

	//cout << "destructor call.... (Enclave Manifest): " << IsEnclaveGLPtrLoaded << endl;
	if (IsEnclaveGLPtrLoaded == 1)
	{
		//cout << "delete flag entry in destructor..." << endl;
		delete[] EnclaveGLPtr;
		delete[] VertexColorGLPtr;
		delete[] TextureGLPtr;
		IsEnclaveGLPtrLoaded = 0;
	}
}

