//
//  main.cpp
//  opengl_chaikin_curve
//
//  Created by Merci on 1/27/18.
//  Copyright © 2018 Merci. All rights reserved.
//

// #include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <iostream>
using namespace std;

class Vertex {
  GLfloat x, y;
  public:
  Vertex(GLfloat, GLfloat);
  GLfloat get_y() { return y; };
  GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    
    vector<Vertex> points;
    // Iterate through out initial control points by pairs
    for (int p = 0; p < control_points.size()-1; p++) {
        Vertex v_0 = control_points[p];
        Vertex v_1 = control_points[p+1];
        // First point at the 1/4 position along the line from v_0 and v_1
        GLfloat y1 = (3.0/4.0)*v_0.get_y()+(1.0/4.0)*v_1.get_y();
        GLfloat x1 = (3.0/4.0)*v_0.get_x()+(1.0/4.0)*v_1.get_x();
        // Second point at the 3/4 position along the line from v_0 and v_1
        GLfloat y2 = (1.0/4.0)*v_0.get_y()+(3.0/4.0)*v_1.get_y();
        GLfloat x2 = (1.0/4.0)*v_0.get_x()+(3.0/4.0)*v_1.get_x();
        // push_back simply adds a new element to the back of your vector (list)
        points.push_back(Vertex(x1, y1));
        points.push_back(Vertex(x2, y2));
    }
    
    return points;
}

