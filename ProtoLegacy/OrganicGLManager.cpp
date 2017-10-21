#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicGLManager.h"

OrganicGLManager::OrganicGLManager()
{
	cout << "Organic GL Manager initialized." << endl;
	positionVecPtr = &position;
	directionVecPtr = &direction;
}

OrganicGLManager::~OrganicGLManager()
{
	//if (isRenderableCollectionListSet == 1)
	//{
	//	delete[] renderableCollectionList;
	//}
}

void OrganicGLManager::createRenderableCollectionList(int in_cubesize)
{
	//renderableCollectionList = new OGLMDrawCallMeta[in_cubesize*in_cubesize*in_cubesize];
	renderableCollectionList.setGLintDynamicArrays(in_cubesize, CollectionBufferSize);
	//isRenderableCollectionListSet = 1;
}

void OrganicGLManager::InitializeOpenGL()
{

	/* ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	
	BEGIN CONTEXT/PREREQUISITE TESTS
	
	|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */

	if (!glfwInit())
	{
		fprintf(stderr, "||||||||||||||||Failed to initialize GLFW\n");
		getchar();
		//return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLwindow = glfwCreateWindow(window_width, window_height, "Organic Testing", NULL, NULL);
	//GLwindow = glfwCreateWindow(2560, 1440, "Organic Testing", NULL, NULL);
	if (GLwindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		//return -1;
	}

	glfwMakeContextCurrent(GLwindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "----------------Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		//return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(GLwindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);



	// Create Vertex Array Object
	glGenVertexArrays(1, &OrganicGLVertexArrayID);
	glBindVertexArray(OrganicGLVertexArrayID);			// sets this as the current VAO to use

	// Setup programID
	OrganicGLprogramID = LoadShaders("SimpleTransform.vertexshader", "SimpleFragmentShader.fragmentshader");		/* NOTE: these shaders need to be in the same directory as the .exe file;
																													How to put these in another directory should be investigated. (8/19/2017) */
	OrganicGLVCprogramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");


	// Setup OpenGL matrices
	OrganicMVPHandle = glGetUniformLocation(OrganicGLprogramID, "MVP");
	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	View = glm::lookAt(													// Camera matrix
		glm::vec3(4, 3, -3),								 // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0),									// and looks at the origin
		glm::vec3(0, 1, 0)									// Head is up (set to 0,-1,0 to look upside-down)
	);

	Model = glm::mat4(1.0f);												// Model matrix
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	END CONTEXT/PREREQUISITE TESTS

	|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */










	/* ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	BEGIN BUFFER SETUP

	|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */




	// ::::::::::::::::MULTI BUFFER STYLE::::::::::::::::::
	//glGenBuffers(10, OrganicGLVertexBufferArray);								// generates 10 buffers, and puts their refrences into the OrganicGLVertexBufferArray
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferArray[0]);				// binds the array at index 0 to be the currently referenced buffer

	// ::::::::::::::::SINGLE BUFFER STYLE::::::::::::::::::
	//glGenBuffers(1, &OrganicGLVertexBufferID);								// generate 1 single buffer, bind it to OrganicGLVertexBufferID; this is different from the above line.
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);					// binds the previously created buffer to be a GL_ARRAY_BUFFER

	glGenBuffers(1, &OrganicGLVertexBufferID);								// generate 10 buffers, bind it to the arrayOrganicGLVertexBufferArray
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);					// binds the previously created buffer to be a GL_ARRAY_BUFFER

	//glBufferData(GL_ARRAY_BUFFER, 1024, OrganicGLarrayPTR, GL_DYNAMIC_DRAW);		// Old method; no longer utilized.
	// 3000000000
	unsigned long int bufferSize = CollectionBufferSize * numberOfBuffers;
	//cout << "Total buffer size: " << CollectionBufferSize * numberOfBuffers << endl;
	//cout << "Total buffer size: " << bufferSize << endl;
	glBufferStorage(GL_ARRAY_BUFFER, CollectionBufferSize * numberOfBuffers, NULL, GL_DYNAMIC_STORAGE_BIT);	/* REQUIRED: pre-allocates memory for the buffer, to any desired amount; this is so
																					   that the buffer doesn't need to be resized in the future.
																					
																						Parameters:
																						-first argument: target -> buffer binding target. Set to GL_ARRAY_BUFFER in this case;
																												   see official documentation for more details.

																						-second argument: buffer size -> size of the buffer.

																						-third argument: pointer to data to copy -> will copy data specified by the pointer; can use NULL
																																	if no data is to be copied (such as in this case)
																						-fourth argument: flags -> sets the flags that will be needed for this buffer; GL_DYNAMIC_STORAGE_BIT 
																													 is required to use glBufferSubData in this buffer.

																					*/
	glGenBuffers(1, &OrganicGLVertexColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexColorBufferID);
	glBufferStorage(GL_ARRAY_BUFFER, CollectionBufferSize * numberOfBuffers, NULL, GL_DYNAMIC_STORAGE_BIT);

	/* ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	END BUFFER SETUP

	|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */



	// Set up temporary data and point to it



	auto GLstart = std::chrono::high_resolution_clock::now();	// optional performance testing values
	auto GLend = std::chrono::high_resolution_clock::now();	// optional performance testing values
	std::chrono::duration<double> GLelapsed = GLend - GLstart;
	//cout << "Total time:  " << trueelapsed.count() << endl;
	//std::cout << "glBufferSubData complete. Time: " << GLelapsed.count() << std::endl;
}

