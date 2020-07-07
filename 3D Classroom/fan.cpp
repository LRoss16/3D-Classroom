#include "fan.h"
#include "read.h"
void Fan::drawFan()
{
	Read read;
	int Mesh_NumVertex;
	int Mesh_NumFaces;
	point *vertex_list;
	point *face_normal;
	int_point *face_list;
	char str[] = "C:\\Temp\\fan.txt";
	struct node ans = read.readFile(str);
	Mesh_NumVertex = ans.Mesh_NumVertex;
	Mesh_NumFaces = ans.Mesh_NumFaces;
	vertex_list = ans.vertex_list;
	face_normal = ans.face_normal;
	face_list = ans.face_list;
	float shine = 30.f;
	glPushMatrix();
	glRotatef(rotationAngle, 0.0f, 1.0, 0.0f);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glColor3f(0.64f, 0.16f, 0.16f);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < Mesh_NumFaces; i++) {
		glColor3f(0.64f, 0.16f, 0.16f);
		glNormal3fv(face_normal[i]);

		glVertex3fv(vertex_list[face_list[i][0]]);
		glVertex3fv(vertex_list[face_list[i][1]]);
		glVertex3fv(vertex_list[face_list[i][2]]);

	}
	glEnd();
	glPopMatrix();
}

void Fan::rotateFan(float r)
{
	rotationAngle += r;
}


