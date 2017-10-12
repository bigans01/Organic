#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicBlockTarget.h"

void OrganicBlockTarget::setVertexOffsets()
{
	// setup offsets for the cube
	//(UPPER SOUTHWEST)
	vertexOffsets[0].x = -offsetValue;
	vertexOffsets[0].y = offsetValue;
	vertexOffsets[0].z = offsetValue;

	//(UPPER NORTHWEST)
	vertexOffsets[1].x = -offsetValue;
	vertexOffsets[1].y = offsetValue;
	vertexOffsets[1].z = -offsetValue;

	//(LOWER SOUTHWEST)
	vertexOffsets[2].x = -offsetValue;
	vertexOffsets[2].y = -offsetValue;
	vertexOffsets[2].z = offsetValue;

	//(LOWER NORTHWEST)
	vertexOffsets[3].x = -offsetValue;
	vertexOffsets[3].y = -offsetValue;
	vertexOffsets[3].z = -offsetValue;

	//(UPPER SOUTHEAST)
	vertexOffsets[4].x = offsetValue;
	vertexOffsets[4].y = offsetValue;
	vertexOffsets[4].z = offsetValue;

	//(UPPER NORTHEAST)
	vertexOffsets[5].x = offsetValue;
	vertexOffsets[5].y = offsetValue;
	vertexOffsets[5].z = -offsetValue;

	//(LOWER SOUTHEAST)
	vertexOffsets[6].x = offsetValue;
	vertexOffsets[6].y = -offsetValue;
	vertexOffsets[6].z = offsetValue;

	//(LOWER NORTHEAST)
	vertexOffsets[7].x = offsetValue;
	vertexOffsets[7].y = -offsetValue;
	vertexOffsets[7].z = -offsetValue;
}