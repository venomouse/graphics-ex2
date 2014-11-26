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

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// Translation matrix handle:
	GLint _translationUV;

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	glm::vec3 _boxCenter, _boxTR, _boxBL;

	glm::mat4 _accumulatedTransMat, _scaleMat, _translateMat;

	uint n_vertices;

	int _displayMode;

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
	
public:
	void draw();
	
public:
	void resize(int width, int height);
	
public:
	void toggleDisplayMode ();
	void toggleZoom ();

private:
	void loadMesh(Mesh& mesh, const char* filename);
	void computeCenterAndBoundingBox(Mesh& mesh);

};

#endif /* defined(__ex0__Model__) */