void OrganicGLManager::RenderReadyArrays()
{
	auto GLstart = std::chrono::high_resolution_clock::now();	
	//glClear(GL_COLOR_BUFFER_BIT);										// clear the screen?
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(OrganicGLprogramID);									// select the already compiled program	(::::SENT TO BE USED IN selectDefaultShader() ::::)
	computeMatricesFromInputs();											// gather inputs from keyboard
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform -- 
	//glUseProgram(OrganicGLprogramID);
	//glUseProgram(OrganicGLVCprogramID);

	glUniformMatrix4fv(OrganicMVPHandle, 1, GL_FALSE, &MVP[0][0]);		// select the matrix to use.


	
	//for (int y = 0; y < 64; y++)
	//{
	/*
		for (int x = 0; x < RMContainer.TotalRenderable; x++)
		{
			// summary:
			// first argument: GL_TRIANGLES
			// second argument: vertex offset, so if byte begins at 73728, offset is 6144. (One vertex = 12 bytes)
			// last argument of glDrawArrays = number of vertices; 6144 for an entire collection face
			//glUniformMatrix4fv(OrganicMVPHandle, 1, GL_FALSE, &MVP[0][0]);		// select the matrix to use.
			//cout << "array size: " << RMContainer.RenderMetaArray[x].ArraySize << endl;
			glDrawArrays(GL_TRIANGLES, x*(CollectionBufferSize / 12), ((RMContainer.RenderMetaArray[x].ArraySize) / 12));

		}
	*/
	//}
	
	glMultiDrawArrays(GL_TRIANGLES, renderableCollectionList.TT1_GL_BufferOffset, renderableCollectionList.TT1_GL_VertexArraySize, renderableCollectionList.numberOfRenderableCollections);


	auto GLend = std::chrono::high_resolution_clock::now();	// optional performance testing values
	std::chrono::duration<double> GLelapsed = GLend - GLstart;
	//std::cout << "Frame render Time: " << GLelapsed.count() << std::endl;
	//std::cout << "RUN MULTI JOB 1 ELAPSED ITERATOR TIME: " << elapsed4.count() << std::endl;

	//glDisableVertexAttribArray(0);										// disable the array that was just used. (::::SENT TO BE USED IN ShutdownOpenGL() ::::)

	glfwSwapBuffers(GLwindow);											// ??
	glfwPollEvents();													// listen for input to the current OpenGL context window

}

