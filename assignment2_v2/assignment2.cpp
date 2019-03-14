/***
 Assignment-2: Geometric Modeling of a Scene
 
 Name: Magallanes, Merci
 
 Collaborators: Chu, Theodore; Alvarez, Marissa; Savini, Sam; Scott, John
 
 
 Project Summary: For this project, I attempted to initially form a cube using 6 planes, by rotating and/or translating from a base plane. In the resubmission, I figured out that my matrix multiplication function was incorrect, therefore affecting my build_cube() function. In this version of assignment 2, the cube is now correct. 
 ***/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i+=3) {
        result.push_back(cartesian_coords[i]);
        result.push_back(cartesian_coords[i+1]);
        result.push_back(cartesian_coords[i+2]);
        result.push_back(1.0);
    }
    
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogenous_coords.size(); i+=4) {
        result.push_back(homogenous_coords[i]);
        result.push_back(homogenous_coords[i+1]);
        result.push_back(homogenous_coords[i+2]);
    }
    
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat;
    
    translate_mat = {
        1.0, 0.0, 0.0, dx,
        0.0, 1.0, 0.0, dy,
        0.0, 0.0, 1.0, dz,
        0.0, 0.0, 0.0, 1.0
    };
    
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat;
    
    scale_mat = {
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return scale_mat;
}

// Definition of a rotation matrix along the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x;
    
    rotate_mat_x = {
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(theta), -sin(theta), 0.0,
        0.0, sin(theta), cos(theta), 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_x;
}

// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    
    rotate_mat_y = {
        cos(theta), 0.0, sin(theta), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(theta), 0.0, cos(theta), 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_y;
}

// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    
    rotate_mat_z = {
        cos(theta), -sin(theta), 0.0, 0.0,
        sin(theta), cos(theta), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_z;
}

// Perform matrix multiplication for A B

vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    
    for (int j = 0; j < B.size(); j += 4) {
        for (int i = 0; i < 16; i += 4) {
            result.push_back(A[i]*B[j]+A[i+1]*B[j+1]+A[i+2]*B[j+2]+A[i+3]*B[j+3]);
        }
    }
    
    return result;
}

vector<GLfloat> combine(vector<GLfloat> part, vector<GLfloat> combo) {
    for (int i = 0; i < part.size(); i++){
        combo.push_back(part[i]);
    }

    return combo;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    
    vector<GLfloat> frontPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(0.0, 0.0, 0.5),
              to_homogenous_coord(init_plane())));
    
    vector<GLfloat> rightPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(0.5, 0.0, 0.0),
              mat_mult(rotation_matrix_y(M_PI/2),
                       to_homogenous_coord(init_plane()))));

    vector<GLfloat> leftPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(-0.5, 0.0, 0.0),
              mat_mult(rotation_matrix_y(M_PI/2),
                       to_homogenous_coord(init_plane()))));

    vector<GLfloat> backPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(0.0, 0.0, -0.5),
                       to_homogenous_coord(init_plane())));

    vector<GLfloat> bottomPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(0.0, -0.5, 0.0),
              mat_mult(rotation_matrix_x(M_PI/2),
                       to_homogenous_coord(init_plane()))));

    vector<GLfloat> topPanel =
    to_cartesian_coord
    (mat_mult(translation_matrix(0.0, 0.5, 0.0),
              mat_mult(rotation_matrix_x(M_PI/2),
                       to_homogenous_coord(init_plane()))));
    
    result = combine(bottomPanel, combine(topPanel, combine(leftPanel, combine(rightPanel, combine(backPanel, combine(frontPanel, result))))));
    return result;

}

//

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by using setting *
 *  the projection viewing matrices               *
 *                                                *
 *************************************************/

float theta = 0.0;

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    vector<GLfloat> result;
    result = combine(build_cube(), result);
    GLfloat* arr = new GLfloat[result.size()];
    arr = vector2array(result);
    return arr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    vector<GLfloat> coloring;
    for (int i = 0; i < 72.0; i++) {
        coloring.push_back(0.5);
    }
    return vector2array(coloring);
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glRotatef(theta, 0.0, 1.0, 0.0);
    glRotatef(theta, 1.0, 0.0, 0.0);
    // Perform display functions
    
    GLfloat* vertices = init_scene();
    GLfloat* colors = init_color();
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4*6);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
    
}

void idle_func() {
    theta = theta+0.3;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 2");
    
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);

    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    delete init_scene();
    delete init_color();
    
    return 0;
}



