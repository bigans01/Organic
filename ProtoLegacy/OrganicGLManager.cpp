#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicGLManager.h"
#include "OrganicSystem.h"

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
	renderableCollectionList.setGLintDynamicArrays(in_cubesize, OGLMVertexSubBufferSize);
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





														// Setup programID
	OrganicMode0ProgramID = LoadShaders("Mode0_VertexShader.vertexshader", "Mode0_FragmentShader.fragmentshader");		/* NOTE: these shaders need to be in the same directory as the .exe file;
																													How to put these in another directory should be investigated. (8/19/2017) */
	OrganicMode1ProgramID = LoadShaders("Mode1_VertexShader.vertexshader", "Mode1_FragmentShader.fragmentshader");


	// Setup OpenGL matrices
	OrganicMVPHandle = glGetUniformLocation(OrganicMode0ProgramID, "MVP");
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
	//glGenBuffers(1, &OrganicGLVertexCoordVBOID);								// generate 1 single buffer, bind it to OrganicGLVertexCoordVBOID; this is different from the above line.
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);					// binds the previously created buffer to be a GL_ARRAY_BUFFER

	//glBufferData(GL_ARRAY_BUFFER, 1024, OrganicGLarrayPTR, GL_DYNAMIC_DRAW);		// Old method; no longer utilized.
	// 3000000000
	//unsigned long int bufferSize = OGLMVertexSubBufferSize * numberOfBuffers;
	//cout << "Total buffer size: " << OGLMVertexSubBufferSize * numberOfBuffers << endl;
	//cout << "Total buffer size: " << bufferSize << endl;
	//GLbitfield flags = GL_DYNAMIC_STORAGE_BIT;
	//GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT;
	//GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	//GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;
	//GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;




	// --- REQUIRED - allocate mandatory buffers and objects

	// Create Vertex Array Object
	glGenVertexArrays(1, &OrganicGLVertexArrayID);
	glBindVertexArray(OrganicGLVertexArrayID);			// sets this as the current VAO to use

																										
																										

																										
																										
																										

																										

																										
																										
																										
																										
																										
	// indirect draw buffer
	glGenBuffers(1, &OrganicGLIndirectBufferID);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OrganicGLIndirectBufferID);


	// --- RENDER MODE BASED -- 

	// MODE 0
	if (renderMode == 0)
	{
		// bufferStorage for vertexes
		glGenBuffers(1, &OrganicGLVertexCoordVBOID);								// generate 10 buffers, bind it to the arrayOrganicGLVertexBufferArray
		glEnableVertexAttribArray(0);												// enable vertex attrib array for attribute 0
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);					// binds the previously created buffer to be a GL_ARRAY_BUFFER
		glBufferStorage(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize * numberOfBuffers, NULL, bufferStorageflags);	
		//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
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

	// MODE 1
	if (renderMode == 1)
	{
		// bufferStorage for vertexes


		/*
		glGenBuffers(1, &OrganicGLVertexCoordVBOID);								// generate 10 buffers, bind it to the arrayOrganicGLVertexBufferArray
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);					// binds the previously created buffer to be a GL_ARRAY_BUFFER
		glBufferStorage(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize * numberOfBuffers, NULL, bufferStorageflags);	
		//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								//For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								//
		);

		// set secondary buffer to store colors per vertex
		glGenBuffers(1, &OrganicGLVertexSecondaryVBOID);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);				
		glBufferStorage(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize * numberOfBuffers, NULL, bufferStorageflags);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								//For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								//
		);
		*/


		
		// NEW PROTOTYPE TESTING -- Step 1, set appropriate vertex attributess
		// glUseProgram(OrganicMode1ProgramID);	// use appropriate program
		glGenBuffers(1, &OrganicGLVertexCoordVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID); // bind to primary VBO
		glBufferStorage(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize * numberOfBuffers, NULL, bufferStorageflags);
		// interleaving, total stride amount is 24 bytes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)0);	// vertex point data: begins at 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)12 ); // color point data: begins at byte 12
		

	}

	// MODE 2
	if (renderMode == 2)
	{
		// set secondary buffer to store UV coordinates per vertex
		glGenBuffers(1, &OrganicGLVertexSecondaryVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);
		glBufferStorage(GL_ARRAY_BUFFER, OGLMTexUVSubBufferSize * numberOfBuffers, NULL, bufferStorageflags);

	}

	// MODE 3


	/*-----RESERVED FOR LATER USE-----
	bufferMap1 = reinterpret_cast<glm::vec3*>
		(
			glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * 1, mapFlags)
			);

	bufferMap2 = reinterpret_cast<glm::vec3*>
		(
			glMapBufferRange(GL_ARRAY_BUFFER, 10, sizeof(glm::vec3) * 1, mapFlags)
			);
	*/


	//GLenum err;
	//err = glGetError();
	//cout << "potential error is: " << err << endl;



	//glGenBuffers(1, &OrganicGLIndirectBufferID);
	//glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OrganicGLIndirectBufferID);


	//glGenBuffers(1, &OrganicGLVertexNormalID);
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexNormalID);
	//glBufferStorage(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize * numberOfBuffers, NULL, GL_DYNAMIC_STORAGE_BIT);



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
	
	//glClear(GL_COLOR_BUFFER_BIT);										// clear the screen?
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(OrganicMode0ProgramID);									// select the already compiled program	(::::SENT TO BE USED IN selectDefaultShader() ::::)
	computeMatricesFromInputs();											// gather inputs from keyboard
																			// Send our transformation to the currently bound shader, 
																			// in the "MVP" uniform -- 
																			//glUseProgram(OrganicMode0ProgramID);
																			//glUseProgram(OrganicMode1ProgramID);

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
																		glDrawArrays(GL_TRIANGLES, x*(OGLMVertexSubBufferSize / 12), ((RMContainer.RenderMetaArray[x].ArraySize) / 12));

																		}
																		*/
																		//}
	//----> OPTION 1: traditional multi draw
	//for (int x = 0; x < 5; x++)
	//{
		//glMultiDrawArrays(GL_TRIANGLES, renderableCollectionList.TT1_GL_BufferOffset.get(), renderableCollectionList.TT1_GL_VertexArraySize.get(), renderableCollectionList.numberOfRenderableCollections);
	//}


	//cout << renderableCollectionList.numberOfRenderableCollections << endl;

	// PROTOTYPE DEBUG LINES -- using key -1, 0, 0

	/*
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID); // bind to primary VBO
	EnclaveKeyDef::EnclaveKey debugKey, debugKey2;
	debugKey.x = -1;
	debugKey.y = 0;
	debugKey.z = 0;
	RenderCollection* tempRenderColl = &organicSystemPtr->RenderCollections.RenderMatrix[debugKey];
	int foundindex = 0;
	for (int x = 0; x < renderableCollectionList.numberOfRenderableCollections; x++)
	{
		if (renderableCollectionList.TT1_CollectionKeys[x] == debugKey)
		{
			//cout << "key found!" << endl;
			foundindex = x;
		}
	}
	*/

	//glBufferSubData(GL_ARRAY_BUFFER, 0, tempRenderColl->RenderCollectionArraySize, tempRenderColl->GLFloatPtr.get());
	//glBufferSubData(GL_ARRAY_BUFFER, OGLMVertexSubBufferSize, tempRenderColl->RenderCollectionArraySize, tempRenderColl->GLFloatPtr.get());

	//glDrawArrays(GL_TRIANGLES, renderableCollectionList.TT1_GL_BufferOffset[foundindex], renderableCollectionList.TT1_GL_VertexArraySize[foundindex]);
	//glDrawArrays(GL_TRIANGLES, OGLMVertexSubBufferSize / 24, (tempRenderColl->RenderCollectionArraySize) / 6);


	//----> OPTION 2: use multidraw arrays
	
	
	// test struct
	struct DETest
	{
		GLuint vertexCount;				// number of vertexex to render
		GLuint instanceCount;			// should be 1
		GLuint firstIndex;				// first index to render
		GLuint baseInstance;
	};
	unique_ptr<DETest[]> testingStruct;
	testingStruct.reset(new DETest[renderableCollectionList.numberOfRenderableCollections]);
	for (int i = 0; i < renderableCollectionList.numberOfRenderableCollections; i++)
	{
		testingStruct[i].vertexCount = renderableCollectionList.TT1_GL_VertexArraySize[i];
		testingStruct[i].instanceCount = 1;
		testingStruct[i].firstIndex = renderableCollectionList.TT1_GL_BufferOffset[i];
		testingStruct[i].baseInstance = i;
	}
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, OrganicGLIndirectBufferID);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(GLuint)*4* renderableCollectionList.numberOfRenderableCollections, testingStruct.get(), GL_DYNAMIC_DRAW);		// important note: size of struct is actually 8 (4 x 2) bytes, not 16 (4 x 4) like normal ints

	//for (int x = 0; x < 5; x++)
	//{
		glMultiDrawArraysIndirect(GL_TRIANGLES, (GLvoid*)0, renderableCollectionList.numberOfRenderableCollections, 0);
	//}


	

	
	//std::cout << "RUN MULTI JOB 1 ELAPSED ITERATOR TIME: " << elapsed4.count() << std::endl;

	//glDisableVertexAttribArray(0);										// disable the array that was just used. (::::SENT TO BE USED IN ShutdownOpenGL() ::::)

	auto GLstart = std::chrono::high_resolution_clock::now();
	glfwSwapBuffers(GLwindow);											// ??
	auto GLend = std::chrono::high_resolution_clock::now();	// optional performance testing values
	glfwPollEvents();													// listen for input to the current OpenGL context window

	std::chrono::duration<double> GLelapsed = GLend - GLstart;
	//std::cout << "Frame render Time: (actual draw call call) " << GLelapsed.count() << std::endl;
}

