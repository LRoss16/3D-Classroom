#include "GL\freeglut.h"
#include "GL\glut.h"
#include <math.h>
#include <iostream>
#include <string.h>
#include <fstream>
typedef float point[3];
typedef int int_point[3];
struct node {
	int Mesh_NumVertex;
	int Mesh_NumFaces;
	point *vertex_list;
	point *face_normal;
	int_point *face_list;
};
class Read {

public:
	struct node readFile(char* datafile);

};


