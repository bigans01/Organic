/*------------------------------------------------------------------------------------------

--OrganicGLManager.h	(Last Update: 9/15/2017)

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
#include "OGLMBufferManager.h"
#include "OGLMDrawCallMeta.h"
#include "OrganicMorphMeta.h"
#include "EnclaveKeyDef.h"

class OrganicSystem;
class OGLMBufferManager;
class OrganicGLManager
{
public:
	glm::vec3* directionVecPtr;
	glm::vec3* positionVecPtr;													// a pointer the position vector.
	GLFWwindow*  GLwindow;														// OpenGL: pointer/handle to the OpenGL window (context?)
private:
	friend class OrganicSystem;
	friend class OGLMBufferManager;
	int renderMode = 0;
	GLuint OrganicGLprogramID;													// OpenGL: pointer/handle to the program used for shading	
	GLuint OrganicGLVCprogramID;

	GLuint OrganicGLVertexArrayID;												// OpenGL: pointer/handle to the vertex array handler
	GLuint OrganicGLVertexBufferID;												// OpenGL: pointer/handle to the vertex buffer handler (single buffer)

	GLuint OrganicGLVertexColorBufferID;										// OpenGL: pointer/handle to the vertex color buffer handler (single buffer)
	GLuint OrganicGLLightingBufferID;											// OpenGL: third potential single buffer; currently in testing only. (not being generated as of (10/11/2017))
	GLuint OrganicGLLightingBufferID2;											// OpenGL: third potential single buffer; currently in testing only. (not being generated as of (10/11/2017))
	GLuint OrganicGLVertexBufferArray[10];										// OpenGL: array of potential buffers
	GLuint OrganicMVPHandle;													// OpenGL: pointer/handle to the MVP matrix result
	OGLMBufferManager OrganicBufferManager;										// the instance of OGLMBufferManager that this OrganicGLManager object will use
	GLfloat *OrganicGLarrayPTR;													// (temporary) OpenGL: used to point to a dynamic array containing vertex data
	//const int CollectionBufferSize = 1024 * 1024;								// the size of the data buffer for each RenderCollection; 
	const int CollectionBufferSize = 589824;										//alternate = 73728   , 147456, 294912, 589824, 1179648
	int numberOfBuffers = 0;													// initial value is 0, but will be reset
	int window_width = 0;														// width in pixels of the OpenGL window
	int window_height = 0;														// height in pixels of the OpenGL window
	OGLMDrawCallMeta renderableCollectionList;									// pointer to a dynamic array that contains a list of OGLMDrawCallMeta
	//int isRenderableCollectionListSet = 0;										// determines whether or not the dynamic array above was created
	//int renderableCollectionCount = 0;											// determines the number of renderable collections in renderableCollectionList
	glm::mat4 Projection;														// (temporary) OpenGL projection matrix
	glm::mat4 View;																// (temporary) OpenGL view matrix
	glm::mat4 Model;															// (temporary) OpenGL model matrix
	glm::mat4 MVP;																// (temporary) will store the result of multiplying Projection * View * Model matrices.																			
	glm::vec3 position = glm::vec3(30, 0, 5);									// Initial position of camera : on +30x, 0y, +5z
	glm::vec3 direction;
	float horizontalAngle = 3.14f;												// Initial horizontal angle : toward -Z
	float verticalAngle = 0.0f;													// Initial vertical angle : none
	float initialFoV = 45.0f;													// Initial Field of View
	float speed = 10.0f;															// 3 units / second
	float mouseSpeed = 0.005f;
	struct RenderMetaContainer
	{
		int CurrentIndex = 0;
		int TotalRenderable = 0;
		OrganicGLRenderCollectionMeta RenderMetaArray[512];
	};
	RenderMetaContainer RMContainer;

	OrganicGLManager();
	~OrganicGLManager();

	void SetupBufferManagerArrays(int in_cubesize);								// sets up buffers based on the passed cube size
	void InitializeOpenGL();													// prepares OpenGL for future use; set up OpenGL context, prepare buffers etc
	void RenderReadyArrays();													// renders whatever render collections have been prepared for rendering, to the screen
	void ShutdownOpenGL();														// unbind buffers, etc
	void computeMatricesFromInputs();											// determines how to move the camera based on user input
	void sendRenderCollectionDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr);		// sends a prepared instance of RenderCollection which has vertex point data to OpenGL buffers
	void sendRenderCollectionDataToBufferOnGameLoad(RenderCollection *renderCollPtr);
	void sendRenderCollectionVCDataTOBufferOnGameLoad(RenderCollection *renderCollPtr);
	void sendRenderCollectionVCDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr);	// sends a prepared instance of RenderCollection which has vertex color point data to OpenGL buffers
	void SendPointerToBufferManager(OrganicGLManager* in_OGLMptr);				// sends this own object's pointer to the instance of OGLMBufferManager (OrganicBufferManager)
	void PrepBuffersForMoveNW();												// reserved for later use (will "move" buffers around)
	void selectShader();														// selects the appropriate shader program based on the render mode
	void setWindowSize(int length, int height);
	void createRenderableCollectionList(int in_cubesize);
	//void addToRenderableCollectionList(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize);
};

#endif