void OrganicGLManager::ShutdownOpenGL()
{
	if (renderMode == 0)
	{
		// Cleanup VBOs for renderMode 0
		glDeleteBuffers(1, &OrganicGLVertexCoordVBOID);
		//glDeleteBuffers(1, &OrganicGLVertexBufferArray[1]);	// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glDeleteVertexArrays(1, &OrganicGLVertexArrayID);
		glDeleteProgram(OrganicMode0ProgramID);
		glDisableVertexAttribArray(0);
		glfwTerminate();										// Close OpenGL window and terminate GLFW
	}

	if (renderMode == 1  || renderMode == 2)
	{
		// Cleanup VBOs for renderMode 0
		glDeleteBuffers(1, &OrganicGLVertexCoordVBOID);
		//glDeleteBuffers(1, &OrganicGLVertexSecondaryVBOID);		// NEW PROTOTYPE TESTING -- remove this line for testing 
		//glDeleteBuffers(1, &OrganicGLVertexBufferArray[1]);	// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glDeleteVertexArrays(1, &OrganicGLVertexArrayID);

		glDeleteProgram(OrganicMode0ProgramID);
		glDeleteProgram(OrganicMode1ProgramID);

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
	renderableCollectionList.sendTerrainT1RequestToDelegator(inMorphMeta.collectionKey, inMorphMeta.subBufferIndex, renderCollPtr->RenderCollectionArraySize, OGLMVertexSubBufferSize);	// send to T1 delegator; delegator will move any occupied data in the sub buffer to another appropriate location
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);
	// cout << "sending to buffer: SubBufferIndex: " << inMorphMeta.subBufferIndex << "  Actual offset: " << inMorphMeta.subBufferIndex*OGLMVertexSubBufferSize << endl;
	glBufferSubData(GL_ARRAY_BUFFER, inMorphMeta.subBufferIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr.get());
	//renderableCollectionList.sendTerrainT1RequestToDelegator(inMorphMeta.collectionKey, inMorphMeta.subBufferIndex, renderCollPtr->RenderCollectionArraySize, OGLMVertexSubBufferSize);
	//cout << "collection sent to buffer..." << inMorphMeta.collectionKey.x << ", " << inMorphMeta.collectionKey.y << ", " << inMorphMeta.collectionKey.z << endl;
	//cout << "size: " << renderCollPtr->RenderCollectionArraySize << endl;
	//EnclaveKeyDef::EnclaveKey firstRenderableEnclaveKey = renderCollPtr->EnclaveCollectionPtr->RenderableEnclaves[0];		// use for the below
	//EnclaveKeyDef::EnclaveKey collectionKey = renderCollPtr->EnclaveCollectionPtr->EnclaveArray[firstRenderableEnclaveKey.x][firstRenderableEnclaveKey.y][firstRenderableEnclaveKey.z].CollectionKey;
	// cout << "(Vertex data) Test; originating collection key:  " << collectionKey.x << ", " << collectionKey.y << ", " << collectionKey.z << endl;
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
	//glBufferSubData(GL_ARRAY_BUFFER, RMContainer.CurrentIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr);
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
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);
	glBufferSubData(GL_ARRAY_BUFFER, subBufferIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr.get());
	//OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[subBufferIndex].ElementRenderCollectionMeta.ArraySize = renderCollPtr->RenderCollectionArraySize;		// set the array size of this collection (in bytes; need to divide by 12 later on), in the appropriate element in OGLMRMC's dynamic array (T2_renderMetaContainerArray)
	OrganicBufferManager.OGLMRMC.T2_renderMetaContainerArray[subBufferIndex].ElementSingularXYZValue = subBufferIndex;		// set the initial sub buffer index for this element (needed for when this sub buffer needs to be recycled during a morph)
	renderableCollectionList.sendTerrainT1RequestToDelegator(collectionKey, subBufferIndex, renderCollPtr->RenderCollectionArraySize, OGLMVertexSubBufferSize);	// add this to the renderable collection list
	cout << "Test; originating collection key:  " << collectionKey.x << ", " << collectionKey.y << ", " << collectionKey.z << endl;

}



