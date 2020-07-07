#include "chair.h"
#include "read.h"
void Chair::drawChair() {
	Read read;
	int Mesh_NumVertex;
	int Mesh_NumFaces;
	point *vertex_list;
	point *face_normal;
	int_point *face_list;
	char str[] = "C:\\Temp\\chair.txt";
	struct node ans = read.readFile(str);
	Mesh_NumVertex = ans.Mesh_NumVertex;
	Mesh_NumFaces = ans.Mesh_NumFaces;
	vertex_list = ans.vertex_list;
	face_normal = ans.face_normal;
	face_list = ans.face_list;
	float shine = 50.0f;
	glPushMatrix();
	glColor3f(1.0f, 0.7f, 0.0f);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);


	glBegin(GL_TRIANGLES);
	for (int i = 0; i < Mesh_NumFaces; i++) {

		glNormal3fv(face_normal[i]);
		glVertex3fv(vertex_list[face_list[i][0]]);
		glVertex3fv(vertex_list[face_list[i][1]]);
		glVertex3fv(vertex_list[face_list[i][2]]);

	}
	glEnd();
	glPopMatrix();
}