void OrganicGLManager::ShutdownOpenGL()
{
	if (renderMode == 0)
	{
		// Cleanup VBOs for renderMode 0
		glDeleteBuffers(1, &OrganicGLVertexBufferID);			
		//glDeleteBuffers(1, &OrganicGLVertexBufferArray[1]);	// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
		glDeleteVertexArrays(1, &OrganicGLVertexArrayID);
		glDeleteProgram(OrganicGLprogramID);
		glDisableVertexAttribArray(0);
		glfwTerminate();										// Close OpenGL window and terminate GLFW
	}

	if (renderMode == 1)
	{
		// Cleanup VBOs for renderMode 0
		glDeleteBuffers(1, &OrganicGLVertexBufferID);
		glDeleteBuffers(1, &OrganicGLVertexColorBufferID);
		//glDeleteBuffers(1, &OrganicGLVertexBufferArray[1]);	// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
		glDeleteVertexArrays(1, &OrganicGLVertexArrayID);

		glDeleteProgram(OrganicGLprogramID);
		glDeleteProgram(OrganicGLVCprogramID);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwTerminate();										// Close OpenGL window and terminate GLFW
	}
}

void OrganicGLManager::computeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(GLwindow, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(GLwindow, window_width / 2, window_height / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(window_width / 2 - xpos);
	verticalAngle += mouseSpeed * float(window_height / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	//std::cout << "Direction vector: " << direction.x << ", " << direction.y << ", " << direction.z << endl;

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(GLwindow, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(GLwindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(GLwindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(GLwindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 400.0f);
	// Camera matrix
	View = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// For the next frame, the "last time" will be "now"

	Model = glm::mat4(1.0);
	MVP = Projection * View * Model;


	lastTime = currentTime;
	//cout << "VALUE TEST: " << position.x << ", " << position.y << ", " << position.z << endl;
}


void OrganicGLManager::sendRenderCollectionDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr)
{
	//EnclaveKeyDef::EnclaveKey firstRenderableEnclaveKey = renderCollPtr->EnclaveCollectionPtr->RenderableEnclaves[0];		// use for the below
	//EnclaveKeyDef::EnclaveKey collectionKey = renderCollPtr->EnclaveCollectionPtr->EnclaveArray[firstRenderableEnclaveKey.x][firstRenderableEnclaveKey.y][firstRenderableEnclaveKey.z].CollectionKey;
	// cout << "(Vertex data) Test; originating collection key:  " << collectionKey.x << ", " << collectionKey.y << ", " << collectionKey.z << endl;
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
	//glBufferSubData(GL_ARRAY_BUFFER, RMContainer.CurrentIndex*CollectionBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr);
	//RMContainer.RenderMetaArray[RMContainer.CurrentIndex].MetaIndex = RMContainer.CurrentIndex;
	//RMContainer.RenderMetaArray[RMContainer.CurrentIndex].ArraySize = renderCollPtr->RenderCollectionArraySize;
	//RMContainer.CurrentIndex++;
	//RMContainer.TotalRenderable++;
}

void OrganicGLManager::sendRenderCollectionDataToBufferOnGameLoad(RenderCollection *renderCollPtr)
{
	EnclaveKeyDef::EnclaveKey firstRenderableEnclaveKey = renderCollPtr->EnclaveCollectionPtr->RenderableEnclaves[0];																					// use for the below line
	EnclaveKeyDef::EnclaveKey collectionKey = renderCollPtr->EnclaveCollectionPtr->EnclaveArray[firstRenderableEnclaveKey.x][firstRenderableEnclaveKey.y][firstRenderableEnclaveKey.z].CollectionKey;	// get the collection key from the first renderable enclave
	int subBufferIndex = OrganicBufferManager.determineRenderDataSubBufferKey(collectionKey);					// use this collection key to determine which sub-buffer the data will go to

	// new glBufferSubData would go here...
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, subBufferIndex*CollectionBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr);
	OrganicBufferManager.OGLMRMC.renderMetaContainerArray[subBufferIndex].ElementRenderCollectionMeta.ArraySize = renderCollPtr->RenderCollectionArraySize;		// set the array size of this collection (in bytes; need to divide by 12 later on), in the appropriate element in OGLMRMC's dynamic array (renderMetaContainerArray)
	OrganicBufferManager.OGLMRMC.renderMetaContainerArray[subBufferIndex].ElementSingularXYZValue = subBufferIndex;		// set the initial sub buffer index for this element (needed for when this sub buffer needs to be recycled during a morph)
	renderableCollectionList.sendTerrainT1RequestToDelegator(collectionKey, subBufferIndex, renderCollPtr->RenderCollectionArraySize, CollectionBufferSize);	// add this to the renderable collection list
	cout << "Test; originating collection key:  " << collectionKey.x << ", " << collectionKey.y << ", " << collectionKey.z << endl;

}



void OrganicGLManager::sendRenderCollectionVCDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr)
{
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexColorBufferID);
	//glBufferSubData(GL_ARRAY_BUFFER, RMContainer.CurrentIndex*CollectionBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr);
	//RMContainer.RenderMetaArray[RMContainer.CurrentIndex].MetaIndex = RMContainer.CurrentIndex;						
	//RMContainer.RenderMetaArray[RMContainer.CurrentIndex].ArraySize = renderCollPtr->RenderCollectionArraySize;
	//RMContainer.CurrentIndex++;
	//RMContainer.TotalRenderable++;
}

void OrganicGLManager::sendRenderCollectionVCDataTOBufferOnGameLoad(RenderCollection *renderCollPtr)
{
	EnclaveKeyDef::EnclaveKey firstRenderableEnclaveKey = renderCollPtr->EnclaveCollectionPtr->RenderableEnclaves[0];																					// use for the below line
	EnclaveKeyDef::EnclaveKey collectionKey = renderCollPtr->EnclaveCollectionPtr->EnclaveArray[firstRenderableEnclaveKey.x][firstRenderableEnclaveKey.y][firstRenderableEnclaveKey.z].CollectionKey;	// get the collection key from the first renderable enclave
	int subBufferIndex = OrganicBufferManager.determineRenderDataSubBufferKey(collectionKey);
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexColorBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, subBufferIndex*CollectionBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr);
}


