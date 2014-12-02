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

#include <algorithm>

#define SHADERS_DIR "shaders/"

#define OBJECT_DEPTH 			(11.5f)
#define OBJECT_B_RAD 			(4.f)
#define NEAR 					(OBJECT_DEPTH - OBJECT_B_RAD)
#define FAR						(OBJECT_DEPTH + OBJECT_B_RAD)
#define INIT_ZOOM 				(25.0f)
#define MIN_ZOOM 				(5.0f)
#define MAX_ZOOM 				(150.0f)
#define ROTATE_RADIUS 			(0.8f)
#define DEGREES_IN_CIRCLE 		(360)
#define VERTICES_IN_PERIMETER 	(DEGREES_IN_CIRCLE*1)

Model::Model() :
_vao(0), _vbo(0), _vao2(0), _vbo2(2), _displayMode(FULL_MODE)
{


}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
	if (_vao2 != 0)
			glDeleteVertexArrays(1, &_vao2);
	if (_vbo2 != 0)
		glDeleteBuffers(1, &_vbo2);
}

void Model::init(Mesh& mesh)
{
	 _verticesInPerimeter = VERTICES_IN_PERIMETER;
	 _isOrthographic = false;
	_translateMat = glm::mat4(1.0f);
	_zoomMode = false;
	_translationMode = false;
	_rotateMode = false;
	initCircle();

	programManager::sharedInstance()
	.createProgram("default",
				   SHADERS_DIR "SimpleShader.vert",
				   SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");
		
	// Obtain uniform variable handles:
	_translationUV = glGetUniformLocation(program, "translation");

	_colorDirection = glGetUniformLocation(program, "colorDirection");
	_colorScale = glGetUniformLocation(program, "colorScale");

	{
		_fov = INIT_ZOOM;
		_rotateMat = glm::mat4(1.0f);
		_accumulatedTransMat = glm::mat4(1.0f);
		_modelMat = glm::mat4(1.0f);
		computeCenterAndBoundingBox(mesh);

		_initScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(OBJECT_B_RAD/glm::length((_boxTR - _boxBL))));
		_accumulatedTransMat = _initScaleMat * glm::mat4(1.0f);

		n_vertices = mesh.n_faces()*3;
		float vertices[n_vertices*NUM_OF_COORDS];
		Mesh::Point meshPoint;
		int i = 0;
		for (Mesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
		{
			for(Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it);fv_it; ++fv_it)
			{
				meshPoint = mesh.point(fv_it);
				vertices[i*NUM_OF_COORDS + X] = meshPoint[X] - _boxCenter[X];
				vertices[i*NUM_OF_COORDS + Y] = meshPoint[Y] - _boxCenter[Y];
				vertices[i*NUM_OF_COORDS + Z] = meshPoint[Z] - _boxCenter[Z];
				vertices[i*NUM_OF_COORDS + W] = 1.0f;
				i++;
			}
		}

		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

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

void Model::initCircle()
{
	programManager::sharedInstance()
	.createProgram("default2",
				   SHADERS_DIR "SimpleShader2.vert",
				   SHADERS_DIR "SimpleShader2.frag");

	GLuint program2 = programManager::sharedInstance().programWithID("default2");
	{

		_circTranslationUV = glGetUniformLocation(program2, "circTranslation");

		float verticeArr[(VERTICES_IN_PERIMETER)*NUM_OF_COORDS];
		//initializing the center
		//the circle should be in the middle of the window
		float center[] = { 0.0f, 0.0f, 0.0f, 1.0f};


		//calculating the vertices on the circle perimeter
		for (int numTriangle = 0; numTriangle < VERTICES_IN_PERIMETER; numTriangle++)
		{
			for (int coordNum = X; coordNum < NUM_OF_COORDS; coordNum++)
			{
				verticeArr[numTriangle*NUM_OF_COORDS + coordNum] = center[coordNum];
			}
			//we offset the X and Y of each point to the needed direction from the circle center
			float deg = (numTriangle)*DEGREES_IN_CIRCLE/_verticesInPerimeter;
			verticeArr[numTriangle*NUM_OF_COORDS + X] += ROTATE_RADIUS*cos(deg*M_PI/180.0f);
			verticeArr[numTriangle*NUM_OF_COORDS + Y] += ROTATE_RADIUS*sin(deg*M_PI/180.0f);
		}

		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao2);
		glBindVertexArray(_vao2);

		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo2);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticeArr), verticeArr, GL_STATIC_DRAW);

		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program2, "position");
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

	//set the projection matrix
	glm::mat4 Projection;
	if (_isOrthographic)
	{

		float zoomFactor = OBJECT_DEPTH* glm::tan(glm::radians(_fov/2) );
		float horZoom = zoomFactor;
		float verZoom = zoomFactor;
		if (_width > _height)
		{
			horZoom *=((float)_width/(float)_height);
		}
		else if(_height > _width)
		{
			verZoom +=  ((float)_height/(float)_width);
		}
		Projection = glm::ortho(-1.0f*horZoom, 1.0f*horZoom, -1.0f*verZoom, 1.0f*verZoom, NEAR,  FAR);
	}
	else
	{
		Projection = glm::perspective(_fov, float(_width) / float(_height), OBJECT_DEPTH - OBJECT_B_RAD,  OBJECT_DEPTH + OBJECT_B_RAD);
	}

	//Setting the view matrix
	glm::mat4 View       = glm::lookAt(
							glm::vec3(0,0,OBJECT_DEPTH), // Camera is at (x,y,z), in World Space
							glm::vec3(0,0,1), // and looks at the origin
							glm::vec3(0,1,0)  // Head is up
							);

	//calculating the transformation matrix
	_modelMat = _translateMat*_rotateMat*_accumulatedTransMat;  // Changes for each model

	glm::vec3 colorDirection = _boxBL - _boxCenter;
    glm::vec3 diagonal = _boxBL -_boxTR;
    float colorScale = sqrt(diagonal[0]*diagonal[0] + diagonal[1]*diagonal[1] + diagonal[2]*diagonal[2])/2;

	glm::mat4 MVP = Projection * View * _modelMat;
	glUniformMatrix4fv(_translationUV, 1, false, &MVP[0][0]);
	glUniform4f(_colorDirection, colorDirection[0], colorDirection[1], colorDirection[2], 1.0);
	glUniform1f(_colorScale, colorScale);

	glDrawArrays(GL_TRIANGLES, 0, n_vertices);
	
	GLuint program2 = programManager::sharedInstance().programWithID("default2");
	glUseProgram(program2);

	GLenum polygonMode2 = GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode2);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao2);

	glm::mat4 circTranslation = glm::mat4(1.0f);
	if (_width > _height)
	{
		circTranslation *= glm::vec4(1.0f,(float)_width/(float)_height, 1.0f, 1.0f);
	}
	else if (_height > _width)
	{
		circTranslation *= glm::vec4((float)_height/(float)_width,1.0f, 1.0f, 1.0f);
	}
	glUniformMatrix4fv(_circTranslationUV, 1, false, &circTranslation[0][0]);
	glDrawArrays(GL_LINE_LOOP, 0, _verticesInPerimeter);
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


