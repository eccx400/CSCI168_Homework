//  main.cpp
//  assignment2

/**
 * @author Eric Cheng
 * @date 7 November, 2018
 * @class CSCI 168 Assignment 2
 *
 * This assignment requires the use of triangles to model a human face.
 * The files provided are face-verticies.txt and face-index.txt, and
 * they are used to store the positional coordinates to model the 
 * triangles that are used to construct the human face. OpenGL will be
 * required to model the program, especially with the use of the glut and
 * glfw3 header files. The face-verticies.txt and face-index.txt files
 * will be used to pinpoint the verticies coordinates and the triangles
 * themselves that will be used to make up the face. File IO will be used
 * to extract data from the two files, and they will be drawn with the
 * drawTriangle() function with help of OpenGL.
 */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// the struct to store x, y, z coordinates of a vertex
struct vertex {
	float x, y, z;
};

struct v {
	float x, y, z;
};

enum { POINTS = 1, WIRE, FILL };

int rendermode = FILL;

// the array to store the vertices from face-vertices.txt
vertex vertices[7061];

//used to store values of mouse coordinates on click
int OldX, OldY;
//stores arbitrary values for the rotation
double AngleX, AngleY;
// zoom value defines the xyz coordinate of the camera to zoom
float ZoomVal = 2;

void MenuValue(int option) {
	rendermode = option;
	glutPostRedisplay();
}

void Zoom(unsigned char key, int x, int y) {
	// 61 represents the "=" or "+" key
	if (key == 61) {
		ZoomVal -= .05;
	}
	// 45 represents the "-" (minus) button
	else if (key == 45) {
		ZoomVal += .05;
	}
	glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
	//if the left button is clicked the coordinates are saved for use in rotation
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		OldX = x;
		OldY = y;
	}
}

void init(void) {
	// Sets background color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//Initialize lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Set light position
	GLfloat light_pos[] = { 0.5f, 0.5f, -5.3f, 1.f };//{ 2, 1, 0, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	static GLfloat face_mat[] = { 1.0f, 0.5f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, face_mat);

	//Initialize camera
	glMatrixMode(GL_PROJECTION);
	gluPerspective(50, 1, 0.1, 10);
	glMatrixMode(GL_MODELVIEW);

	//Initialize Menu and options
	glutCreateMenu(MenuValue);
	glutAddMenuEntry("Points", POINTS);
	glutAddMenuEntry("Wireframe", WIRE);
	glutAddMenuEntry("Solid", FILL);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Zooming in and out
	glutKeyboardFunc(Zoom);
}


/**
 * The drawTriangle function helps to draw the triangle with the given
 * verticies coordinates from the face-vertices.txt file that we took
 * from the initObject function. It has three parameters: first, second,
 * and third that form the verticies of the triangle from the given 
 * coordinate positions.
 *
 * @param first = The coordinates of the first vertex of the triangle
 * @param second = The coordinates of the second vertex of the triangle
 * @param third = The coordinates of the third vertex of the triangle
 */
void drawTriangle(int first, int second, int third)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(vertices[first].x, vertices[first].y, vertices[first].z);
	glVertex3f(vertices[second].x, vertices[second].y, vertices[second].z);
	glVertex3f(vertices[third].x, vertices[third].y, vertices[third].z);
	glEnd();
};

/**
 * Read/store the object data file
 *
 * The initObject() function takes in the vertex coordinates from
 * the face-verticies.txt file using File IO and stores them into 
 * the verticies struct listed above. The file is sorted by x, y, z
 * coordinates, and they must be read with delimiter ',' so that 
 * the x, y, z coordinates may be stored in their specified variables.
 * Since getline takes the input as a string, we use the stof() function
 * to store the numbers as a float. The counter helps to store the
 * coordinates in different indexes in the vertices structure.
 */
void initObject()
{
	ifstream instream;
	instream.open("C:\\Users\\ericc\\OneDrive\\Documents\\CS\\Graphics Homework\\CSCI168_HW2\\face-vertices.txt");

	string x, y, z;
	int count = 0;
	while (!instream.eof())
	{
		getline(instream, x, ',');
		//cout << x << endl; //Output Testing

		getline(instream, y, ',');
		//cout << y << endl; //Output Testing

		getline(instream, z, ',');
		//cout << z << endl; //Output Testing

		vertices[count].x = stof(x);
		vertices[count].y = stof(y);
		vertices[count].z = stof(z);
		count++;
	}
}

void redraw(void) {
	//Print OpenGL errors
	GLenum err_code;
	do {
		err_code = glGetError();
		if (err_code != GL_NO_ERROR)
			printf("Error: %s\n", gluErrorString(err_code));
	} while (err_code != GL_NO_ERROR);

	//Clear buffer data
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set camera
	glLoadIdentity();
	gluLookAt(ZoomVal, ZoomVal, ZoomVal, 0, 0, 0, 0, 1, 0);
	glRotated(AngleX, 0, 1, 0);
	glRotated(AngleY, 1, 0, 0);


	// set color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (rendermode) {
	case FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		break;

	case POINTS:
		glPointSize(3);
	case WIRE:
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	default:
		break;

	}

	/**
	 * This part of the redraw function draws the face itself from the 
	 * positions specified in the face-index.txt. The face-index.txt file
	 * shows the positions of the triangles that should be drawn and which
	 * ones are adjacent to each other. After reading in the indexes from
	 * the file, we then convert the strings into integers using stoi, and
	 * then store them into specific variables. These variables are the 
	 * parameters for the drawTriangle() function we wrote earlier, and
	 * will be used to draw the triangles that make up the face model.
	 */
	ifstream rstream;
	rstream.open("C:\\Users\\ericc\\OneDrive\\Documents\\CS\\Graphics Homework\\CSCI168_HW2\\face-index.txt");
	while (!rstream.eof())
	{
		int i, j, k;
		string x;

		getline(rstream, x, ',');
		i = stoi(x);
		
		getline(rstream, x, ',');
		j = stoi(x);

		getline(rstream, x, ',');
		k = stoi(x);

		drawTriangle(i, j, k);
	}

	//Flush data
	glFlush();
}

void RotateObject(int x, int y) {
	double diffX, diffY;
	//calculate the difference of old x value to current
	diffX = x - OldX;
	diffY = y - OldY;
	//arbitrarily scales the difference
	diffX = diffX / 4;
	diffY = diffY / 4;
	//creates and alters the angle to rotate by
	AngleX += diffX;
	AngleY += diffY;
	//reassigns x and y value to be continuously used over and over
	OldX = x;
	OldY = y;
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	initObject();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(700, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("object representation");
	init();
	glutDisplayFunc(redraw);

	//Rotational
	glutMotionFunc(RotateObject);
	glutMouseFunc(MouseClick);

	glutMainLoop();
	return 0;
}

