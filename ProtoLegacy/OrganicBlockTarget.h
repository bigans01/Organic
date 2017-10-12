/*------------------------------------------------------------------------------------------

--OrganicBlockTarget.h		(Last update 10/4/2017)

Description: Header file for OrganicBlockTarget.cpp

Summary: An OrganicBlockTarget keeps track of metadata about a block that has been targeted by the camera/player in the 3d world space.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICBLOCKTARGET_H
#define ORGANICBLOCKTARGET_H

#include "FloatTupleXYZ.h"
#include <GL/glew.h>

class OrganicBlockTarget
{
	public:
		FloatTupleXYZ targetVertexesXYZ[36];	// stores 36 vertices (6 vertices per face), and their x/y/z floats
		FloatTupleXYZ vertexOffsets[8];			// x/y/z offset for each vertex, to shape the target block
		int hasAcquiredTarget = 0;				// determines if a block has been targeted (1 = true, 0 = false)
		float offsetValue = 0.1f;				// the offset constant used for each vertex's x/y/z; can be changed dynamically
		void setVertexOffsets();				// function used to setup the vertexOffsets array

};

#endif

