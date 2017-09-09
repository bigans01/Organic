/*------------------------------------------------------------------------------------------

--OGLMBufferManager.h		(Last update 9/7/2017)

Description: Header file for OGLMBufferManager.cpp

Summary: An OGLMBufferManager is responsible for determining which OpenGL buffers need to be updated, as well as how many buffers need to be udpated. It contains several 3d matrices that are used in unison to properly render
only buffers that actually contain data to the screen.

Dependents: -a valid instance of an OrganicSystem
			-enough RAM on MemoryCard to support the desired buffer size (could possibly result in undefined behavior or worse, a crash)

Notes:	this will very likely be modified continuously throughout development.

------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef OGLMBUFFERMANAGER_H
#define OGLMBUFFERMANAGER_H

class OrganicGLManager;					// required forward declaration
class OGLMBufferManager {
public:
	int* BufferOffsetMatrixArray;		// a pointer to a dynamic array which will contain memory offsets measured in bytes. these offsets represent the exact beginning location of each "sub-buffer" that a collection has within the mega buffer
	int* RenderCollectionMatrixArray;	// a pointer to a dynamic array which will contain pointers to the render collections that will need to be rendered.
	int* RenderableBufferList;			// a pointer to a sorted list that will determine how many buffers to render. This will be resorted every time a new buffer needs to be executed by OpenGL.
	int cubesize = 0;					// determines the x, y, and z lengths of the cube. 
	int arraysSet = 0;
	OrganicGLManager* OGLMPtr;

	//OGLMBufferManager();	
	~OGLMBufferManager();

	void SetCubesize(int inCubesize);
	void SetOGLMPointer(OrganicGLManager* in_OGLMptr);
	void GenerateArrays();
	void ShiftMatricesForNW();
	int translateXYZToSingle(int x, int y, int z);
};

#endif
