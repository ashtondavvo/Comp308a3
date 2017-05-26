//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

//#include "cgra_cow.hpp"
#include "cgra_math.hpp"
#include "geometry.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;


Geometry::Geometry(string filename) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
		createDisplayListWire();
	}
}


Geometry::~Geometry() { }


void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));


	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		//std::cout << mode << std::endl;
		if (!objLine.fail()) {
			if(mode == "s"){

			}
			else if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<vertex> verts;
				int count = 1;
				while (objLine.good()){
					vertex v;
					//std::cout << count << std::endl;
//					count = count + 1;
//					std::cout << "^-----------------------------^" << std::endl;
//					std::cout << line << std::endl;
//					std::cout << "<------------------------------->" << std::endl;
					//-------------------------------------------------------------
					// [Assignment 1] :
					// Modify the following to parse the bunny.obj. It has no uv
					// coordinates so each vertex for each face is in the format
					// v//vn instead of the usual v/vt/vn.
					//
					// Modify the following to parse the dragon.obj. It has no
					// normals or uv coordinates so the format for each vertex is
					// v instead of v/vt/vn or v//vn.
					//
					// Hint : Check if there is more than one uv or normal in
					// the uv or normal vector and then parse appropriately.
					//-------------------------------------------------------------

					// Assignment code (assumes you have all of v/vt/vn for each vertex)
					objLine >> v.p;		// Scan in position index
					objLine.ignore(1);	// Ignore the '/' character
					if(m_uvs.size() > 1){
						objLine >> v.t;		// Scan in uv (texture coord) index
						//std::cout << "There are uvs" << std::endl;
					}
					objLine.ignore(1);	// Ignore the '/' character
					if(m_normals.size() > 1){
						objLine >> v.n;		// Scan in normal index
						//std::cout << "There are normals" << std::endl;
					}
					verts.push_back(v);
				}
//				std::cout << "-------------------------------" << std::endl;
//				std::cout << verts.size() << std::endl;
//				std::cout << "-------------------------------" << std::endl;
				// IFF we have 3 verticies, construct a triangle
				if(verts.size() >= 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;


	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to populate the normals for 
// the model currently loaded. Compute per face normals
// first and get that working before moving onto calculating
// per vertex normals.
//-------------------------------------------------------------
void Geometry::createNormals() {
	// YOUR CODE GOES HERE
	// ...
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE_AND_EXECUTE);

	// YOUR CODE GOES HERE
	// ...
	glBegin(GL_TRIANGLES);

	for(long i = 0;  i < m_triangles.size(); i++){
		glTexCoord2f(m_uvs[m_triangles[i].v[0].p].x, m_uvs[m_triangles[i].v[0].p].y);
		glNormal3f(m_normals[m_triangles[i].v[0].p].x, m_normals[m_triangles[i].v[0].p].y, m_normals[m_triangles[i].v[0].p].z);
		//glVertex3f(m_points[m_triangles[i].v[0].p].x, m_points[m_triangles[i].v[0].p].y, m_points[m_triangles[i].v[0].p].z);
		glVertex3f(m_points[m_triangles[i].v[0].p].x, m_points[m_triangles[i].v[0].p].y, m_points[m_triangles[i].v[0].p].z);
		//glNormal3f(m_normals[m_triangles[i].v[0].p].x, m_normals[m_triangles[i].v[0].p].y, m_normals[m_triangles[i].v[0].p].z);

		glTexCoord2f(m_uvs[m_triangles[i].v[1].p].x, m_uvs[m_triangles[i].v[1].p].y);
		glNormal3f(m_normals[m_triangles[i].v[1].p].x, m_normals[m_triangles[i].v[1].p].y, m_normals[m_triangles[i].v[1].p].z);
		//glVertex3f(m_points[m_triangles[i].v[1].p].x, m_points[m_triangles[i].v[1].p].y, m_points[m_triangles[i].v[1].p].z);
		glVertex3f(m_points[m_triangles[i].v[1].p].x, m_points[m_triangles[i].v[1].p].y, m_points[m_triangles[i].v[1].p].z);
		//glNormal3f(m_normals[m_triangles[i].v[1].p].x, m_normals[m_triangles[i].v[1].p].y, m_normals[m_triangles[i].v[1].p].z);

		glTexCoord2f(m_uvs[m_triangles[i].v[2].p].x, m_uvs[m_triangles[i].v[2].p].y);
		glNormal3f(m_normals[m_triangles[i].v[2].p].x, m_normals[m_triangles[i].v[2].p].y, m_normals[m_triangles[i].v[2].p].z);
		//glVertex3f(m_points[m_triangles[i].v[2].p].x, m_points[m_triangles[i].v[2].p].y, m_points[m_triangles[i].v[2].p].z);
		glVertex3f(m_points[m_triangles[i].v[2].p].x, m_points[m_triangles[i].v[2].p].y, m_points[m_triangles[i].v[2].p].z);
		//glNormal3f(m_normals[m_triangles[i].v[2].p].x, m_normals[m_triangles[i].v[2].p].y, m_normals[m_triangles[i].v[2].p].z);
	}

	glEnd();
	glEndList();
	//glPointSize(5.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(m_displayListPoly);
	cout << "Finished creating Poly Geometry" << endl;
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as polygon model
//-------------------------------------------------------------
void Geometry::createDisplayListWire() {
	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);

	// Create a new list
	cout << "Creating Wire Geometry" << endl;
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE_AND_EXECUTE);

	// YOUR CODE GOES HERE
	// ...
	glBegin(GL_TRIANGLES);

	for(long i = 0;  i < m_triangles.size(); i++){

		glVertex3f(m_points[m_triangles[i].v[0].p].x, m_points[m_triangles[i].v[0].p].y, m_points[m_triangles[i].v[0].p].z);
		glTexCoord2f(m_uvs[m_triangles[i].v[0].p].x, m_uvs[m_triangles[i].v[0].p].y);
		glNormal3f(m_normals[m_triangles[i].v[0].p].x, m_normals[m_triangles[i].v[0].p].y, m_normals[m_triangles[i].v[0].p].z);

		glVertex3f(m_points[m_triangles[i].v[1].p].x, m_points[m_triangles[i].v[1].p].y, m_points[m_triangles[i].v[1].p].z);
		glTexCoord2f(m_uvs[m_triangles[i].v[1].p].x, m_uvs[m_triangles[i].v[1].p].y);
		glNormal3f(m_normals[m_triangles[i].v[1].p].x, m_normals[m_triangles[i].v[1].p].y, m_normals[m_triangles[i].v[1].p].z);

		glVertex3f(m_points[m_triangles[i].v[2].p].x, m_points[m_triangles[i].v[2].p].y, m_points[m_triangles[i].v[2].p].z);
		glTexCoord2f(m_uvs[m_triangles[i].v[2].p].x, m_uvs[m_triangles[i].v[2].p].y);
		glNormal3f(m_normals[m_triangles[i].v[2].p].x, m_normals[m_triangles[i].v[2].p].y, m_normals[m_triangles[i].v[2].p].z);
	}
	glEnd();
	glEndList();
	glPointSize(5.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(m_displayListWire);

	cout << "Finished creating Wire Geometry" << endl;
}


void Geometry::renderGeometry() {
	//if (m_wireFrameOn) {

		//-------------------------------------------------------------
		// [Assignment 1] :
		// When moving on to displaying your obj, comment out the
		// wire_cow function & uncomment the glCallList function
		//-------------------------------------------------------------
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//		glShadeModel(GL_SMOOTH);
//		//wire_cow();
//		glCallList(m_displayListWire);

//	} else {
//
//		//-------------------------------------------------------------
//		// [Assignment 1] :
//		// When moving on to displaying your obj, comment out the
//		// cow function & uncomment the glCallList function
//		//-------------------------------------------------------------
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		//cow();
		glCallList(m_displayListPoly);
//
//	}
}


void Geometry::toggleWireFrame() {
	std::cout << "Toggle was called \n";
	m_wireFrameOn = !m_wireFrameOn;
	if(m_wireFrameOn) std::cout << "true \n";
	else std::cout << "false \n";
}
