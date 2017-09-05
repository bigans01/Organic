/*------------------------------------------------------------------------------------------

--OrganicGLManager.h	(Last Update: 8/20/2017)

Description: header file for OrganicGLManager.cpp

Summary: An instance of OrganicGLManager is responsible for managing all OpenGL objects, including but not limited to terrain and other objects.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICGLMANAGER_H
#define ORGANICGLMANAGER_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "shader.hpp"
#include "common/shader.hpp"
#include "RenderCollection.h"
#include "OrganicGLRenderCollectionMeta.h"

class OrganicGLManager
{
public:
	int renderMode = 0;
	GLFWwindow  *GLwindow;														// OpenGL: pointer/handle to the OpenGL window (context?)
	GLuint OrganicGLprogramID;													// OpenGL: pointer/handle to the program used for shading	
	GLuint OrganicGLVCprogramID;
	GLuint OrganicGLVertexArrayID;												// OpenGL: pointer/handle to the vertex array handler
	GLuint OrganicGLVertexBufferID;												// OpenGL: pointer/handle to the vertex buffer handler (single buffer)

	GLuint OrganicGLVertexColorBufferID;										// OpenGL: pointer/handle to the vertex color buffer handler (single buffer)
	GLuint OrganicGLVertexBufferArray[10];										// OpenGL: array of potential buffers
	GLuint OrganicMVPHandle;													// OpenGL: pointer/handle to the MVP matrix result
	GLfloat *OrganicGLarrayPTR;													// (temporary) OpenGL: used to point to a dynamic array containing vertex data
	//const int CollectionBufferSize = 1024 * 1024;								// the size of the data buffer for each RenderCollection; 
	const int CollectionBufferSize = 589824;										//alternate = 73728   , 147456, 294912, 589824, 1179648
	glm::mat4 Projection;														// (temporary) OpenGL projection matrix
	glm::mat4 View;																// (temporary) OpenGL view matrix
	glm::mat4 Model;															// (temporary) OpenGL model matrix
	glm::mat4 MVP;																// (temporary) will store the result of multiplying Projection * View * Model matrices.																			
	glm::vec3 position = glm::vec3(0, 0, 5);									// Initial position : on +Z
	float horizontalAngle = 3.14f;												// Initial horizontal angle : toward -Z
	float verticalAngle = 0.0f;													// Initial vertical angle : none
	float initialFoV = 45.0f;													// Initial Field of View
	float speed = 3.0f;															// 3 units / second
	float mouseSpeed = 0.005f;
	struct RenderMetaContainer
	{
		int CurrentIndex = 0;
		int TotalRenderable = 0;
		OrganicGLRenderCollectionMeta RenderMetaArray[512];
	};
	RenderMetaContainer RMContainer;

	OrganicGLManager();

	void InitializeOpenGL();													// prepares OpenGL for future use; set up OpenGL context, prepare buffers etc
	void RenderReadyArrays();
	void ShutdownOpenGL();
	void computeMatricesFromInputs();
	void sendRenderCollectionDataToBuffer(RenderCollection *renderCollPtr);
	void sendRenderCollectionVCDataToBuffer(RenderCollection *renderCollPtr);

	void selectShader();
};

#endif
