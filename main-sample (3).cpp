// raycast sample code

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//added these
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

// light 1 position and color
Point light1;
float light1_color[3];  // diffuse


//
// return length of a vector
//
float vec_len(Vector u)
{

}

//
// return dot product of two vectors
//
float vec_dot(Vector p, Vector q)
{

}

//
// return sum of two vectors
//
Vector vec_plus(Vector p, Vector q)
{
 
}

//
// return difference between two vectors
//
Vector vec_minus(Vector p, Vector q)
{

}

//
// return vector from point point to another
//
Vector get_vec(Point q, Point p)
{

}



//
// normalize a vector
//
void normalize(Vector* u)
{
 
}

//
// return a scaled vector
//
Vector vec_scale(Vector p, float scale)
{

}



//
// sets up scene
//
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
    
    // sphere 2 - red
    
    // sphere 3 - yellow
    
}



//
// get color at intersection point
//
RGB_float get_color(Vector ray, Point p)
{

}


//
// ray casting algorithm
//
void ray_casting() {

    double x_grid_size = image_width / double(win_width);
    double y_grid_size = image_height / double (win_height);
    
    Vector ray;
    
    
    for (int i = 0; i < win_height; i++) {
        for (int j = 0; j < win_width; j++) {
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

