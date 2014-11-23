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
	
	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	//the mesh to be displayed
	Mesh _displayedMesh;

public:
	Model();

public:
	virtual ~Model();

public:
	void init(char* filename);
	
public:
	void draw();
	
public:
	void resize(int width, int height);
	
private:
	void loadMesh(Mesh& mesh, const char* filename);


};

#endif /* defined(__ex0__Model__) */
