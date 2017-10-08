/*------------------------------------------------------------------------------------------

--OrganicBlockTarget.h		(Last update 10/4/2017)

Description: Header file for OrganicBlockTarget.cpp

Summary: An OrganicBlockTarget keeps track of metadata about a block that has been targeted by the camera/player in the 3d world space.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICBLOCKTARGET_H
#define ORGANICBLOCKTARGET_H

#include <GL/glew.h>

class OrganicBlockTarget
{
	public:
		GLfloat targetVertexesXYZ[108];	// stores 36 vertices (6 vertices per face), and their x/y/z floats
};

#endif

