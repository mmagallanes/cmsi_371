/***
 Assignment-3: Shading via Lighting and Colors
 
 Name: Magallanes, Merci
 
 Collaborators: 371 Spring 2018 Class
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: In the resubmission of this assignment, I attempted to recreate the bedroom from the film, 2001: A Space Odyssey. I created the bed, the bench at the foot of the bed, a side table, and the monolith from the movie. In the scene, the lighting comes from the floor, but in this recreation, the lighting comes from the side, leaving half of the room in shadow, including the monolith, which is all black.
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
using namespace std;

/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
class ObjectModel {
  vector<GLfloat> _points;
  vector<GLfloat> _normals;
  vector<GLfloat> _base_colors;
  vector<GLfloat> _colors;
public:
  ObjectModel() { };
  vector<GLfloat> get_points() { return _points; };
  vector<GLfloat> get_normals() { return _normals; };
  vector<GLfloat> get_base_colors() { return _base_colors; };
  vector<GLfloat> get_colors() { return _colors; };
  void set_points(vector<GLfloat> points) { _points = points; };
  void set_normals(vector<GLfloat> normals) { _normals = normals; };
  void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
  void set_colors(vector<GLfloat> colors) { _colors = colors; };
};

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  Using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene.*
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

// Simplification of vectors
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
  to_cartesian_coord(mat_mult(translation_matrix(0.0, 0.0, 0.5),
                              to_homogenous_coord(init_plane())));
  
  vector<GLfloat> rightPanel =
  to_cartesian_coord(mat_mult(translation_matrix(0.5, 0.0, 0.0),
                              mat_mult(rotation_matrix_y(M_PI/2),
                                       to_homogenous_coord(init_plane()))));
  
  vector<GLfloat> leftPanel =
  to_cartesian_coord(mat_mult(translation_matrix(-0.5, 0.0, 0.0),
                              mat_mult(rotation_matrix_y(M_PI/2),
                                       to_homogenous_coord(init_plane()))));
  
  vector<GLfloat> backPanel =
  to_cartesian_coord(mat_mult(translation_matrix(0.0, 0.0, -0.5),
                              to_homogenous_coord(init_plane())));
  
  vector<GLfloat> bottomPanel =
  to_cartesian_coord(mat_mult(translation_matrix(0.0, -0.5, 0.0),
                              mat_mult(rotation_matrix_x(M_PI/2),
                                       to_homogenous_coord(init_plane()))));
  
  vector<GLfloat> topPanel =
  to_cartesian_coord(mat_mult(translation_matrix(0.0, 0.5, 0.0),
                              mat_mult(rotation_matrix_x(M_PI/2),
                                       to_homogenous_coord(init_plane()))));
  
  result = combine(bottomPanel, combine(topPanel, combine(leftPanel, combine(rightPanel, combine(backPanel, combine(frontPanel, result))))));
  return result;
}

/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/

// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
  vector<GLfloat> C;
  
  C = { (A[1]*B[2]) - (A[2]*B[1]),
    (A[0]*B[2]) - (A[2]*B[0]),
    (A[0]*B[1]) - (A[1]*B[0])};
  
  return C;
}

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points) {
  // Note: each plane (quad) contains 4 points, choose the points
  // to generate your vectors such that the normals (given by the
  // cross product, point to the correct direction
  
  vector<GLfloat> normals;
  vector<GLfloat> vector_A;
  vector<GLfloat> vector_B;
  
  vector<GLfloat> q0 = {points[0], points[1], points[2]};
  vector<GLfloat> q1 = {points[3], points[4], points[5]};
  vector<GLfloat> q2 = {points[6], points[7], points[8]};
  vector<GLfloat> q3 = {points[9], points[10], points[11]};
  
  GLfloat xA = q1[0] - q0[0];
  GLfloat yA = q1[1] - q0[1];
  GLfloat zA = q1[2] - q0[2];
  
  vector_A.push_back(xA);
  vector_A.push_back(yA);
  vector_A.push_back(zA);
  
  
  //    return slideLeft;
  //    return slideDown;
  //
  //    normals = combine(cross_product(slideDown, slideLeft), normals);
  
  return normals;
}


/**************************************************
 *         Shading via Lighting and Color         *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/

// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
  vector<GLfloat> base_color = {
    r,   g,   b,
    r,   g,   b,
    r,   g,   b,
    r,   g,   b
  };
  return base_color;
}

// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
  // This enables OpenGL to use interpolation for (Gouraud) shading the plane
  vector<GLfloat> base_color = {
    r0,   g0,   b0,
    r1,   g1,   b1,
    r2,   g2,   b2,
    r3,   g3,   b3
  };
  return base_color;
}

// Generates the colors of a set of surfaces based on the light source, surface normals, and base color of the surface
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera) {
  vector<GLfloat> colors;
  
  object_model.set_colors(colors);
  return object_model;
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 x n values) and specular (vector of 3 x n values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec) {
  vector<GLfloat> colors;
  
  
  object_model.set_colors(colors);
  return object_model;
}

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {
  GLfloat sum = 0.0;
  for (int i = 0; i < A.size(); i++) {
    for(int j = 0; j < B.size(); j++) {
      sum += A[i]*B[i];
    }
  }
  return sum;
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  Create a scene by applying transformations to *
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
  
  // Enable colors
  glEnable(GL_COLOR_MATERIAL);
  
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  
  // Set up some default base color
  glColor3f(0.5, 0.5, 0.5);
  
  // Set up white background
  glClearColor(0.941, 1.000, 0.941, 0.0);
}

void init_camera() {
  // Camera parameters
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(50.0, 1.0, 2.0, 50.0);
  
  gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // original
  
  // Lighting
  GLfloat position[] = {0.0, 0.0, 0.0};
  GLfloat ambience[] = {0.35, 0.35, 0.35, 1.0};
  GLfloat diffusion[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat specular[] = {1.0, 1.0, 1.0, 2.0};
  
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambience);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusion);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

vector<GLfloat> build_bed() {
  vector<GLfloat> result;
  
  vector<GLfloat> mattress =
  to_cartesian_coord(mat_mult(translation_matrix(-0.625, 0.0, 0.5),
                              mat_mult(scaling_matrix(0.25, 0.5, 1.5),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> duvet =
  to_cartesian_coord(mat_mult(translation_matrix(0.0, 0.0, 0.5),
                              mat_mult(scaling_matrix(1.0, 0.5, 1.5),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> headboard =
  to_cartesian_coord(mat_mult(translation_matrix(-0.78, 0.35, 0.5),
                              mat_mult(scaling_matrix(0.05, 1.2, 1.5),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> pillow =
  to_cartesian_coord(mat_mult(translation_matrix(-0.6, 0.3, 0.5),
                              mat_mult(scaling_matrix(0.2, 0.07, 0.5),
                                       to_homogenous_coord(build_cube()))));
  
  result = combine(duvet, combine(pillow, combine(headboard, combine(mattress, result))));
  return result;
}

vector<GLfloat> build_bedroom_bench() {
  vector<GLfloat> result;
  
  vector<GLfloat> table =
  to_cartesian_coord(mat_mult(translation_matrix(0.68, 0.0, 0.5),
                              mat_mult(scaling_matrix(0.3, 0.08, 1.0),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> back_right_leg =
  to_cartesian_coord(mat_mult(translation_matrix(0.548, -0.17, 0.016),
                              mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> back_left_leg =
  to_cartesian_coord(mat_mult(translation_matrix(0.548, -0.17, 0.984),
                              mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> front_left_leg =
  to_cartesian_coord(mat_mult(translation_matrix(0.814, -0.17, 0.984),
                              mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                       to_homogenous_coord(build_cube()))));
  
  vector<GLfloat> front_right_leg =
  to_cartesian_coord(mat_mult(translation_matrix(0.814, -0.17, 0.016),
                              mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                       to_homogenous_coord(build_cube()))));
  
  result = combine(front_right_leg, combine(front_left_leg, combine(back_left_leg, combine(back_right_leg, combine(table, result)))));
  return result;
}

vector<GLfloat> build_table() {
  vector<GLfloat> result;
  
  vector<GLfloat> table =
  to_cartesian_coord(mat_mult(rotation_matrix_y(M_PI-(M_PI/4)),
                              (mat_mult(translation_matrix(-1.7, 0.0, -1.5),
                                        mat_mult(scaling_matrix(0.3, 0.08, 1.0),
                                                 to_homogenous_coord(build_cube()))))));
  
  vector<GLfloat> front_left_leg =
  to_cartesian_coord(mat_mult(rotation_matrix_y(M_PI-(M_PI/4)),
                              (mat_mult(translation_matrix(-1.835, -0.17, -1.987),
                                        mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                                 to_homogenous_coord(build_cube()))))));
  
  vector<GLfloat> front_right_leg =
  to_cartesian_coord(mat_mult(rotation_matrix_y(M_PI-(M_PI/4)),
                              (mat_mult(translation_matrix(-1.83, -0.17, -1.016),
                                        mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                                 to_homogenous_coord(build_cube()))))));
  
  vector<GLfloat> back_right_leg =
  to_cartesian_coord(mat_mult(rotation_matrix_y(M_PI-(M_PI/4)),
                              (mat_mult(translation_matrix(-1.56, -0.17, -1.016),
                                        mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                                 to_homogenous_coord(build_cube()))))));
  
  vector<GLfloat> back_left_leg =
  to_cartesian_coord(mat_mult(rotation_matrix_y(M_PI-(M_PI/4)),
                              (mat_mult(translation_matrix(-1.559, -0.17, -1.984),
                                        mat_mult(scaling_matrix(0.03, 0.25, 0.03),
                                                 to_homogenous_coord(build_cube()))))));
  
  result = combine(front_right_leg, combine(front_left_leg, combine(back_left_leg, combine(back_right_leg, combine(table, result)))));
  return result;
}

vector<GLfloat> monolith() {
  vector<GLfloat> monolith;
  
  vector<GLfloat> space_oddity =
  to_cartesian_coord(mat_mult(translation_matrix(3.0, 0.9, 0.5),
                              mat_mult(scaling_matrix(0.07, 2.5, 0.8),
                                       to_homogenous_coord(build_cube()))));
  
  monolith = combine(space_oddity, monolith);
  return monolith;
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
  vector<GLfloat> result;
  result = combine(monolith(), combine(build_table(), combine(build_bedroom_bench(), combine(build_bed(), result))));
  GLfloat* arr = new GLfloat[result.size()];
  arr = vector2array(result);
  return arr;
}

// Construct the color mapping of the scene
GLfloat* init_color() {
  vector<GLfloat> coloring;
  // mattress color
  for (int i = 0; i < 24.0; i++) {
    coloring.push_back(0.686);
    coloring.push_back(0.933);
    coloring.push_back(0.933);
  }
  
  // headboard color
  for (int i = 25; i < 48.0; i++) {
    coloring.push_back(0.00);
    coloring.push_back(0.192);
    coloring.push_back(0.00);
  }
  
  // pillow color
  for (int i = 49; i < 70.0; i++) {
    coloring.push_back(0.482);
    coloring.push_back(0.820);
    coloring.push_back(0.800);
  }
  
  // duvet color
  for (int i = 71.0; i < 96.0; i++) {
    coloring.push_back(0.961);
    coloring.push_back(0.871);
    coloring.push_back(0.702);
  }
  
  // bed bench
  for (int i = 97.0; i < 124.0; i++) {
    coloring.push_back(0.741);
    coloring.push_back(0.718);
    coloring.push_back(0.420);
  }
  
  // bed bench legs
  for (int i = 125.0; i < 220.0; i++) {
    coloring.push_back(1.0);
    coloring.push_back(1.0);
    coloring.push_back(1.0);
  }
  
  // side table
  for (int i = 221.0; i < 244.0; i++) {
    coloring.push_back(1.00);
    coloring.push_back(0.855);
    coloring.push_back(0.725);
  }
  // side table legs
  for (int i = 245.0; i < 342.0; i++) {
    coloring.push_back(1.0);
    coloring.push_back(1.0);
    coloring.push_back(1.0);
  }
  
  // monolith
  for (int i = 343.0; i < 824.0; i++) {
    coloring.push_back(0.0);
  }
  return vector2array(coloring);
}

void display_func() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glRotatef(theta, 0.0, 1.0, 0.0);
  //glRotatef(theta, 1.0, 0.0, 0.0);
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
  glDrawArrays(GL_QUADS, 0, (4*6)*15);
  
  glFlush();            //Finish rendering
  glutSwapBuffers();
}

void idle_func() {
  theta = theta+0.5;
  display_func();
}

int main (int argc, char **argv) {
  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  
  // Create a window with rendering context and everything else we need
  glutCreateWindow("Assignment 3");
  
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
