// raycast sample code

/**
 * @author Eric Cheng
 * @date 18 November, 2018
 *
 * This assignment will implement the concept of ray casting by
 * using the definitions and terminologies from class. The program
 * requires the definition of light intensity, light location, and
 * eye location coordinates. Light intensity is given as RGB, while 
 * light and eye location are given by x, y, z coordinates. The
 * program also has to utilize the structs to draw out the vector
 * and spheres, and also find the vector functions using mathematical
 * equations.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//added these
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

// this size is the real image size, the bigger the image size
// the smaller the scene
// set this to be an appropriate size, then the grid size is determined as:
// x_grid_size = image_width / SCREEN_WIDTH;
#define IMAGE_WIDTH 10.0

using namespace std;

// window size
int win_width = SCREEN_WIDTH;
int win_height = SCREEN_HEIGHT;

// image size
float image_width = IMAGE_WIDTH;
float image_height = (float(SCREEN_HEIGHT) / float(SCREEN_WIDTH)) * IMAGE_WIDTH;

float vertices[SCREEN_WIDTH*SCREEN_HEIGHT*2];
float colors[SCREEN_WIDTH*SCREEN_HEIGHT*3];

typedef struct {
    float x;
    float y;
    float z;
} Point;  // 3D point

typedef struct {
    float x;
    float y;
    float z;
} Vector;  // 3D vector

typedef struct {
    float r;
    float g;
    float b;
} RGB_float;  // Color

typedef struct sphere {
    int index; // identifies a sphere; must be greater than 0
	int id;
    
    Point center;
    float radius;
    
    float sphere_ambient[3];  // ambient light of sphere
    float color[3];  // diffuse light of sphere
    
} Spheres; // a list of spheres

Point eye_pos = {0.0, 0.0, 1.1 }; // eye position
float image_plane = 0;//-1.5; // image plane position

RGB_float background_clr; // background color
RGB_float null_clr = {0.0, 0.0, 0.0}; // NULL color

// list of spheres in the scene
Spheres *scene = NULL;

Spheres gold;
Spheres red;
Spheres yellow;

Spheres arr[3] = { gold, red, yellow };

// light 1 position and color
Point light1;
float light1_color[3];  // diffuse


/**
 * This function returns the length of a given vector
 * by finding the square root of the sum of the verticies
 * squared. The equation is sqrt(x^2 + y^2 + z^2).
 *
 * @param u = vector which the length is to be found
 */
float vec_len(Vector u)
{
	return sqrt((u.x * u.x) + (u.y * u.y) + (u.z * u.z));
}

/**
 * This function returns the dot product of two given vectors
 * by using the equation dot_product(x,y) = (x1 * y1) + (x2 * y2)
 *
 * @param p = the first vector
 * @param q = the second vector
 */
float vec_dot(Vector p, Vector q)
{
	return (p.x * q.x) + (p.y * q.y);
}

/**
 * This function returns the sum of two vectors which can e found 
 * by adding the x, y, and z values of the two vectors
 *
 * @param p = the first vector
 * @param q = the second vector
 */
Vector vec_plus(Vector p, Vector q)
{
	Vector a;
	a.x = p.x + q.x;
	a.y = p.y + q.y;
	a.z = p.z + q.z;
	return a;
}

/**
 * This function returns the sum of two vectors which can e found
 * by subtracting the x, y, and z values of the two vectors
 *
 * @param p = the first vector
 * @param q = the second vector
 */
Vector vec_minus(Vector p, Vector q)
{
	Vector a;
	a.x = p.x - q.x;
	a.y = p.y - q.y;
	a.z = p.z - q.z;
	return a;
}

/**
 * The get_vec() function takes in two points as
 * inputs, each with its x, y, and z values, and 
 * 
 *
 * @param q = the first point to get the vector
 * @param p = the second point to get the vector
 */
Vector get_vec(Point q, Point p)
{
	Vector a;
	a.x = p.x - q.x;
	a.y = p.y - q.y;
	a.z = p.z - q.z;
	return a;
}

/**
 * This function returns the sum of two vectors which can e found
 * by adding the x, y, and z values of the two vectors
 *
 * @param u = the vector to be normalized
 */
Vector normalize(Vector u)
{
	Vector a;
	float p = vec_len(u);
	a.x = u.x / p;
	a.y = u.y / p;
	a.z = u.z / p;
	return a;
}

/**
 * This function returns the vector values scaled by 
 * a float value. 
 *
 * @param p = the first vector
 * @param scale = The multitude that the vector is scaled
 */
Vector vec_scale(Vector p, float scale)
{
	Vector a;
	a.x = p.x * scale;
	a.y = p.y * scale;
	a.z = p.z * scale;
	return a;
}

/**
 * The newVect function initializes a new Vector
 * with given float values for its x, y, and z 
 * values.
 *
 * @param i = the x value of the new Vector
 * @param j = the y value of the new Vector
 * @param k = the z value of the new Vector
 */
Vector newVect(float i, float j, float k)
{
	Vector a;
	a.x = i;
	a.y = j;
	a.z = k;
	return a;
}

/**
 * This function sets up the scene for the program to run on.
 * It sets the background color based on RGB additive color,
 * and the light that we use to pinpoint its source. We also have
 * to create the 3 spheres which are of the color gold, red, and
 * yellow.
 */