void OrganicGLManager::selectShader()
{
	if (renderMode == 0)										// Default terrain shader program: only puts colors on fragments.
	{
		glUseProgram(OrganicGLprogramID);

		glEnableVertexAttribArray(0);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								   For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								*/
		);
	}	

	if (renderMode == 1)
	{
		glUseProgram(OrganicGLVCprogramID);

		glEnableVertexAttribArray(0);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								*/
		);



		glEnableVertexAttribArray(1);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexColorBufferID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								*/
		);
	}
}

void OrganicGLManager::SetupBufferManagerArrays(int in_cubesize)
{
	/* Summary: this sets up the OGLMBufferManager's arrays; it must be called before generating any OpenGL buffers. */
	OrganicBufferManager.SetCubesize(in_cubesize);					// sets the cubesize in the buffer manager
	OrganicBufferManager.GenerateArrays();							// generates the arrays in the buffer manager
	numberOfBuffers = (in_cubesize*in_cubesize*in_cubesize);		// set the OrganicGLManager's total buffer value 
	cout << "OpenGL buffer data: --------------" << endl;
	cout << "number of Buffers" << numberOfBuffers << endl;
	cout << "buffer size: " << (numberOfBuffers * CollectionBufferSize) / 1024 / 1024 << " Megabytes " << endl;
}

void OrganicGLManager::SendPointerToBufferManager(OrganicGLManager* in_OGLMptr)
{
	OrganicBufferManager.SetOGLMPointer(in_OGLMptr);	// sends a pointer to this object (OrganicGLManager) to the object's instance of OrganicBufferManager
}

void OrganicGLManager::PrepBuffersForMoveNW()
{
	OrganicBufferManager.ShiftMatricesForNW();
}

void OrganicGLManager::setWindowSize(int width, int height)	// set size of OpenGL window
{
	window_width = width;
	window_height = height;
}