void OrganicGLManager::sendRenderCollectionVCDataToBuffer(OrganicMorphMeta inMorphMeta, RenderCollection* renderCollPtr)
{
	if (renderMode == 1) // for safety -- don't send data for a buffer that doesn't exist
	{
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);
		glBufferSubData(GL_ARRAY_BUFFER, inMorphMeta.subBufferIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr.get());
	}
	//glBufferSubData(GL_ARRAY_BUFFER, subBufferIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr);
	//glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);
	//glBufferSubData(GL_ARRAY_BUFFER, RMContainer.CurrentIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr);
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
	if (renderMode == 1) // for safety -- don't send data for a buffer that doesn't exist
	{
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);
		glBufferSubData(GL_ARRAY_BUFFER, subBufferIndex*OGLMVertexSubBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->VertexColorArrayPtr.get());
	}
}


void OrganicGLManager::selectShader()
{
	if (renderMode == 0)										// Default terrain shader program: only puts colors on fragments.
	{
		glUseProgram(OrganicMode0ProgramID);

		/*
		glEnableVertexAttribArray(0);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								// For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								
		);
		*/
	}

	if (renderMode == 1)
	{
		glUseProgram(OrganicMode1ProgramID);


		/*
		glEnableVertexAttribArray(0);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								//For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								
		);



		glEnableVertexAttribArray(1);										//select the buffer we will be using
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexSecondaryVBOID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexCoordVBOID
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
								//For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
								
		);
		*/

		/*
		// NEW PROTOTYPE TESTING -- Step 1, set appropriate vertex attributess
		glUseProgram(OrganicMode1ProgramID);	// use appropriate program
		glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexCoordVBOID); // bind to primary VBO
		// interleaving, total stride amount is 24 bytes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)0);	// vertex point data: begins at 0
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)12 ); // color point data: begins at byte 12
		*/
	}

	if (renderMode == 2)
	{

	}
}