void Model::updateMatrices (int x, int y)
{
	if (_translationMode)
	{
		float scale =  OBJECT_B_RAD ;
		float diffX = scale * (float)(x - _mouseX)/_width;
		float diffY = -scale * (float)(y - _mouseY)/_height;
		_translateMat = glm::translate(_translateMat,glm::vec3( diffX, diffY, 0.0f));
		_mouseX = x;
		_mouseY = y;
	}

	else if (_zoomMode)
    {
        float zoomFactor = (float)(MAX_ZOOM - MIN_ZOOM)*(float)(y - _mouseY)/(float)(_height);
		_fov -= zoomFactor;
	    if (_fov > MAX_ZOOM)
	    {
		    _fov = MAX_ZOOM;
	    }
	    else if (_fov < MIN_ZOOM)
	    {
		    _fov = MIN_ZOOM;
	    }
	    _mouseY = y;
	}

	else if (_rotateMode && _pressedInside)
    {
    	glm::vec3 currVec = computeNormalVector(x,y);
    	glm::vec3 rotAxisVec = glm::cross(_initRotVec, currVec);
    	float rotAngle = float(2.0 * acosf(glm::dot(_initRotVec, currVec) )/(2.0f * M_PI)*360.0);
    	_rotateMat = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxisVec);
    }


}

void Model::toggleOrthographMode()
{
	_isOrthographic = !_isOrthographic;
}

void Model::toggleDisplayMode ()
{
	_displayMode = 1 - _displayMode;
}


