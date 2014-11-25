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

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

class Model {
	
	enum Coords {X=0, Y, Z, W, NUM_OF_COORDS};

	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// Translation matrix handle:
	GLint _translationUV;

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	float _fuv;

	uint n_vertices;

	//the mesh to be displayed
//	Mesh _displayedMesh;

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
	
private:
	void loadMesh(Mesh& mesh, const char* filename);


};

#endif /* defined(__ex0__Model__) */