void OrganicGLManager::SetupBufferManagerArrays(int in_T1_cubesize, int in_T2_cubesize)
{
	/* Summary: this sets up the OGLMBufferManager's arrays; it must be called before generating any OpenGL buffers. */
	OrganicBufferManager.T1_SetCubeSize(in_T1_cubesize);					// sets the T1 buffer cubesize in the buffer manager
	OrganicBufferManager.T2_SetCubesize(in_T2_cubesize);					// sets the T2 buffer cubesize in the buffer manager
	numberOfBuffers = (in_T2_cubesize*in_T2_cubesize*in_T2_cubesize);		// set the OrganicGLManager's total buffer value 
	cout << "OpenGL buffer data: --------------" << endl;
	cout << "number of Buffers" << numberOfBuffers << endl;
	cout << "buffer size: " << (numberOfBuffers * OGLMVertexSubBufferSize) / 1024 / 1024 << " Megabytes " << endl;
}

void OrganicGLManager::SendPointerToBufferManager(OrganicGLManager* in_OGLMptr)
{
	OrganicBufferManager.SetOGLMPointer(in_OGLMptr);	// sends a pointer to this object (OrganicGLManager) to the object's instance of OrganicBufferManager
}



void OrganicGLManager::setWindowSize(int width, int height)	// set size of OpenGL window
{
	window_width = width;
	window_height = height;
}
