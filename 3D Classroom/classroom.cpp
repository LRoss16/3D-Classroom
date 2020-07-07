#include <iostream>
#include <cstdio>
#include <math.h>
#include <string>

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "GL\glut.h"
#include "chair.h"
#include "table.h"
#include "fan.h"
#include <conio.h>

using namespace std;

// XYZ position of the camera
float x = 0.0f, y = 3.5f, z = -9.5f;
float ux = 0.0f, uy = 2.5f, uz = 0.0f;
float cx = 0.0f, cy = 3.5f, cz = 0.0f;

float WINDOW_WIDTH = 1000;
float WINDOW_HEIGHT = 600;


Fan f1, f2, f3, f4;
Chair chair;
Table table;
Fan fan;
GLuint studentChair;
GLuint studentTable;
GLuint theFan;
float zoom = 90.0f;
GLuint texture;
void plot(void);

GLuint loadBMP_custom(const char * imagepath) {
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
							  // Actual RGB data
	unsigned char * data;
	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		cerr << "Unable to open file, filepath = " << imagepath;
		return 0;
	}
	if (fread(header, 1, 54, file) != 54) {
		// If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue 	component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);


	// Give the image to OpenGL
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
		GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);
	return textureID;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT1); //Enable light 1
	glEnable(GL_LIGHT2); //Enable light 2
	glEnable(GL_LIGHT3); // Enable light 3
	glEnable(GL_LIGHT4); // enable light 4
	glEnable(GL_LIGHT5); // Enable spotlight
	glEnable(GL_LIGHT6); // Enable daylight
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
							 //getchar();
							 // display list for table

	studentTable = glGenLists(1);
	glNewList(studentTable, GL_COMPILE);
	table.drawTable();
	glEndList();

	// display list for chair
	studentChair = glGenLists(1);
	glNewList(studentChair, GL_COMPILE);
	chair.drawChair();
	glEndList();

	// display list for fan
	theFan = glGenLists(1);
	glNewList(theFan, GL_COMPILE);
	fan.drawFan();
	glEndList();


	// load texture from image

	texture = loadBMP_custom("C:\\Temp\\texture.bmp");

}

