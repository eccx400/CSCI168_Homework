//  main.cpp
//  assignment2

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <cmath>

using namespace std;

// the struct to store x, y, z coordinates of a vertex
struct vertex {
    float x, y, z;
};

struct v {
    float x,y,z;
};

enum {POINTS=1, WIRE, FILL};

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
        ZoomVal-= .05;
    }
    // 45 represents the "-" (minus) button
    else if (key == 45) {
        ZoomVal+= .05;
    }
    glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
    //if the left button is clicked the coordinates are saved for use in rotation
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
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
    GLfloat light_pos[] = {0.5f, 0.5f, -5.3f, 1.f};//{ 2, 1, 0, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    static GLfloat face_mat[] = {1.0f, 0.5f, 0.0f, 1.0f};
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


void drawTriangle(int first, int second, int third) {
    
    glBegin(GL_TRIANGLES);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    
};

FILE *fp;
void initObject()
{
    // read/store the object data file 
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
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //Set camera
    glLoadIdentity();
    gluLookAt(ZoomVal, ZoomVal, ZoomVal, 0, 0, 0, 0, 1, 0);
    glRotated(AngleX, 0, 1, 0);
    glRotated(AngleY, 1, 0, 0);
    
    
    // set color
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    switch(rendermode) {
        case FILL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glShadeModel(GL_FLAT);
            break;
            
        case POINTS:
            glPointSize(3);
        case WIRE:
            glColor4f(1.0f, 0.5f, 0.0f,1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            break;
        
    }

    //Display face - add your code here





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
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
    
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

