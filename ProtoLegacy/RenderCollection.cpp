#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "RenderCollection.h"
#include <unordered_map>
#include "EnclaveKeyDef.h"
#include <GL/glew.h>

void RenderCollection::SetManifestCollectionPtr(ManifestCollection *manifestcollectionref)
{
	/* Summary: sets the value of ManifestCollectionPtr to a valid reference of a ManifestCollection*/
	ManifestCollectionPtr = manifestcollectionref;
}

void RenderCollection::SetEnclaveCollectionPtr(EnclaveCollection *enclavecollectionref)
{
	/* Summary: sets the value of EnclaveCollectionPtr to a valid reference of a EnclaveCollection*/
	EnclaveCollectionPtr = enclavecollectionref;
}

void RenderCollection::CombineManifestArrays()
{
	/* Summary: creates a dynamic array by reading all EnclaveManifests found in a particular ManifestCollection. */




	//if (IsGLFloatPtrLoaded == 1)		// delete old array if it was previously set
	//{
	//delete[] GLFloatPtr;
	//delete[] VertexColorArrayPtr;
	//}
	//IsGLFloatPtrLoaded = 1;

	// 1))) First iterator pass: find number of manifests to attach to, create a temp array for this
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher>::iterator ManMatrixIter;
	ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	int totaltrianglestorender = 0;
	int totalenclavesfound = 0;
	RenderableManifestMeta.EnclaveManifestCount = 0;

	auto teststart1 = std::chrono::high_resolution_clock::now();



	auto start3 = std::chrono::high_resolution_clock::now();
	for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	{
		//cout << "test count = " << testcount++ << endl;
		totaltrianglestorender += ManMatrixIter->second.TotalEnclaveTriangles;

		ManMatrixIter->second.RenderCollectionRef = this;		// send the enclave manifest a pointer to this render collection, so that it may send updates to the render collection
		ManMatrixIter->second.IsRenderCollectionRefSet = 1;		// set this flag to 1, so that the enclave manifest itself knows that RenderCollectionRef is set

		RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].EnclaveManifestKey = ManMatrixIter->second.UniqueKey;
		RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].currentTriangleCount = ManMatrixIter->second.TotalEnclaveTriangles;
		RenderableManifestMeta.CollectionTriangleCount += RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].currentTriangleCount;
		RenderableManifestMeta.EnclaveManifestCount++;

		//cout << "enclave found! (" << totaltrianglestorender << ")" <<  endl;
		//cout << "key of found enclave: " << ManMatrixIter->second.UniqueKey.x << ", " << ManMatrixIter->second.UniqueKey.y << ", " << ManMatrixIter->second.UniqueKey.z << endl;
		totalenclavesfound++;
	}

	auto finish3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	//std::cout << "Render Collection test, step 1 " << elapsed3.count() << "): "  << endl;
	//EnclaveKeyDef::EnclaveKey *tempManifestKeys = new EnclaveKeyDef::EnclaveKey[totalenclavesfound];







	// 2))) create the new array, based on length from previous step.
	//ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	//int beginindex = 0;
	//auto start4 = std::chrono::high_resolution_clock::now();
	//for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	//{
	//tempManifestKeys[beginindex] = ManMatrixIter->second.UniqueKey;
	//totaltrianglestorender += ManMatrixIter->second.TotalEnclaveTriangles;
	//cout << "enclave found! (" << totaltrianglestorender << ")" <<  endl;
	//totalenclavesfound++;
	//beginindex++;
	//}
	//auto finish4 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed4 = finish4 - start4;
	//std::cout << "Render Collection test, step 2 " << elapsed4.count() << "): " <<  endl;
	//GLfloat *glfloatptr;


	//GLFloatPtr = new GLfloat[totaltrianglestorender*9];	// 9 floats per triangle; stores vertex data
	//VertexColorArrayPtr = new GLfloat[totaltrianglestorender * 9];

	//GLFloatPtr = make_unique<GLfloat[]>(totaltrianglestorender * 9);	// 9 floats per triangle; stores vertex data
	//VertexColorArrayPtr = make_unique<GLfloat[]>(totaltrianglestorender * 9);

	GLFloatPtr.reset(new GLfloat[totaltrianglestorender * 9]);
	VertexColorArrayPtr.reset(new GLfloat[totaltrianglestorender * 9]);

	RenderCollectionArraySize = totaltrianglestorender * 4 * 9;
	//cout << "value of RenderCollectionArraySize: " << RenderCollectionArraySize << endl;
	int index = 0;


	// 3))) Populate array
	ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	int currentBegin = 0;
	auto start5 = std::chrono::high_resolution_clock::now();
	for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	{


		//GLfloat *tempGLptr;																// temp pointer to the vertex data array
		//tempGLptr = ManMatrixIter->second.EnclaveGLPtr;									// set tempGLptr equivalent to the EnclaveGLPtr in the currently found manifest

		//GLfloat *tempGLColorPtr;
		//tempGLColorPtr = ManMatrixIter->second.VertexColorGLPtr;

		int pointedBegin = 0;
		for (int bb = 0; bb < (ManMatrixIter->second.TotalEnclaveTriangles) * 3; bb++)
		{

			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin + 2];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin - 2 ] = tempGLptr[pointedBegin];

			GLFloatPtr[currentBegin + 2] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 2];									// for first coord, x
			GLFloatPtr[currentBegin + 1] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 1];									// for first coord, x
			GLFloatPtr[currentBegin] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin];

			VertexColorArrayPtr[currentBegin + 2] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 2];
			VertexColorArrayPtr[currentBegin + 1] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 1];
			VertexColorArrayPtr[currentBegin] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin];

			// NOTE: current begin should be 0...check this!
			// currentBegin = end index of currently checked enclave manifest....
			//GLFloatPtr[currentBegin - 2] = tempGLptr[pointedBegin + 2];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin];


			//GLFloatPtr[currentBegin - 2] = tempGLptr[pointedBegin];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin + 2];

			//GLFloatPtr[currentBegin - 2] = tempGLptr[currentBegin];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[currentBegin - 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[currentBegin - 2];									// for first coord, x
			//cout << "Triangle coord matching: (-2): " << GLFloatPtr[currentBegin - 2] << " (-1): " << GLFloatPtr[currentBegin - 1] << " (0):" << GLFloatPtr[currentBegin] << endl;
			//currentBegin -= 3;
			currentBegin += 3;
			pointedBegin += 3;

		}


	}
	auto finish5 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed5 = finish5 - start5;
	//std::cout << "Render Collection test, step 2 " << elapsed5.count() << "): "  << endl;
	//std::cout << "total enclaves found: " << totalenclavesfound << endl;
	/*int index2 = 0;
	for (int z = 0; z < totaltrianglestorender; z++)
	{
	for (int aa = 0; aa < 3; aa++)
	{
	cout << "Triangle ["<< z << "]: Point ["<< aa << "] "<< GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << endl;
	}
	}*/



}