void renderScene(void) {

	// Clear Color and Depth Buffers

	// left viewport
	float ratio = (WINDOW_WIDTH / 2.0) / WINDOW_HEIGHT;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
	gluLookAt(0.0f, 15.0f, 0, 0, 3.5f, 0, -2.5, 0.0f, 0);
	plot();


	// right viewport
	glMatrixMode(GL_PROJECTION);
	glViewport(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
	glLoadIdentity();
	gluPerspective(zoom, ratio, 0.1f, 100.0f);
	gluLookAt(x, y, z,
		cx, cy, cz,
		ux, uy, uz);
	glMatrixMode(GL_MODELVIEW);
	plot();

	glutSwapBuffers();
}

void plot(void) {


	GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat light_diffuse[] = { 0.25f, 0.25f, 0.25f, 0.25f };
	GLfloat light_specular[] = { 0.25f, 0.25f, 0.25f, 0.25f };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	float modelTwoside[] = { GL_TRUE };
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, modelTwoside);

	//Add first omnidirectional light source
	GLfloat light_position_1[] = { -7.0f, 7.0f, -7.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);



	//Add second omnidirectional light source
	GLfloat light_position_2[] = { 7.0f, 7.0f, -7.0f, 1.0f };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position_2);

	//Add third omnidirectional light source
	GLfloat light_position_3[] = { 7.0f, 7.0f, 7.0f, 1.0f };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position_3);


	//Add fourth omnidirectional light source
	GLfloat light_position_4[] = { -7.0f, 7.0f, 7.0f, 1.0f };
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position_4);


	// Add spotlight

	GLfloat spotlight_direction[] = { 0.0, -1.0, 0.0 };
	GLfloat spotlight_position[] = { -2.0f, 6.0f, -1.9f, 1.0f };
	GLfloat spotlight_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0 };
	GLfloat spotlight_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0 };
	GLfloat spotlight_specular[] = { 0.8f, 0.8f, 0.8f, 1.0 };
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 15.0);
	glLightfv(GL_LIGHT5, GL_AMBIENT, spotlight_ambient);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, spotlight_diffuse);
	glLightfv(GL_LIGHT5, GL_SPECULAR, spotlight_specular);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spotlight_direction);
	glLightfv(GL_LIGHT5, GL_POSITION, spotlight_position);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 1);

	// Add daylight

	GLfloat daylight_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT6, GL_AMBIENT, daylight_ambient);

	// floor

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glColor3f(0.21f, 0.361f, 0.502f);
	glTranslatef(0.0f, -0.5f, 0.0f);
	glScalef(20, 1, 20);
	glutSolidCube(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

	// wall
	glPushMatrix();
	glColor3f(0.4f, 0.561f, 0.702f);
	glTranslatef(0.0f, 3.5f, 10.0f);
	glScalef(20, 7, 1);
	glutSolidCube(1);
	glPopMatrix();

	// front wall
	glPushMatrix();
	glColor3f(0.4f, 0.561f, 0.702f);
	glTranslatef(0.0f, 3.5f, -10.0f);
	glScalef(20, 7, 1);
	glutSolidCube(1);
	glPopMatrix();

	// left wall
	glPushMatrix();
	glColor3f(0.4f, 0.561f, 0.702f);
	glTranslatef(-10.0f, 3.5f, 0.0f);
	glScalef(1, 7, 20);
	glutSolidCube(1);
	glPopMatrix();

	// right wall
	glPushMatrix();
	glColor3f(0.4f, 0.561f, 0.702f);
	glTranslatef(10.0f, 3.5f, 0.0f);
	glScalef(1, 7, 20);
	glutSolidCube(1);
	glPopMatrix();


	// Draw student chairs
	float shiftx = 0.6f;
	float shiftz = -0.9f;
	for (int i = -3; i <= 3; i += 2) {
		for (int j = -3; j <= 3; j += 2) {
			glPushMatrix();
			glTranslatef(i*2.0 - 1.0f + shiftx, 0.0f, j * 2.0 + 3.5f + shiftz);
			glScalef(0.012f, 0.012f, 0.012f);
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glCallList(studentChair);
			glPopMatrix();
		}
	}

	// Draw student tables

	for (int i = -3; i <= 3; i += 2) {
		for (int j = -3; j <= 3; j += 2) {
			glPushMatrix();
			glTranslatef(i*2.0 + 0.3f + shiftx, 0.0f, j * 2.0 + 1.7f + shiftz);
			glScalef(0.014f, 0.014f, 0.014f);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glCallList(studentTable);
			glPopMatrix();
		}
	}

	// first fan
	glPushMatrix();
	glTranslatef(-3.0f, 6.0f, -3.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	f1.drawFan();
	glPopMatrix();

	// second fan
	glPushMatrix();
	glTranslatef(-3.0f, 6.0f, 3.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	f2.drawFan();
	glPopMatrix();

	// third fan
	glPushMatrix();
	glTranslatef(3.0f, 6.0f, -3.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	f3.drawFan();
	glPopMatrix();

	// fourth fan
	glPushMatrix();
	glTranslatef(3.0f, 6.0f, 3.0f);
	glScalef(0.03f, 0.03f, 0.03f);
	f4.drawFan();
	glPopMatrix();


}



// Handles the events triggered when any key on the keyboard is pressed.
void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == '1') {
		if (glIsEnabled(GL_LIGHT1)) {
			glDisable(GL_LIGHT1);
		}
		else {
			glEnable(GL_LIGHT1);
		}

	}
	else if (key == '2') {
		if (glIsEnabled(GL_LIGHT2)) {
			glDisable(GL_LIGHT2);
		}
		else {
			glEnable(GL_LIGHT2);
		}

	}
	else if (key == '3') {
		if (glIsEnabled(GL_LIGHT3)) {
			glDisable(GL_LIGHT3);
		}
		else {
			glEnable(GL_LIGHT3);
		}

	}
	else if (key == '4') {
		if (glIsEnabled(GL_LIGHT4)) {
			glDisable(GL_LIGHT4);
		}
		else {
			glEnable(GL_LIGHT4);
		}

	}
	else if (key == 's') {
		if (glIsEnabled(GL_LIGHT5)) {
			glDisable(GL_LIGHT5);
		}
		else {
			glEnable(GL_LIGHT5);
		}

	}
	else if (key == 'd') {
		if (glIsEnabled(GL_LIGHT6)) {
			glDisable(GL_LIGHT6);
		}
		else {
			glEnable(GL_LIGHT6);
		}

	}
	else if (key == 'z') {
		zoom = zoom + 0.9f;
	}
	else if (key == 'Z') {
		zoom = zoom - 0.9f;
	}
	else if (key == 't') {
		x = 0.0f;
		y = 11.0f;
		z = 0.0f;

		cx = 0.0f;
		cy = 3.5f;
		cz = 0.0f;

		ux = -2.50f;
		uy = 0.0f;
		uz = 0.0f;

		zoom = 90.0f;
	}
	else if (key == 'r') {
		x = -9.5f;
		y = 3.5f;
		z = 0.0f;

		cx = 0.0f;
		cy = 3.5f;
		cz = 0.0f;

		ux = 0.0f;
		uy = 2.5f;
		uz = 0.0f;
		zoom = 90.0f;
	}
	else if (key == 'b') {
		x = 0.0f;
		y = 0.5f;
		z = 0.0f;


		cx = 0.0f;
		cy = 3.5f;
		cz = 0.0f;


		ux = 2.5f;
		uy = 0.0f;
		uz = 0.0f;
		zoom = 100.0f;

	}
	else if (key == 'f') {
		x = 0.0f;
		y = 3.5f;
		z = -9.5f;

		cx = 0.0f;
		cy = 3.5f;
		cz = 0.0f;

		ux = 0.0f;
		uy = 2.5f;
		uz = 0.0f;

		zoom = 90.0f;
	}





}



// Adjusts the viewport sie when the window size is changed and sets the projection.

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	WINDOW_HEIGHT = h;
	WINDOW_WIDTH = w;
}

void animate() {

	/* rotate fans with different speed and direction*/
	f1.rotateFan(-7.0f);
	f2.rotateFan(-10.0f);
	f3.rotateFan(4.0f);
	f4.rotateFan(10.0f);
	/* refresh screen */
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Lewis Ross 1501830");


	initRendering();


	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(animate);
	glutKeyboardFunc(processNormalKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();


	return 1;
}
