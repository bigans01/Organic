/*------------------------------------------------------------------------------------------

--OrganicGLRenderCollectionMeta.h	(Last Update: 9/4/2017)

Description: header file for OrganicGLRenderCollectionMeta.cpp

Summary: A data structure that stores the size of an array that is to be sent to the vertex data OpenGL buffer

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICGLRENDERCOLLECTIONMETA_H
#define ORGANICGLRENDERCOLLECTIONMETA_H

class OrganicGLManager;
class OrganicGLRenderCollectionMeta
{
	private:
		friend class OrganicGLManager;
		int MetaIndex;			 // potentially unused variable
		int ArraySize;			 // stores the size of the array, in bytes
};

#endif