void set_up_scene()
{
	// set background color
	background_clr.r = 0.1;
	background_clr.g = 0.1;
	background_clr.b = 0.1;

	// setup light
	light1.x = 1.0;
	light1.y = 2.0;
	light1.z = 3.0;

	light1_color[0] = light1_color[1] = light1_color[2] = 1.0;

	// sphere 1 - gold
	gold.index = 1; // identifies a sphere; must be greater than 0
	gold.id = 1;

	gold.radius = float(1.0f);
	gold.center.x = 0;
	gold.center.y = 1;
	gold.center.z = 1;

	// diffuse light of sphere
	gold.color[0] = 255;
	gold.color[1] = 0;
	gold.color[2] = 0;

	gold.sphere_ambient[3] = {}; // ambient light of sphere

	// sphere 2 - red
	red.index = 2; // identifies a sphere; must be greater than 0
	red.id = 2;

	red.radius = float(1.0f);
	red.center.x = 0;
	red.center.y = 1;
	red.center.z = -1;

	// diffuse light of sphere
	red.color[0] = 0;
	red.color[1] = 0;
	red.color[2] = 255;

	red.sphere_ambient[3] = {}; // ambient light of sphere

    // sphere 3 - yellow
	yellow.index = 3; // identifies a sphere; must be greater than 0
	yellow.id = 3;

	yellow.radius = float(1.0f);
	yellow.center.x = 0;
	yellow.center.y = -0.75;
	yellow.center.z = 0;

	// diffuse light of sphere
	yellow.color[0] = 0;
	yellow.color[1] = 255;
	yellow.color[2] = 0;

	yellow.sphere_ambient[3] = {}; // ambient light of sphere
}

/**
 * The get_color() function is a conditional function because it is only 
 * called upon when there is an intersection between the light vector and
 * eye vector when it is touching the spheres. What it does is that it
 * multiplies the sphere colors with the lights and the cosine value so that
 * it can calculate the color at intersection point (x, y, z). The cosine
 * value can be found by finding the dot product of the normal vector and
 * light vector. 
 *
 * @param ray = Is the vector ray that we take in 
 * @param p = Is the Point object that we take in
 * @param arr[] = Is the array that stores all of the spheres that we declared
 * and initialized above; each of them will be used in get_color if there is 
 * an intersection.
 */
RGB_float get_color(Vector ray, Point p, Spheres arr[])
{
	RGB_float colors;
	for (int i = 1; i < 4; i++)
	{
		if ((eye_pos.x == light1.x) && (ray.y == light1.y) && (ray.z == light1.z))
		{
			p.x = arr[i].color[1] * light1.x;
			p.y = arr[i].color[2] * light1.y;
			p.z = arr[i].color[3] * light1.y;

			colors.r = p.x;
			colors.g = p.y;
			colors.b = p.z;
		}
	}
	return colors;
}

/**
 * The ray casting function utilizes the diagram given for 
 * the setup of the ray casting algorithm to model its solution.
 * Given the size of the screen, we loop through each pixel to
 * check whether or not there is an intersection. If there is
 * an intersection, get the color value and store it
 * in the pixel.
 */
void ray_casting() {

    double x_grid_size = image_width / double(win_width);
    double y_grid_size = image_height / double (win_height);
    
    Vector ray;
    
    /**
	 * Check intersection of ray from eye [i,j] and get color
	 * value.
	 */
    for (int i = 0; i < win_height; i++) 
	{
        for (int j = 0; j < win_width; j++) 
		{
			float determinant = 0;

			Point temp = { i, j, 0 };
			ray = get_vec(temp, eye_pos);

			// use eyepos to find eye position

			//sphere equation
			//(x - center.x) ^ 2 + (y - center.y) ^ 2 + (z - center.z) ^ 2 - r ^ 2 = 0;
			for (int m = 1; m < 4 ; m++)
			{
				((arr[m].center.x) * (arr[m].center.x)) - ((arr[m].center.y) * (arr[m].center.y))
				- ((arr[m].center.z) * (arr[m].center.z)) - ((arr[m].radius) * (arr[m].radius));

				float B = 2 * (eye_pos.x - arr[m].center.x) * ray.x + (eye_pos.y - arr[m].center.y)
					* ray.y + (eye_pos.z - arr[m].center.z) * ray.z;

				float C = (eye_pos.x - arr[m].center.x) * (eye_pos.x - arr[m].center.x) + (eye_pos.y - arr[m].center.y)
					* (eye_pos.y - arr[m].center.y) + (eye_pos.z - arr[m].center.z) * (eye_pos.z - arr[m].center.z) -
					((arr[m].radius) * (arr[m].radius));

				if ((B * B) - (4 * C))
				{
					float t = (-1 * B - sqrt(B * B - 4 * C)) / 2;

					//line equation
					float x = eye_pos.x + ray.x * t;
					float y = eye_pos.y + ray.y * t;
					float z = eye_pos.z + ray.z * t;

					Point p;
					p.x = x;
					p.y = y;
					p.z = z;

					get_color(ray, p, arr);
				}
			}
        }
    }
}

int main( void )
{

    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); // essentially set coordinate system
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        // set the clear color to be the same as background color
        glClearColor(0.1,0.1,0.8,1);
        glClear( GL_COLOR_BUFFER_BIT );
        
        set_up_scene();
        ray_casting();
        
        // tell OpenGL that you're using a vertex array for fixed-function attribute
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY );
        
        // specify the vertices position and color to be used by DrawArrays below
        glVertexPointer( 2, GL_FLOAT, 0, vertices ); // point to the vertices to be used
        glColorPointer( 3, GL_FLOAT, 0, colors ); // point to the vertices to be used
        
        // Point size
        glPointSize( 10 ); // must be added before glDrawArrays is called
        
        // draw the vertixes
        glDrawArrays( GL_POINTS, 0, win_height*win_width );
        
        // tell OpenGL that you're finished using the vertex and colorarrayattribute
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_COLOR_ARRAY );
        
        // Swap front and back buffers
        glfwSwapBuffers( window );
        
        // Poll for and process events
        glfwPollEvents( );
    }
    
    glfwTerminate( );

    return 0;
}

