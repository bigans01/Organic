#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicGLManager.h"

OrganicGLManager::OrganicGLManager()
{

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
	GLwindow = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
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



	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(GLwindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create Vertex Array Object
	glGenVertexArrays(1, &OrganicGLVertexArrayID);
	glBindVertexArray(OrganicGLVertexArrayID);

	// Setup programID
	OrganicGLprogramID = LoadShaders("SimpleTransform.vertexshader", "SimpleFragmentShader.fragmentshader");		/* NOTE: these shaders need to be in the same directory as the .exe file;
																													How to put these in another directory should be investigated. (8/19/2017) */


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
	glBufferStorage(GL_ARRAY_BUFFER, CollectionBufferSize * 500, NULL, GL_DYNAMIC_STORAGE_BIT);	/* REQUIRED: pre-allocates memory for the buffer, to any desired amount; this is so
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
	glClear(GL_COLOR_BUFFER_BIT);										// clear the screen?
	glUseProgram(OrganicGLprogramID);									// select the already compiled program

	// gather inputs from keyboard
	computeMatricesFromInputs();


	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform -- 
	glUniformMatrix4fv(OrganicMVPHandle, 1, GL_FALSE, &MVP[0][0]);		// select the matrix to use.


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);										/* select the buffer we will be using (we are only using vertexes here; if we 
																		   were using a fragment shader as well, we would need to run this command again, but for a value of 1 instead.
																		*/
																				
	glBindBuffer(GL_ARRAY_BUFFER, OrganicGLVertexBufferID);				// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
	//auto GLstart = std::chrono::high_resolution_clock::now();			// optional, performance testing only
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
	//auto GLend = std::chrono::high_resolution_clock::now();				// optional performance testing values
															
	//auto GLstart = std::chrono::high_resolution_clock::now();	
	
	//for (int y = 0; y < 166; y++)
	//{
		for (int x = 0; x < RMContainer.TotalRenderable; x++)
		{
			// summary:
			// first argument: GL_TRIANGLES
			// second argument: vertex offset, so if byte begins at 73728, offset is 6144. (One vertex = 12 bytes)
			// last argument of glDrawArrays = number of vertices; 6144 for an entire collection face

			glDrawArrays(GL_TRIANGLES, x*(CollectionBufferSize / 12), ((RMContainer.RenderMetaArray[x].ArraySize) / 12));

		}
	//}

	auto GLend = std::chrono::high_resolution_clock::now();	// optional performance testing values
	std::chrono::duration<double> GLelapsed = GLend - GLstart;
	//std::cout << "Frame render Time: " << GLelapsed.count() << std::endl;
	//std::cout << "RUN MULTI JOB 1 ELAPSED ITERATOR TIME: " << elapsed4.count() << std::endl;

	glDisableVertexAttribArray(0);										// disable the array that was just used.

	glfwSwapBuffers(GLwindow);											// ??
	glfwPollEvents();													// listen for input to the current OpenGL context window
}

void OrganicGLManager::ShutdownOpenGL()
{
	// Cleanup VBO
	glDeleteBuffers(1, &OrganicGLVertexBufferID);			// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
	//glDeleteBuffers(1, &OrganicGLVertexBufferArray[1]);			// OrganicGLVertexBufferArray[0], OrganicGLVertexBufferID
	glDeleteVertexArrays(1, &OrganicGLVertexArrayID);
	glDeleteProgram(OrganicGLprogramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
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
	glfwSetCursorPos(GLwindow, 1024 / 2, 768 / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

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
	Projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
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
}

void OrganicGLManager::sendDataToBuffer(GLfloat* floatPtr, int size)
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, floatPtr);
}

void OrganicGLManager::sendRenderCollectionDataToBuffer(RenderCollection *renderCollPtr)
{
	cout << "Test; array size: " << renderCollPtr->RenderCollectionArraySize <<endl;
	glBufferSubData(GL_ARRAY_BUFFER, RMContainer.CurrentIndex*CollectionBufferSize, renderCollPtr->RenderCollectionArraySize, renderCollPtr->GLFloatPtr);
	RMContainer.RenderMetaArray[RMContainer.CurrentIndex].MetaIndex = RMContainer.CurrentIndex;
	RMContainer.RenderMetaArray[RMContainer.CurrentIndex].ArraySize = renderCollPtr->RenderCollectionArraySize;
	RMContainer.CurrentIndex++;
	RMContainer.TotalRenderable++;
}