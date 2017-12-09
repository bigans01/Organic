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
	glm::vec3* directionVecPtr = NULL;
	glm::vec3* positionVecPtr = NULL;													// a pointer the position vector.
	GLFWwindow* GLwindow = NULL;														// OpenGL: pointer/handle to the OpenGL window (context?)
private:
	friend class OrganicSystem;
	friend class OGLMBufferManager;
	int renderMode = 0;
	GLuint OrganicMode0ProgramID = 0;													// OpenGL: pointer/handle to the program used for shading	
	GLuint OrganicMode1ProgramID = 0;

	GLuint OrganicGLVertexArrayID = 0;												// OpenGL: pointer/handle to the vertex array handler
	GLuint OrganicGLVertexCoordVBOID = 0;										// OpenGL: pointer/handle to the vertex coordinate VBO buffer handler (single buffer)
	GLuint OrganicGLVertexNormalID = 0;
	GLuint OrganicGLIndirectBufferID = 0;

	GLuint OrganicGLVertexSecondaryVBOID = 0;									// OpenGL: pointer/handle to the secondary buffer; depending on mode, this will store either colors or texture coordinates
	GLuint OrganicGLLightingBufferID = 0;											// OpenGL: third potential single buffer; currently in testing only. (not being generated as of (10/11/2017))
	GLuint OrganicGLLightingBufferID2 = 0;											// OpenGL: third potential single buffer; currently in testing only. (not being generated as of (10/11/2017))
	GLuint OrganicGLVertexBufferArray[10];										// OpenGL: array of potential buffers
	GLuint OrganicMVPHandle = 0;													// OpenGL: pointer/handle to the MVP matrix result
	OGLMBufferManager OrganicBufferManager;										// the instance of OGLMBufferManager that this OrganicGLManager object will use
	GLfloat *OrganicGLarrayPTR = NULL;													// (temporary) OpenGL: used to point to a dynamic array containing vertex data
	//const int OGLMVertexSubBufferSize = 1024 * 1024;								// the size of the data buffer for each RenderCollection; 
	const int cltnFaceVertexCoordTotalBytes = 73728;					// total bytes per collection face, per vertex attribute
	const int cltnFaceVertexColorTotalBytes = 73728;
	const int cltnFaceTextureCoordTotalBytes = 49152;
	const int cltnFaceNormalCoordTotalBytes = 73728;
	const int cltnFaceStorageCount = 8;



	/* A Note on OGLMVertexSubBufferSize (and similiar members) values:
		73728 = the total byte size of all vertices that can exist on one collection's face. It is equivalent to one collection face, which iscalculated by:
		18 floats per cube face (6 vertexes)		X    16 faces	X	64 chunks	X size of float (4 bytes)

		So, a size of 589824 is equivalent to the total size of all vertices in 8 collection faces.
		1 collection face:		73,728
		2 collecion faces:		147,456
		3 collection faces:		221,184
		4 collection faces:		294,912
		5 collection faces:		368,640
		6 collection faces:		442,368
		7 collection faces:		516,096
		8 collection faces:		589,824


	*/
	//const int OGLMVertexSubBufferSize = cltnFaceStorageCount * ((cltnFaceVertexCoordTotalBytes*2)+ cltnFaceVertexCoordTotalBytes);	// size of all vertex data for a sub element = number of collection faces * 73728;
	//int OGLMVertexSubBufferSize = cltnFaceStorageCount *  ((cltnFaceVertexCoordTotalBytes * 2) + cltnFaceTextureCoordTotalBytes);
	int OGLMVertexSubBufferSize = cltnFaceStorageCount * cltnFaceVertexCoordTotalBytes;	// size of all vertex data for a sub element = number of collection faces * 73728;
	int OGLMTexUVSubBufferSize = cltnFaceStorageCount * cltnFaceTextureCoordTotalBytes;	// size of all UV texture coordinate data for a sub element = number of collection faces * 49152


	const GLbitfield bufferStorageflags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// mandatory flags for glBufferStorage
	const GLbitfield mapFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glm::vec3* bufferMap1;
	glm::vec3* bufferMap2;


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

	void SetupBufferManagerArrays(int in_T1_cubesize, int in_T2_cubesize);								// sets up buffers based on the passed cube size
	void InitializeOpenGL();													// prepares OpenGL for future use; set up OpenGL context, prepare buffers etc
	void RenderReadyArrays();													// renders whatever render collections have been prepared for rendering, to the screen
	void ShutdownOpenGL();														// unbind buffers, etc
	void computeMatricesFromInputs();											// determines how to move the camera based on user input
	void sendRenderCollectionDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr);		// sends a prepared instance of RenderCollection which has vertex point data to OpenGL buffers
	void sendRenderCollectionDataToBufferOnGameLoad(RenderCollection *renderCollPtr);
	void sendRenderCollectionVCDataTOBufferOnGameLoad(RenderCollection *renderCollPtr);
	void sendRenderCollectionVCDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr);	// sends a prepared instance of RenderCollection which has vertex color point data to OpenGL buffers
	void SendPointerToBufferManager(OrganicGLManager* in_OGLMptr);				// sends this own object's pointer to the instance of OGLMBufferManager (OrganicBufferManager)
	//void PrepBuffersForMoveNW();												// reserved for later use (will "move" buffers around)
	void selectShader();														// selects the appropriate shader program based on the render mode
	void setWindowSize(int length, int height);
	void createRenderableCollectionList(int in_cubesize);
	//void addToRenderableCollectionList(EnclaveKeyDef::EnclaveKey in_key, int in_subBufferIndex, int in_vertexArrayByteSize);
};

#endif