void RenderCollection::CombineManifestArrays(mutex& mutexval)
{
	//cout << "RenderCollection, manifest arrays combine renderMode value is: " << currentRenderMode << endl;
	// 1))) First iterator pass: find number of manifests to attach to, create a temp array for this
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveManifest, EnclaveKeyDef::KeyHasher>::iterator ManMatrixIter;
	ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	int totaltrianglestorender = 0;
	int totalenclavesfound = 0;
	RenderableManifestMeta.EnclaveManifestCount = 0;

	auto teststart1 = std::chrono::high_resolution_clock::now();



	auto start3 = std::chrono::high_resolution_clock::now();
	for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	{
		//cout << "test count = " << testcount++ << endl;
		totaltrianglestorender += ManMatrixIter->second.TotalEnclaveTriangles;

		ManMatrixIter->second.RenderCollectionRef = this;		// send the enclave manifest a pointer to this render collection, so that it may send updates to the render collection
		ManMatrixIter->second.IsRenderCollectionRefSet = 1;		// set this flag to 1, so that the enclave manifest itself knows that RenderCollectionRef is set

		RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].EnclaveManifestKey = ManMatrixIter->second.UniqueKey;
		RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].currentTriangleCount = ManMatrixIter->second.TotalEnclaveTriangles;
		RenderableManifestMeta.CollectionTriangleCount += RenderableManifestMeta.MetaArray[RenderableManifestMeta.EnclaveManifestCount].currentTriangleCount;
		RenderableManifestMeta.EnclaveManifestCount++;

		//cout << "key of found enclave: " << ManMatrixIter->second.UniqueKey.x << ", " << ManMatrixIter->second.UniqueKey.y << ", " << ManMatrixIter->second.UniqueKey.z << endl;
		totalenclavesfound++;
	}

	auto finish3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	//std::cout << "Render Collection test, step 1 " << elapsed3.count() << "): "  << endl;
	//EnclaveKeyDef::EnclaveKey *tempManifestKeys = new EnclaveKeyDef::EnclaveKey[totalenclavesfound];
	//unique_ptr<EnclaveKeyDef::EnclaveKey[]> tempManifestKeys = make_unique<EnclaveKeyDef::EnclaveKey[]>(totalenclavesfound);






	// 2))) create the new array, based on length from previous step.
	//ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	//int beginindex = 0;
	//auto start4 = std::chrono::high_resolution_clock::now();
	//for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	//{
	//tempManifestKeys[beginindex] = ManMatrixIter->second.UniqueKey;
	//totaltrianglestorender += ManMatrixIter->second.TotalEnclaveTriangles;
	//cout << "enclave found! (" << totaltrianglestorender << ")" <<  endl;
	//totalenclavesfound++;
	//beginindex++;
	//}
	//auto finish4 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed4 = finish4 - start4;
	//std::cout << "Render Collection test, step 2 " << elapsed4.count() << "): " <<  endl;
	//GLfloat *glfloatptr;

	/*
	AllocateNewArraysViaLockGuard(totaltrianglestorender * 9, std::ref(mutexval));
	RenderCollectionArraySize = totaltrianglestorender * 4 * 9;
	*/
	
	// NEW PROTOTYPE TESTING -- pass appropriate array size to AllocateNewArraysViaLockGuard
	if (currentRenderMode == 0)
	{
		AllocateNewArraysViaLockGuard((totaltrianglestorender * 9), std::ref(mutexval));
		RenderCollectionArraySize = (totaltrianglestorender * 9) * 4;
	}

	if (currentRenderMode == 1)
	{
		AllocateNewArraysViaLockGuard((totaltrianglestorender * 9)*2, std::ref(mutexval));
		RenderCollectionArraySize = (totaltrianglestorender * 9 * 2) * 4;
	}
	

	//cout << "value of RenderCollectionArraySize: " << RenderCollectionArraySize << endl;
	int index = 0;


	// 3))) Populate array
	ManMatrixIter = ManifestCollectionPtr->ManMatrix.begin();
	int currentBegin = 0;
	auto start5 = std::chrono::high_resolution_clock::now();
	for (ManMatrixIter; ManMatrixIter != ManifestCollectionPtr->ManMatrix.end(); ++ManMatrixIter)
	{


		//GLfloat *tempGLptr;																// temp pointer to the vertex data array
		//tempGLptr = ManMatrixIter->second.EnclaveGLPtr;									// set tempGLptr equivalent to the EnclaveGLPtr in the currently found manifest

		//GLfloat *tempGLColorPtr;
		//tempGLColorPtr = ManMatrixIter->second.VertexColorGLPtr;

		int pointedBegin = 0;
		for (int bb = 0; bb < (ManMatrixIter->second.TotalEnclaveTriangles) * 3; bb++)
		{

			/*

			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin + 2];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin - 2 ] = tempGLptr[pointedBegin];

			GLFloatPtr[currentBegin + 2] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 2];									// for first coord, x
			GLFloatPtr[currentBegin + 1] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 1];									// for first coord, x
			GLFloatPtr[currentBegin] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin];

			VertexColorArrayPtr[currentBegin + 2] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 2];
			VertexColorArrayPtr[currentBegin + 1] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 1];
			VertexColorArrayPtr[currentBegin] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin];

			// NOTE: current begin should be 0...check this!
			// currentBegin = end index of currently checked enclave manifest....
			//GLFloatPtr[currentBegin - 2] = tempGLptr[pointedBegin + 2];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin];


			//GLFloatPtr[currentBegin - 2] = tempGLptr[pointedBegin];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin + 2];

			//GLFloatPtr[currentBegin - 2] = tempGLptr[currentBegin];									// for first coord, x
			//GLFloatPtr[currentBegin - 1] = tempGLptr[currentBegin - 1];									// for first coord, x
			//GLFloatPtr[currentBegin] = tempGLptr[currentBegin - 2];									// for first coord, x
			//cout << "Triangle coord matching: (-2): " << GLFloatPtr[currentBegin - 2] << " (-1): " << GLFloatPtr[currentBegin - 1] << " (0):" << GLFloatPtr[currentBegin] << endl;
			//currentBegin -= 3;
			currentBegin += 3;
			pointedBegin += 3;

			*/
			// NEW PROTOTYPE TESTING -- create appropriate array size
			// Render mode 0 settings
			if (currentRenderMode == 0)
			{
				GLFloatPtr[currentBegin + 2] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 2];									// for first coord, x
				GLFloatPtr[currentBegin + 1] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 1];									// for first coord, x
				GLFloatPtr[currentBegin] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin];

				currentBegin += 3;		// iterating by 6 per VBO array element
				pointedBegin += 3;		// only iterating by 3 per manifest array
			}

			
			// Render mode 1 settings
			if (currentRenderMode == 1)
			{
				GLFloatPtr[currentBegin + 2] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 2];									// for first coord, x
				GLFloatPtr[currentBegin + 1] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin + 1];									// for first coord, x
				GLFloatPtr[currentBegin] = ManMatrixIter->second.EnclaveGLPtr[pointedBegin];

				GLFloatPtr[currentBegin + 5] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 2];
				GLFloatPtr[currentBegin + 4] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin + 1];
				GLFloatPtr[currentBegin + 3] = ManMatrixIter->second.VertexColorGLPtr[pointedBegin];

				currentBegin += 6;		// iterating by 6 per VBO array element
				pointedBegin += 3;		// only iterating by 3 per manifest array
			}
			

		}


	}
	auto finish5 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed5 = finish5 - start5;
	//std::cout << "Render Collection test, step 2 " << elapsed5.count() << "): "  << endl;
	//std::cout << "total enclaves found: " << totalenclavesfound << endl;
	/*int index2 = 0;
	for (int z = 0; z < totaltrianglestorender; z++)
	{
	for (int aa = 0; aa < 3; aa++)
	{
	cout << "Triangle ["<< z << "]: Point ["<< aa << "] "<< GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << endl;
	}
	}*/
}