void Model::toggleTranslationMode()
{
	_translationMode = !_translationMode;

}
void Model::toggleZoom()
{
	_zoomMode = !_zoomMode;
}

void Model::toggleRotate()
{
	_rotateMode = !_rotateMode;
}

void Model::setInitRotVector (int x, int y)
{
	if (!_rotVecInit)
	{
		_initRotVec = computeNormalVector(x,y);
		_rotVecInit = true;
	}

}

void Model::accumulateTransformations()
{
	_accumulatedTransMat = _modelMat;
	_translateMat = glm::mat4(1.0f);
	_rotVecInit = false;
	_rotateMat = glm::mat4(1.0f);

}

void Model::resetTranslations()
{
	_rotateMat = glm::mat4(1.0f);
	_translateMat = glm::mat4(1.0f);
	_fov = INIT_ZOOM;
	_accumulatedTransMat = _initScaleMat * glm::mat4(1.0f);
}

void Model::computeCenterAndBoundingBox(Mesh& mesh)
{
	/* Vertex iterator is an iterator which goes over all the vertices of the mesh */
	Mesh::VertexIter vertexIter;
	/* This is the specific class used to store the geometrical position of the vertices of
	the mesh */
	Mesh::Point p;
	Mesh:: Point center(0,0,0);

	const float fm = std::numeric_limits<float>::max();
	Mesh::Point lowerLeft(fm,fm, fm);
	Mesh::Point upperRight(0,0,0);

	/* number of vertices in the mesh */
	int vNum = mesh.n_vertices();
	vertexIter = mesh.vertices_begin();
	lowerLeft = upperRight = mesh.point(vertexIter);

	/* This is how to go over all the vertices in the mesh */
	for(vertexIter = mesh.vertices_begin();vertexIter != mesh.vertices_end();++vertexIter)
	{
		/* this is how to get the point associated with the vertex */
		p = mesh.point(vertexIter);
		center += p;
		for(int i =0;i <3;i++)
		{
			lowerLeft[i] = std::min(lowerLeft[i], p[i]);
			upperRight[i] = std::max(upperRight[i],p[i]);
		}
	}
	center /= (double)vNum;
	_boxCenter = glm::vec3(center[0], center[1], center[2]);
	_boxBL = glm::vec3(lowerLeft[0], lowerLeft[1], lowerLeft[2]);
	_boxTR = glm::vec3(upperRight[0], upperRight[1], upperRight[2]);
}

glm::vec3 Model::computeNormalVector(int winX, int winY)
{
//	float x = (winX - 0.5f*_width)/(0.5f*_width);
//	float y = (0.5f*_height - winY)/(0.5f*_height);
//	if (_width > _height)
//	{
//		y*= (float)_height/(float)_width;
//	}
//	else if (_height > _width)
//	{
//		x *= (float)_width/(float)_height;
//	}
	glm::vec2 xy = getCoordianatesInWorldSpace(winX, winY);
	float x = xy[0];
	float y = xy[1];
	float z = (sqrt(x*x + y*y) > ROTATE_RADIUS) ?
									0: sqrt(ROTATE_RADIUS*ROTATE_RADIUS-x*x - y*y);
	return glm::normalize(glm::vec3(x, y,z));
}

bool Model::insideArcball(int winX, int winY)
{
//	float x = (winX - 0.5f*_width)/(0.5f*_width);
//	float y = (0.5f*_height - winY)/(0.5f*_height);
//	if (_width > _height)
//	{
//		y*= (float)_height/(float)_width;
//	}
//	else if (_height > _width)
//	{
//		x *= (float)_width/(float)_height;
//	}
	glm::vec2 xy = getCoordianatesInWorldSpace(winX, winY);
	float x = xy[0];
	float y = xy[1];
	return (sqrt(x*x + y*y) <= ROTATE_RADIUS);
}

void Model::setPressedInside(int winX, int winY)
{
	_pressedInside = insideArcball(winX, winY);
}

glm::vec2 Model::getCoordianatesInWorldSpace(int winX, int winY)
{
	float x = (winX - 0.5f*_width)/(0.5f*_width);
	float y = (0.5f*_height - winY)/(0.5f*_height);
	if (_width > _height)
	{
		y*= (float)_height/(float)_width;
	}
	else if (_height > _width)
	{
		x *= (float)_width/(float)_height;
	}
	return glm::vec2(x,y);
}
