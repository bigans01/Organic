/*------------------------------------------------------------------------------------------

--OrganicVtxColorRGB.h (Last Update: 9/4/2017)

Description: Contains 3 GLfloat values representing the RGB value of a vertex to color.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICVTXCOLORRGB_H
#define ORGANICVTXCOLORRGB_H

#include <GL/glew.h>

//class OrganicSystem;
class OrganicVtxColorRGB
{
private:
	friend class OrganicSystem;
	friend class EnclaveManifest;
	friend class EnclaveManifestFactoryT1;
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};

#endif
