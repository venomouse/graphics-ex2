//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define SHADERS_DIR "shaders/"

#define OBJECT_DEPTH (11.5f)
#define OBJECT_B_RAD (6.f)

Model::Model() :
_vao(0), _vbo(0), _displayMode(FULL_MODE)
{

}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}

void Model::init(Mesh& mesh)
{
	_fuv = 30.0f;
	programManager::sharedInstance()
	.createProgram("default",
				   SHADERS_DIR "SimpleShader.vert",
				   SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");
		
	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(program, "fillColor");

	_translationUV = glGetUniformLocation(program, "translation");

	{


		n_vertices = mesh.n_faces()*3;
		float vertices[n_vertices*NUM_OF_COORDS];
		Mesh::Point meshPoint;
		int i = 0;
		for (Mesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
		{
			for(Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it);fv_it; ++fv_it)
			{
				meshPoint = mesh.point(fv_it);
//				std::cout<< i << ") " << meshPoint[X] << ", " << meshPoint[Y] << ", " << meshPoint[Z] << std::endl;
				vertices[i*NUM_OF_COORDS + X] = meshPoint[X];
				vertices[i*NUM_OF_COORDS + Y] = meshPoint[Y];
				vertices[i*NUM_OF_COORDS + Z] = meshPoint[Z];
				vertices[i*NUM_OF_COORDS + W] = 1.0f;
				i++;
			}
		}
//		std::cout<< "number of vertices: " << n_vertices << std::endl;
//		for(uint j = 0 ; j < n_vertices; ++j)
//		{
//			std::cout<< j << ") " << vertices[j*NUM_OF_COORDS + X] << ", "
//					<< vertices[j*NUM_OF_COORDS + Y] << ", "
//					<< vertices[j*NUM_OF_COORDS + Z] << std::endl;
//		}
		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		
//		VertexHandle beginning = _displayedMesh.vertexHandle(0);


		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, n_vertices*sizeof(float)*4, vertices, GL_STATIC_DRAW);
		
		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
							  4,          // number of scalars per vertex
							  GL_FLOAT,   // scalar type
							  GL_FALSE,
							  0,
							  0);
		
		// Unbind vertex array:
		glBindVertexArray(0);
	}
}




void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = (_displayMode == WIREFRAME_MODE) ? GL_LINE  : GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	// Set uniform variable with RGB values:
	float red = 0.3f; float green = 0.5f; float blue = 0.7f;
	glUniform4f(_fillColorUV, red, green, blue, 1.0);

	glm::mat4 Projection = glm::perspective(_fuv, float(_width) / float(_height), OBJECT_DEPTH - OBJECT_B_RAD,  OBJECT_DEPTH + OBJECT_B_RAD);

	glm::mat4 View       = glm::lookAt(
	    glm::vec3(0,0,OBJECT_DEPTH), // Camera is at (x,y,z), in World Space
	    glm::vec3(0,0,1), // and looks at the origin
	    glm::vec3(0,1,0)  // Head is up
	);

	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model

	glm::mat4 MVP = Projection * View * Model;
	glUniformMatrix4fv(_translationUV, 1, false, &MVP[0][0]);

	
//	size_t numberOfVertices = 3;
	glDrawArrays(GL_TRIANGLES, 0, n_vertices);
	
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}


void Model::toggleDisplayMode ()
{
	_displayMode = 1 - _displayMode;
}
