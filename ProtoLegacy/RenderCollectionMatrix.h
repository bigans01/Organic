/*------------------------------------------------------------------------------------------

--RenderCollectionMatrix.h		(Last update 7/14/2017)

Description: Header file for RenderCollectionMatrix.cpp.

Summary: A RenderCollectionMatrix is an unordered_map of RenderCollections. It's primary purpose is to be organzied in a way such that OpenGL may populate its buffers with the vertex data found in this object; an OrganicSystem object 
will iterate over all elements found within this object, calling one draw call for each RenderCollection.


------------------------------------------------------------------------------------------*/


#pragma once


// activate guards
#ifndef RENDERCOLLECTIONMATRIX_H
#define RENDERCOLLECTIONMATRIX_H

#include <unordered_map>
#include "RenderCollection.h"
#include "ManifestCollectionMatrix.h"
#include <mutex>

class RenderCollectionMatrix
{
	public:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RenderCollection, EnclaveKeyDef::KeyHasher> RenderMatrix;			// the unordered map of RenderCollections.
		ManifestCollectionMatrix *ManifestCollectionMatrixPtr;															// pointer to the ManifestCollectionMatrix, where the ManifestCollections will be found.
		EnclaveCollectionMatrix *EnclaveCollectionMatrixPtr;
		void CreateRenderArrayFromManifestCollection(EnclaveKeyDef::EnclaveKey Key);									// creates a new 3d vertex data array, for the RenderCollection having a value of Key, 
																														// by iterating through all elements of the ManifestCollection that has a value of Key.
		void SetManifestCollectionMatrixPtr(ManifestCollectionMatrix *manifestcollectionmatrixref);
		void SetEnclaveCollectionMatrixPtr(EnclaveCollectionMatrix *enclavecollectionmatrixref);
		RenderCollectionMatrix(ManifestCollectionMatrix *manifestcollectionmatrixref);									// sets the ManifestCollectionMatrixPtr value; default constructor.
		RenderCollectionMatrix();																						// constructor designed for use in OrganicSystem objects
		//mutex cmutex;
};

#endif
