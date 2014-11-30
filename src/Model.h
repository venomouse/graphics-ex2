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
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// Translation matrix handle:
	GLint _translationUV;

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	glm::vec3 _boxCenter, _boxTR, _boxBL, _initRotVec;


	glm::mat4 _modelMat, _accumulatedTransMat, _scaleMat, _translateMat, _rotateMat;

	uint n_vertices;

	int _verticesInPerimeter;

	int _displayMode;

	bool _isOrthographic;

	bool _translationMode;

	bool _zoomMode;

	bool _rotateMode;

	bool _rotVecInit;

	//the mesh to be displayed
//	Mesh _displayedMesh;

public:
	float _fov;
	int _mouseX, _mouseY, _beginEventX, _beginEventY;

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
	void updateMatrices (int x, int y);
	
public:
	void toggleOrthographMode();
	void toggleTranslationMode();
	void toggleDisplayMode ();
	void toggleZoom ();
	void toggleRotate ();
	void setInitRotVector(int x, int y);
	void resetInitRotVector();
	void accumulateTranslations();
	void resetTranslations();


private:
	void loadMesh(Mesh& mesh, const char* filename);
	void computeCenterAndBoundingBox(Mesh& mesh);
	glm::vec3 computeNormalVector(int winX, int winY);

};

#endif /* defined(__ex0__Model__) */
