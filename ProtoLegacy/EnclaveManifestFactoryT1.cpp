#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <EnclaveManifestFactoryT1.h>

void EnclaveManifestFactoryT1::AttachManifestToEnclave(Enclave *in_ptr)
{
	OrganicTextureMeta *textureMetaRef;
	//OrganicTextureMeta textureMetaCopy;
	OrganicTextureMetaArray *textureMetaArrayRef;
	textureMetaArrayRef = &TextureDictionaryRef->Dictionary["base"];

	OrganicVtxColorMeta *vertexColorMetaRef;
	OrganicVtxColorMetaArray *vertexColorMetaArrayRef;
	vertexColorMetaArrayRef = &VertexColorDictionaryRef->Dictionary["base"];

	//StorageArray[CurrentStorage].VertexArrayCount =	(in_ptr->GetTotalTrianglesInEnclave()) * 9;	// set the total amount of vertex data to store
	//StorageArray[CurrentStorage].TextureArrayCount = (in_ptr->GetTotalTrianglesInEnclave()) * 6; // set the total amount of texture data to store
	StorageArray[CurrentStorage].StorageKey = in_ptr->UniqueKey;
	StorageArray[CurrentStorage].VertexArrayCount = 0;			// set the total amount of vertex data to store
	StorageArray[CurrentStorage].TextureArrayCount = 0;			// set the total amount of texture data to store
	StorageArray[CurrentStorage].VertexColorArrayCount = 0;		// set the total amount of vertex color data to store
	StorageArray[CurrentStorage].TotalEnclaveTriangles = in_ptr->GetTotalTrianglesInEnclave();	// total triangles in the attached enclave (saved for later use)
	int RenderablePolyCount = in_ptr->TotalRenderable;

	char testval[36] = {	0, 1, 2, 1, 2, 3,								// negative x			(WEST)		(32)	// this array could be constant?		
							4, 0, 6, 0, 6, 2,								// negative z			(NORTH)		(16)	// OLD:  1, 5, 3, 5, 3, 7,
							5, 4, 7, 4, 7, 6,								// positive x			(EAST)		(8)
							1, 5, 3, 5, 3, 7,								// positive z			(SOUTH)		(4)		// OLD: 0, 4, 6, 0, 6, 2,
							1, 5, 0, 5, 0, 4,								// positive y			(TOP)		(2)
							3, 7, 2, 7, 2, 6								// negative y			(BOTTOM)	(1)
	};

	GLfloat GL_x = 0.5f;		// instantiate within stack frame
	GLfloat GL_y = 0.5f;
	GLfloat GL_z = 0.5f;
	int iteratorval, totaltuples = 0, texturetuples = 0, somedumbval = 0;

	for (int i = 0; i < RenderablePolyCount; ++i)
	{
		EnclavePolyArrayPtr = in_ptr->Sorted.RenderArray[i];
		int t1_flagsint = EnclavePolyArrayPtr->t1_flags;
		polyfacebitmask = 32;

		textureMetaRef = &textureMetaArrayRef->Index[1];
		vertexColorMetaRef = &vertexColorMetaArrayRef->Index[1];

		EnclaveManifestOffset = SingleToMulti(in_ptr->Sorted.PolyArrayIndex[i]);
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
					GL_x = GL_x * (in_ptr->Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].x) + ((in_ptr->UniqueKey.x) * 4) + ((in_ptr->CollectionKey.x) * 32) + EnclaveManifestOffset.x;			// multiply by x coord of vertex at structarray[...]	array index of: [(j*6) + some int] 
					GL_y = GL_y * (in_ptr->Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].y) + ((in_ptr->UniqueKey.y) * 4) + ((in_ptr->CollectionKey.y) * 32) + EnclaveManifestOffset.y;			// multiply by y coord of vertex at structarray[...]	array index of: [(j*6) + some int]
					GL_z = GL_z * (in_ptr->Sorted.RenderArray[i]->structarray[testval[(j * 6) + iteratorval]].z) + ((in_ptr->UniqueKey.z) * 4) + ((in_ptr->CollectionKey.z) * 32) + EnclaveManifestOffset.z;			// multiply by z coord of vertex at structarray[...]	array index of: [(j*6) + some int]
					TempTuple.x = GL_x;
					TempTuple.y = GL_y;
					TempTuple.z = GL_z;

					StorageArray[CurrentStorage].VertexArray[StorageArray[CurrentStorage].VertexArrayCount++] = TempTuple.x;
					StorageArray[CurrentStorage].VertexArray[StorageArray[CurrentStorage].VertexArrayCount++] = TempTuple.y;
					StorageArray[CurrentStorage].VertexArray[StorageArray[CurrentStorage].VertexArrayCount++] = TempTuple.z;
					//EnclaveGLPtr[totaltuples++] = TempTuple.x;
					//EnclaveGLPtr[totaltuples++] = TempTuple.y;
					//EnclaveGLPtr[totaltuples++] = TempTuple.z;

					StorageArray[CurrentStorage].TextureArray[StorageArray[CurrentStorage].TextureArrayCount++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
					StorageArray[CurrentStorage].TextureArray[StorageArray[CurrentStorage].TextureArrayCount++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;

					StorageArray[CurrentStorage].VertexColorArray[StorageArray[CurrentStorage].VertexColorArrayCount++] = vertexColorMetaRef->BlockData.FaceIndex[0].FaceMeta[k].red;
					StorageArray[CurrentStorage].VertexColorArray[StorageArray[CurrentStorage].VertexColorArrayCount++] = vertexColorMetaRef->BlockData.FaceIndex[0].FaceMeta[k].green;
					StorageArray[CurrentStorage].VertexColorArray[StorageArray[CurrentStorage].VertexColorArrayCount++] = vertexColorMetaRef->BlockData.FaceIndex[0].FaceMeta[k].blue;
					//TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
					//TextureGLPtr[texturetuples++] = textureMetaRef->BlockData.FaceIndex[0].FaceData[0].U;
					//EnclaveManifestRenderables[totaltuples++] = TempTuple;	

					// switch out 0 to appropriate value (insert incrementing value?)
					//cout << "{ " << TempTuple.x << ", " << TempTuple.y << ", " << TempTuple.z << " }" << endl;
					//cout << "test contents of actual array: " << EnclaveGLPtr[30] << endl;
					iteratorval++;
					somedumbval++;
				}
				//cout << "VertexArrayCount: " << StorageArray[CurrentStorage].VertexArrayCount << endl;
			}
			polyfacebitmask >>= 1;													// move bit mask to the right one bit, after one iteration.
																					//cout << "out test value " << polyfacebitmask <<  endl;
																					//cout << "vertexes for this chunk so far: " << totaltuples << endl;
		}
	}
	CurrentStorage++;
	StorageArrayCount++;
	//cout << "VertexArrayCount: " << StorageArray[CurrentStorage].VertexArrayCount << endl;
	//cout << "VertexArrayCount: " << somedumbval << endl;
}

void EnclaveManifestFactoryT1::SetEnclaveCollectionPtr(EnclaveCollection *InPtr)
{

}

EnclaveManifestFactoryT1::EnclaveManifestTuple EnclaveManifestFactoryT1::SingleToMulti(int input) //EnclaveManifest::EnclaveManifestTuple
{
	EnclaveManifestFactoryT1::EnclaveManifestTuple ReturnTuple;

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