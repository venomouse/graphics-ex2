//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

class Model {
	
	enum Coords {X=0, Y, Z, W, NUM_OF_COORDS};
	typedef enum {WIREFRAME_MODE = 0,FULL_MODE = 1} DisplayMode;

	GLuint _vao, _vbo;

	GLuint  _vao2, _vbo2;

	// Attribute handle:
	GLint _posAttrib;
	
	// Translation matrix handle:
	GLint _translationUV;

	GLint _circTranslationUV;

	GLint _colorDirection;

	GLint _colorScale;
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	float _initScaleFactor, _scaleFactor;
	//bounding box parameters
	glm::vec3 _boxCenter, _boxTR, _boxBL, _initRotVec;

	//transformation matrices
	glm::mat4 _modelMat, _accumulatedTransMat, _initScaleMat, _translateMat, _rotateMat, _scaleMat;

	uint n_vertices;

	int _verticesInPerimeter;

	//wireframe or fill displaymode
	int _displayMode;

	//projection mode
	bool _isOrthographic;

	//mouse movement modes
	bool _translationMode, _zoomMode, _rotateMode;

	bool _rotVecInit;

	bool _pressedInside;


public:
	float _fov;
	//current mouse coordinates
	int _mouseX, _mouseY;


public:
	Model();

public:
	virtual ~Model();

public:
	void init(Mesh& mesh);
	void initCircle();
	
public:
	void draw();
	
public:
	void resize(int width, int height);

public:
	//updates the translation and rotation matrices and zoom angle during mouse move
	void updateMatrices (int x, int y);
	//change display modes
	void toggleOrthographMode();
	void toggleTranslationMode();
	void toggleDisplayMode ();
	void toggleZoom ();
	void toggleRotate ();

	void setInitRotVector(int x, int y);

	//save the transformations after the mouse release
	void accumulateTransformations();

	//reset to initial position
	void resetTranslations();

	bool insideArcball(int winX, int winY);

	void setPressedInside(int winX, int winY);
private:
	//determine the bounding box
	void computeCenterAndBoundingBox(Mesh& mesh);
	//compute normal for rotation purposes
	glm::vec3 computeNormalVector(int winX, int winY);

	glm::vec2 getCoordianatesInWorldSpace(int winX, int winY);

};

#endif /* defined(__ex0__Model__) */
