# include "table.h"
# include "read.h"
void Table::drawTable() {

	Read read;
	int Mesh_NumVertex;
	int Mesh_NumFaces;
	point *vertex_list;
	point *face_normal;
	int_point *face_list;
	char str[] = "C:\\Temp\\table.txt";
	struct node ans = read.readFile(str);
	Mesh_NumVertex = ans.Mesh_NumVertex;
	Mesh_NumFaces = ans.Mesh_NumFaces;
	vertex_list = ans.vertex_list;
	face_normal = ans.face_normal;
	face_list = ans.face_list;

	float shine = 12.8f;

	glPushMatrix();

	glPushAttrib(GL_LIGHTING_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColor3f(0.6, 0.2, 0.0);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < Mesh_NumFaces; i++) {
		glNormal3fv(face_normal[i]);
		glVertex3fv(vertex_list[face_list[i][0]]);
		glVertex3fv(vertex_list[face_list[i][1]]);
		glVertex3fv(vertex_list[face_list[i][2]]);

	}
	glEnd();
	glPopAttrib();
	glPopMatrix();
}
