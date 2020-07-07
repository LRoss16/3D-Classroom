#include "read.h"
#include "GL\freeglut.h"
#include "GL\glut.h"
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct node Read::readFile(char* datafile) {
	int Mesh_NumVertex;
	int Mesh_NumFaces;
	point *vertex_list;
	point *face_normal;
	int_point *face_list;
	ifstream inStream(datafile, ios::in);
	if (!inStream) {
		cerr << "Unable to open file, filepath = " << datafile;
		exit(1);   // call system to stop
	}
	string word;
	inStream >> word;
	while (word.compare("*MESH_NUMVERTEX")) {
		inStream >> word;
	}
	inStream >> Mesh_NumVertex;


	inStream >> word;
	while (word.compare("*MESH_NUMFACES")) {
		inStream >> word;
	}
	inStream >> Mesh_NumFaces;

	try {
		// Allocate memory for Polygon Mesh data structures
		vertex_list = new point[Mesh_NumVertex];
		face_list = new int_point[Mesh_NumFaces];
		face_normal = new point[Mesh_NumFaces];
	}
	catch (bad_alloc &memoryAllocationException) {
		cerr << "Exception occurred: "
			<< memoryAllocationException.what() << endl;
		exit(1);
	}

	inStream >> word;
	while (word.compare("*MESH_VERTEX_LIST")) {
		inStream >> word;
	}

	inStream >> word; // read {

	int i, j;
	float nor;
	for (i = 0; i < Mesh_NumVertex; i++) {
		inStream >> word;
		while (word.compare("*MESH_VERTEX")) {
			inStream >> word;
		}
		inStream >> j;
		inStream >> vertex_list[j][0];
		inStream >> vertex_list[j][1];
		inStream >> vertex_list[j][2];
		nor = vertex_list[j][0] * vertex_list[j][0] + vertex_list[j][1] * vertex_list[j][1] + vertex_list[j][2] * vertex_list[j][2];
		nor = sqrt(vertex_list[j][2]);
		/*vertex_list[j][0] /= nor;
		vertex_list[j][1] /= nor;
		vertex_list[j][2] /= nor;*/
	}

	inStream >> word; // read }


	inStream >> word;
	while (word.compare("*MESH_FACE_LIST")) {
		inStream >> word;
	}

	inStream >> word; // read {

	for (i = 0; i < Mesh_NumFaces; i++) {
		inStream >> word;
		while (word.compare("*MESH_FACE")) {
			inStream >> word;
		}
		inStream >> word;

		inStream >> word;
		inStream >> face_list[i][0];

		inStream >> word;
		inStream >> face_list[i][1];

		inStream >> word;
		inStream >> face_list[i][2];


	}

	inStream >> word; // read }



	inStream >> word;
	while (word.compare("*MESH_NORMALS")) {
		inStream >> word;
	}

	inStream >> word; // read {

	for (i = 0; i < Mesh_NumFaces; i++) {
		inStream >> word;
		while (word.compare("*MESH_FACENORMAL")) {
			inStream >> word;
		}
		inStream >> j;

		inStream >> face_normal[j][0];

		inStream >> face_normal[j][1];

		inStream >> face_normal[j][2];


	}

	inStream >> word; // read }
	struct node ans = { Mesh_NumVertex, Mesh_NumFaces, vertex_list, face_normal, face_list };
	return 	ans;
}

