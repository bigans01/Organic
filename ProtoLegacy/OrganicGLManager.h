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

class OrganicGLManager
{
public:
	GLFWwindow  *GLwindow;														// OpenGL: pointer/handle to the OpenGL window (context?)
	GLuint OrganicGLprogramID;													// OpenGL: pointer/handle to the program used for shading	
	GLuint OrganicGLVertexArrayID;												// OpenGL: pointer/handle to the vertex array handler
	GLuint OrganicGLVertexBufferID;												// OpenGL: pointer/handle to the vertex buffer handler (single buffer)
	GLuint OrganicGLVertexBufferArray[10];										// OpenGL: array of potential buffers
	GLuint OrganicMVPHandle;													// OpenGL: pointer/handle to the MVP matrix result
	GLfloat *OrganicGLarrayPTR;													// (temporary) OpenGL: used to point to a dynamic array containing vertex data
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);	// (temporary) OpenGL projection matrix
	glm::mat4 View;																// (temporary) OpenGL view matrix
	glm::mat4 Model;															// (temporary) OpenGL model matrix
	glm::mat4 MVP;																// (temporary) will store the result of multiplying Projection * View * Model matrices.

	OrganicGLManager();

	void InitializeOpenGL();													// prepares OpenGL for future use; set up OpenGL context, prepare buffers etc
	void RenderReadyArrays();
	void ShutdownOpenGL();
};

#endif