void RenderCollection::AllocateNewArraysViaLockGuard(int in_totalSizeOfVertexFloats, mutex& mutexval)
{
	std::lock_guard<std::mutex> lock(mutexval);
	//cout << "GL float check begin...; total floats is: " << totalfloats << "Total Enclaves is: " << TotalEnclavesInFactory << endl;

	/*
	GLFloatPtr.reset(new GLfloat[in_totalSizeOfVertexFloats]);
	VertexColorArrayPtr.reset(new GLfloat[in_totalSizeOfVertexFloats]);
	*/
	// NEW PROTOTYPE TESTING:	Step 3: create appropriate array size

	//cout << "Testing: (vertex attribute total size) " << in_totalSizeOfVertexFloats << endl;
	//cout << "Testing: (vertex attribute total size) " << (in_totalSizeOfVertexFloats/3)*2 << endl;
	
	//if (currentRenderMode == 1)
	//{
		int modeVertexTupleWidth = in_totalSizeOfVertexFloats;		// in_totalSizeOfVertexFloats = 3 vertexes; multiply by 2 to get the total amount w/ vertex attrib for vertex color
		GLFloatPtr.reset(new GLfloat[modeVertexTupleWidth]);
	//}
	

}

void RenderCollection::CombineManifestArraysFromT1Factory(EnclaveManifestFactoryT1 *factoryRef, mutex& mutexval)
{
	//cout << "RenderCollection, factory storage combine renderMode value is: " << currentRenderMode << endl;
	int totalfloats = 0;												// the total number of floats that will need be stored.
	int totalenclavesfound = 0;
	int TotalEnclavesInFactory = factoryRef->StorageArrayCount;		// get the number of enclaves to iterate through
																	//cout << "total enclaves in factory will be: " << TotalEnclavesInFactory << endl;

																	// STEP 1: get the size of the array
																	//cout << "total enclaves in factory: " << TotalEnclavesInFactory << endl;

	for (int x = 0; x < TotalEnclavesInFactory; x++)
	{

		/*
		EnclaveManifestFactoryT1Storage *StoragePtr = &factoryRef->StorageArray[x];			// get the pointer to the storage unit
		totalfloats += StoragePtr->VertexArrayCount;										// increment totalfloats by VertexArrayCount from the pointed-to enclave
		*/

		if (currentRenderMode == 0)
		{
			EnclaveManifestFactoryT1Storage *StoragePtr = &factoryRef->StorageArray[x];
			totalfloats += StoragePtr->VertexArrayCount;
		}

		// NEW PROTOTYPE TESTING -- set totalfloats to appropriate value
		if (currentRenderMode == 1)
		{
			EnclaveManifestFactoryT1Storage *StoragePtr = &factoryRef->StorageArray[x];			// get the pointer to the storage unit
			totalfloats += StoragePtr->VertexArrayCount;
			totalfloats += StoragePtr->VertexColorArrayCount;
		}
		
	}
	//cout << "total triangles to render will be: " << totalfloats << endl;


	AllocateNewArraysViaLockGuard(totalfloats, std::ref(mutexval));			// new code from (11/2/2017)

																			

																			//cout << "Render 2 entry:" << endl;

																			// STEP 3: populate dynamic array(s)
	struct enclaveDataFinder
	{
		EnclaveKeyDef::EnclaveKey DFKey;
		int indexStart;
		int totalTriangles;
	};
	enclaveDataFinder enclaveDataStart[512];					// THIS WAS THE CAUSE OF THE ERROR ON (9/2/2017) ---> 512 was incorrect value of around 64. This caused corruption.
	int currentBegin = 0;
	for (int x = 0; x < TotalEnclavesInFactory; x++)
	{

		EnclaveManifestFactoryT1Storage *StoragePtr = &factoryRef->StorageArray[x];
		int currentEnclaveTriangles = (StoragePtr->VertexArrayCount) / 9;

		int pointedBegin = 0;
		GLfloat dumbval = StoragePtr->VertexArray[pointedBegin];
		GLfloat dumbval2 = StoragePtr->VertexColorArray[pointedBegin];
		//GLfloat dumbval2 = StoragePtr->VertexArray[pointedBegin+1];
		//GLfloat dumbval3 = StoragePtr->VertexArray[pointedBegin+2];
		//cout << "test  " << dumbval << ", " << dumbval2 << ", " << dumbval3 << endl;
		GLFloatPtr[currentBegin] = dumbval;
		//VertexColorArrayPtr[currentBegin] = dumbval2;
		//cout << "Render 2 entry:" << endl;
		//cout << "Key output: " << StoragePtr->StorageKey.x << ", " << StoragePtr->StorageKey.y << "," << StoragePtr->StorageKey.z << " | " << dumbval << endl ;
		enclaveDataStart[x].DFKey = StoragePtr->StorageKey;
		enclaveDataStart[x].indexStart = currentBegin;
		enclaveDataStart[x].totalTriangles = currentEnclaveTriangles;

		for (int y = 0; y < (currentEnclaveTriangles) * 3; y++)			// (currentEnclaveTriangles) * 3
		{

			/*
			GLFloatPtr[currentBegin] = StoragePtr->VertexArray[pointedBegin];
			GLFloatPtr[currentBegin + 1] = StoragePtr->VertexArray[pointedBegin + 1];
			GLFloatPtr[currentBegin + 2] = StoragePtr->VertexArray[pointedBegin + 2];

			VertexColorArrayPtr[currentBegin] =		StoragePtr->VertexColorArray[pointedBegin];
			VertexColorArrayPtr[currentBegin + 1] = StoragePtr->VertexColorArray[pointedBegin + 1];
			VertexColorArrayPtr[currentBegin + 2] = StoragePtr->VertexColorArray[pointedBegin + 2];

			currentBegin += 3;
			pointedBegin += 3;
			*/

			// NEW PROTOTYPE TESTING -- create appropriate array size

			// Render mode 0 settings
			if (currentRenderMode == 0)
			{
				GLFloatPtr[currentBegin] = StoragePtr->VertexArray[pointedBegin];
				GLFloatPtr[currentBegin + 1] = StoragePtr->VertexArray[pointedBegin + 1];
				GLFloatPtr[currentBegin + 2] = StoragePtr->VertexArray[pointedBegin + 2];
				currentBegin += 3;			// iterating by 6 per VBO array element
				pointedBegin += 3;			// only iterating by 3 per manifest array
			}

			// Render mode 1 settings
			if (currentRenderMode == 1)
			{
				GLFloatPtr[currentBegin] = StoragePtr->VertexArray[pointedBegin];
				GLFloatPtr[currentBegin + 1] = StoragePtr->VertexArray[pointedBegin + 1];
				GLFloatPtr[currentBegin + 2] = StoragePtr->VertexArray[pointedBegin + 2];

				GLFloatPtr[currentBegin + 3] = StoragePtr->VertexColorArray[pointedBegin];
				GLFloatPtr[currentBegin + 4] = StoragePtr->VertexColorArray[pointedBegin + 1];
				GLFloatPtr[currentBegin + 5] = StoragePtr->VertexColorArray[pointedBegin + 2];

				currentBegin += 6;			// iterating by 6 per VBO array element
				pointedBegin += 3;			// only iterating by 3 per manifest array
			}
			

		}


		EnclaveKeyDef::EnclaveKey scanKey;
		scanKey.x = 0;
		scanKey.y = 6;
		scanKey.z = 0;
		if (enclaveDataStart[x].DFKey == scanKey)
		{
			//cout << "key found! loading array data..." << endl;
			int tempBegin = enclaveDataStart[x].indexStart;
			for (int z = 0; z < (enclaveDataStart[x].totalTriangles) * 3; z++)
			{
				//cout << "vertex: " << GLFloatPtr[tempBegin] << ", " << GLFloatPtr[tempBegin + 1] << ", " << GLFloatPtr[tempBegin + 2] << endl;
				tempBegin += 3;
			}

		}

	}
	RenderCollectionArraySize = totalfloats * 4;
	//mutexval.unlock();

}


