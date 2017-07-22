/*------------------------------------------------------------------------------------------

--PathTraceContainer.h		(Last update 7/14/2017)

Description: Header file for PathTraceContainer.cpp.

Summary: A PathTraceContainer stores the x, y, or z coordinates of a block's location, the x, y, or z of the chunk it is contained in, and x, y, or z of the collection that the chunk is contained in. 


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef PATHTRACECONTAINER_H
#define PATHTRACECONTAINER_H

#include <iostream>

class PathTraceContainer {
public:
	int CollectionCoord;			// x, y, or z coordinate of the Collection of Enclaves
	int ChunkCoord;					// x, y, or z coordinate of the chunk within the Collection
	int BlockCoord;					// x, y, or z coordinate of the block within the chunk
};

#endif