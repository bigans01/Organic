/*------------------------------------------------------------------------------------------

--CursorPathTraceContainer.h		(Last update 9/22/2017)

Description: Header file for CursorPathTraceContainer.h

Summary: A CursorPathTraceContanier contains the x/y/z coordinates of a collection, a chunk within the collection, a block within the chunk, and the exact point within the block.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef CURSORPATHTRACECONTAINER_H
#define CURSORPATHTRACECONTAINER_H

class CursorPathTraceContainer
{
public:
	float CollectionCoord = 0.0f;			// x, y, or z coordinate of the Collection of Enclaves
	float ChunkCoord = 0.0f;				// x, y, or z coordinate of the chunk within the Collection
	float BlockCoord = 0.0f;				// x, y, or z coordinate of the block within the chunk
	float ExactBlockCoord = 0.0f;				// any x/y/z remainder after block coord is calculated
	float distance_to_pos = 0.0f;		// distance of x from this point to the east border of the block
	float distance_to_neg = 0.0f;		// distance of x from this point to the west border of the block
};

#endif