void RenderCollection::UpdateManifestArray(EnclaveKeyDef::EnclaveKey Key)	// update the key in RenderableManifestMeta.EnclaveManifestMeta
{
	/* Summary: creates a new dynamic array, but only updates a single enclave which has a value of Key */

	//cout << "Key loop test..." << endl;

	// STEP 1: find index of the Key that needs to be updated.
	int UpdateType = 0;
	int FoundIndex = 0;
	for (int x = 0; x < RenderableManifestMeta.EnclaveManifestCount; x++)
	{
		if (RenderableManifestMeta.MetaArray[x].EnclaveManifestKey == Key)
		{
			FoundIndex = x;
			//cout << "Key match!" << endl;
			//cout << "key of found enclave: " << ManMatrixIter->second.UniqueKey.x << ", " << ManMatrixIter->second.UniqueKey.y << ", " << ManMatrixIter->second.UniqueKey.z << endl;
			if (x == 0 || x == (RenderableManifestMeta.EnclaveManifestCount - 1))
			{
				//cout << "Reform type will be 0 (two arrays)" << endl;
			}
			else
			{
				//cout << "Reform type will be 1 (three arrays)" << endl;
				UpdateType = 1;
			}
		}
	}

	// STEP 2: set up arrays based on UpdateType
	if (UpdateType == 0)
	{

	}
	else   // UpdateType would be == 1 here
	{
		auto true_start = std::chrono::high_resolution_clock::now();

		// ASSEMBLE FIRST ARRAY
		auto start4 = std::chrono::high_resolution_clock::now();
		int ArrayOneLength = 0;
		int aa = 0;					// can use for second array
		do {
			ArrayOneLength += RenderableManifestMeta.MetaArray[aa].currentTriangleCount;
			aa++;
		} while (aa != FoundIndex);
		//cout << "Triangle count of first array will be: " << ArrayOneLength << endl;
		//GLfloat *tempGLptr;
		unique_ptr<GLfloat[]> tempGLptr(new GLfloat[ArrayOneLength * 9]);
		//GLfloat *tempGLptr00;
		//tempGLptr = new GLfloat[ArrayOneLength * 9];
		//tempGLptr00 = new GLfloat[ArrayOneLength * 9];

		auto test_start1 = std::chrono::high_resolution_clock::now();
		for (int bb = 0; bb < (ArrayOneLength * 9); bb++)
		{
			tempGLptr[bb] = GLFloatPtr[bb];
		}
		auto test_finish1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> test_elapsed1 = test_finish1 - test_start1;

		//cout << "Looped array copy  = " << test_elapsed1.count() << endl;
		//for (int t = 0; t < 30; ++t)
		//{
		//cout << "test data: " << tempGLptr[t] << endl;
		//}

		//auto test_start2 = std::chrono::high_resolution_clock::now();
		//std::copy(&*GLFloatPtr.get(), (&*GLFloatPtr.get()) + (ArrayOneLength * 9), &*tempGLptr00);
		//auto test_finish2 = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> test_elapsed2 = test_finish2 - test_start2;
		//cout << "True array copy  = " << test_elapsed2.count() << endl;						// IMPORTANT NOTE: this is FASTER when dealing with large ranges of arrays, SLOWER when dealing with smaller.
		//for (int t = 0; t < (ArrayOneLength * 9); t++) // ArrayOneLength * 9
		//{
		//cout << "test data2: " << tempGLptr00[t] << endl;
		//}
		//delete[] tempGLptr00;

		int index2 = 0;
		//EnclavePtr->UnveilSinglePolyWithMtrl(in_x, in_y, in_z, 0, in_otherflags, 0, 40, 0); // reveal the polygon to the world	
		auto finish4 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed4 = finish4 - start4;
		//cout << "Array 1 reassembly duration = " << elapsed4.count() << endl;

		// --------------------OUTPUT OF FIRST ARRAY
		/*index2 = 0;
		for (int z = 0; z < ArrayOneLength; z++)
		{
		for (int aa = 0; aa < 3; aa++)
		{
		//cout << "Array one Triangle ["<< z << "]: Point ["<< aa << "] "<< tempGLptr[index2++] << " " << tempGLptr[index2++] << " " << tempGLptr[index2++] << endl;
		}
		}*/

		// ASSEMBLE THIRD ARRAY
		auto start5 = std::chrono::high_resolution_clock::now();
		int ArrayThreeLength = 0;
		//int bb = 0;
		aa++;										// set to beginning of first element, past the current one that needs to be modified
		do {
			//cout << "test of third array loop" << endl;
			ArrayThreeLength += RenderableManifestMeta.MetaArray[aa].currentTriangleCount;
			aa++;
		} while (aa != RenderableManifestMeta.EnclaveManifestCount);

		//GLfloat *tempGLptr3;

		int startIndex = (ArrayOneLength * 9) + (RenderableManifestMeta.MetaArray[FoundIndex].currentTriangleCount * 9);						// start position of the 3rd array's scan of GLFloatPtr
		unique_ptr<GLfloat[]> tempGLptr3(new GLfloat[ArrayThreeLength * 9]);
		//tempGLptr3 = new GLfloat[ArrayThreeLength * 9];
		for (int cc = 0; cc < (ArrayThreeLength * 9); ++cc)
		{
			tempGLptr3[cc] = GLFloatPtr[startIndex++];
		}



		// --------------------OUTPUT OF THIRD ARRAY
		/*
		index2 = 0;
		for (int z = 0; z < ArrayOneLength; z++)
		{
		for (int aa = 0; aa < 3; aa++)
		{
		//cout << "Array three Triangle [" << z << "]: Point [" << aa << "] " << tempGLptr3[index2++] << " " << tempGLptr3[index2++] << " " << tempGLptr3[index2++] << endl;
		}
		}*/
		auto finish5 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed5 = finish5 - start5;
		//cout << "Array 3 reassembly duration = " << elapsed5.count() << endl;


		// ASSEMBLE SECOND ARRAY (targeted manifest)
		auto start6 = std::chrono::high_resolution_clock::now();
		int ArrayTwoLength = (ManifestCollectionPtr->ManMatrix[Key].TotalEnclaveTriangles);
		//GLfloat *tempGLptr2;

		//tempGLptr2 = ManifestCollectionPtr->ManMatrix[Key].EnclaveGLPtr;
		unique_ptr<GLfloat[]> tempGLptr2(new GLfloat[ArrayTwoLength * 9]);
		//tempGLptr2 = new GLfloat[ArrayTwoLength * 9];
		//array2ptr = ManifestCollectionPtr->ManMatrix[Key].EnclaveGLPtr;								// this pointer will not be deleted later; doing so would delete parts of the manifest.
		int currentBegin = 0;
		int pointedBegin = 0;
		//auto finish6 = std::chrono::high_resolution_clock::now();
		int startval = (ManifestCollectionPtr->ManMatrix[Key].TotalEnclaveTriangles) * 3;
		for (int bb = 0; bb < startval; ++bb)
		{

			//GLFloatPtr[currentBegin] = tempGLptr[pointedBegin + 2];									// for first coord, x
			//GLFloatPtr[currentBegin + 1] = tempGLptr[pointedBegin + 1];									// for first coord, x
			//GLFloatPtr[currentBegin + 2] = tempGLptr[pointedBegin];

			tempGLptr2[currentBegin + 2] = ManifestCollectionPtr->ManMatrix[Key].EnclaveGLPtr[pointedBegin + 2];									// for first coord, x
			tempGLptr2[currentBegin + 1] = ManifestCollectionPtr->ManMatrix[Key].EnclaveGLPtr[pointedBegin + 1];									// for first coord, x
			tempGLptr2[currentBegin] = ManifestCollectionPtr->ManMatrix[Key].EnclaveGLPtr[pointedBegin];
			// for first coord, x
			//cout << "Triangle coord matching: (-2): " << GLFloatPtr[currentBegin - 2] << " (-1): " << GLFloatPtr[currentBegin - 1] << " (0):" << GLFloatPtr[currentBegin] << endl;
			//currentBegin -= 3;
			currentBegin += 3;
			pointedBegin += 3;

		}

		//--------------------OUTPUT OF second ARRAY
		/*
		index2 = 0;
		for (int z = 0; z < ArrayTwoLength; z++)
		{
		for (int aa = 0; aa < 3; aa++)
		{
		//cout << "Array two Triangle [" << z << "]: Point [" << aa << "] " << tempGLptr2[index2++] << " " << tempGLptr2[index2++] << " " << tempGLptr2[index2++] << endl;
		}
		}
		*/

		auto finish6 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed6 = finish6 - start6;
		//cout << "Array 2 reassembly duration = " << elapsed6.count() << endl;


		// ASSEMBLE NEW RENDER ARRAY FROM PREVIOUS 3

		auto start7 = std::chrono::high_resolution_clock::now();


		int NewRenderArrayLength = (ArrayOneLength * 9) + (ArrayThreeLength * 9) + (ArrayTwoLength * 9);		// combine lengths of first, second, and third arrays.
																												//cout << "test final length: " << NewRenderArrayLength << endl;
																												//GLfloat *newRenderArray;
																												//newRenderArray = new GLfloat[NewRenderArrayLength];
		unique_ptr<GLfloat[]> newRenderArray(new GLfloat[NewRenderArrayLength]);
		int newRenderArrayBeginIndex = 0;
		for (int cc = 0; cc < (ArrayOneLength * 9); ++cc)	// insert array one.
		{
			newRenderArray[newRenderArrayBeginIndex++] = tempGLptr[cc];
		}
		for (int dd = 0; dd < (ArrayTwoLength * 9); ++dd)		// insert array two.
		{
			newRenderArray[newRenderArrayBeginIndex++] = tempGLptr2[dd];
		}
		for (int ee = 0; ee < (ArrayThreeLength * 9); ++ee)
		{
			newRenderArray[newRenderArrayBeginIndex++] = tempGLptr3[ee];
		}

		//delete[] GLFloatPtr;
		//GLFloatPtr = newRenderArray;
		//GLFloatPtr = make_unique<GLfloat[]>;

		//--------------------OUTPUT OF FINAL ARRAY


		index2 = 0;
		RenderCollectionArraySize = NewRenderArrayLength * 4;
		//cout << "value of RenderCollectionArraySize: " << RenderCollectionArraySize << endl;
		/*
		for (int z = 0; z < (NewRenderArrayLength/9); z++)
		{
		for (int aa = 0; aa < 3; aa++)
		{
		//cout << "Final array; Triangle [" << z << "]: Point [" << aa << "] " << GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << " " << GLFloatPtr[index2++] << endl;
		}
		}
		*/

		//cout << "Array 4 reassembly duration = " << elapsed7.count() << endl;



		//delete[] tempGLptr;
		//delete[] tempGLptr2;
		//delete[] tempGLptr3;

		auto true_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> true_elapsed = true_end - true_start;
		std::cout << "Elapsed time (RenderCollection update):" << true_elapsed.count() << endl;

		auto finish7 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed7 = finish7 - start7;

	}


}

RenderCollection::RenderCollection()
{
	/* Summary: unused constructor (reserved for later, potentially)*/
}

RenderCollection::RenderCollection(int dummy)
{
	/* Summary: unused constructor (reserved for later, potentially)*/
}

RenderCollection::~RenderCollection()
{
	/* Summary: removes dynamically allocated array, if it exists */
	//if (IsGLFloatPtrLoaded == 1)
	//{
	//cout << "DELETING RENDER COLLECTION" << endl;
	//delete[] GLFloatPtr;
	//delete[] VertexColorArrayPtr;
	//IsGLFloatPtrLoaded = 0;
	//cout << "DELETE COMPLETE" << endl;
	//}
	std::lock_guard<std::mutex> lock(*heapmutexref);
	GLFloatPtr = nullptr;
	VertexColorArrayPtr = nullptr;
}
