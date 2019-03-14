/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Magallanes, Merci
 
 Collaborators: Alvarez, Marissa; Scott, John; Chu, Theodore
 
 Project Summary: Initially, I started out with plotting the points of the picture and having lines connect the points before I worked on the functions that would take the points and create a curve. To create the curve, I used the iterative method, which uses the "corner cutting algorithm". Using a midpoint_generator method, I generated two sets of midpoints; the first iteration called 'secondary_midpoints' and the second iteration called 'tertiary_midpoints'. I then placed the two sets of midpoints into a compilation vector called 'final_points'.
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

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector<Vertex> midpoint_generator(vector<Vertex> basic_points) {
    vector<Vertex> midpoints;
    
    for (int i = 0; i < basic_points.size()-1; i++) {
        Vertex p0 = basic_points[i];
        Vertex p1 = basic_points[i+1];
        
        GLfloat new_y = (p0.get_y()+p1.get_y())/2;
        GLfloat new_x = (p0.get_x()+p1.get_x())/2;
        
        midpoints.push_back(Vertex(new_x, new_y));
    }
    return midpoints;
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> secondary_midpoints;
    vector<Vertex> tertiary_midpoints;
    vector<Vertex> final_points;
    
    Vertex first_pt = control_points[0];
    Vertex last_pt = control_points[control_points.size()-1];
    
    secondary_midpoints = midpoint_generator(control_points);
    tertiary_midpoints = midpoint_generator(secondary_midpoints);
    
    final_points.push_back(first_pt);
    final_points.push_back(secondary_midpoints[0]);
    
    for (int i = 0; i < tertiary_midpoints.size(); i++) {
        final_points.push_back(tertiary_midpoints[i]);
        final_points.push_back(secondary_midpoints[i+1]);
    }
    
    final_points.push_back(last_pt);
    return final_points;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {
    for (int i = 0; i < n_iter; i++) {
        control_points = generate_points(control_points);
    }
    
    glBegin(GL_POINTS);
    for (int current_point = 0; current_point < control_points.size(); current_point++) {
        glVertex2f(control_points[current_point].get_x(), control_points[current_point].get_y());
    }
    glEnd();
    
    glBegin(GL_LINES);
    for (int current_point = 0; current_point < control_points.size()-2; current_point++) {
        glVertex2f(control_points[current_point ].get_x(), control_points[current_point ].get_y());
        glVertex2f(control_points[current_point +1].get_x(), control_points[current_point +1].get_y());
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.0f);
    
    // Head outline
    vector<Vertex> head_outline;
    head_outline.push_back(Vertex(0.585f, 0.206f));
    head_outline.push_back(Vertex(0.592f, 0.208f));
    head_outline.push_back(Vertex(0.6f, 0.325f));
    head_outline.push_back(Vertex(0.602f, 0.438f));
    head_outline.push_back(Vertex(0.604f, 0.56f));
    head_outline.push_back(Vertex(0.593f, 0.69f));
    head_outline.push_back(Vertex(0.516f, 0.783f));
    head_outline.push_back(Vertex(0.407f, 0.86f));
    head_outline.push_back(Vertex(0.278f, 0.94f));
    head_outline.push_back(Vertex(0.135f, 0.965f));
    head_outline.push_back(Vertex(0.0f, 0.973f));
    head_outline.push_back(Vertex(-0.137f, 0.938f));
    head_outline.push_back(Vertex(-0.23f, 0.861f));
    head_outline.push_back(Vertex(-0.329f, 0.762f));
    head_outline.push_back(Vertex(-0.379f, 0.63f));
    head_outline.push_back(Vertex(-0.401f, 0.512f));
    head_outline.push_back(Vertex(-0.42f, 0.389f));
    head_outline.push_back(Vertex(-0.412f, 0.251f));
    head_outline.push_back(Vertex(-0.41f, 0.167f));
    head_outline.push_back(Vertex(-0.389f, 0.093f));
    head_outline.push_back(Vertex(-0.377f, 0.01f));
    head_outline.push_back(Vertex(-0.356f, -0.077f));
    head_outline.push_back(Vertex(-0.344f, -0.181f));
    head_outline.push_back(Vertex(-0.313f, -0.277f));
    head_outline.push_back(Vertex(-0.241f, -0.368f));
    head_outline.push_back(Vertex(-0.145f, -0.44f));
    head_outline.push_back(Vertex(-0.063f, -0.49f));
    head_outline.push_back(Vertex(0.04f, -0.486f));
    head_outline.push_back(Vertex(0.15f, -0.45f));
    head_outline.push_back(Vertex(0.25f, -0.388f));
    head_outline.push_back(Vertex(0.338f, -0.31f));
    head_outline.push_back(Vertex(0.4f, -0.218f));
    head_outline.push_back(Vertex(0.436f, -0.132f));
    head_outline.push_back(Vertex(0.51f, -0.134f));
    head_outline.push_back(Vertex(0.568f, -0.077f));
    head_outline.push_back(Vertex(0.613f, 0.007f));
    head_outline.push_back(Vertex(0.645f, 0.087f));
    head_outline.push_back(Vertex(0.635f, 0.159f));
    head_outline.push_back(Vertex(0.629f, 0.176f));
    head_outline.push_back(Vertex(0.58f, 0.206f));
    head_outline.push_back(Vertex(0.553f, 0.183f));
    draw_curve(head_outline, 8);
    
    // Hairline
    vector<Vertex> hairline;
    hairline.push_back(Vertex(0.55f, 0.18f));
    hairline.push_back(Vertex(0.51f, 0.147f));
    hairline.push_back(Vertex(0.474f, 0.105f));
    hairline.push_back(Vertex(0.449f, 0.066f));
    hairline.push_back(Vertex(0.437f, 0.158f));
    hairline.push_back(Vertex(0.42f, 0.225f));
    hairline.push_back(Vertex(0.363f, 0.292f));
    hairline.push_back(Vertex(0.372f, 0.4f));
    hairline.push_back(Vertex(0.341f, 0.495f));
    hairline.push_back(Vertex(0.285f, 0.56f));
    hairline.push_back(Vertex(0.267f, 0.633f));
    hairline.push_back(Vertex(0.214f, 0.68f));
    hairline.push_back(Vertex(0.16f, 0.666f));
    hairline.push_back(Vertex(0.205f, 0.588f));
    hairline.push_back(Vertex(0.104f, 0.571f));
    hairline.push_back(Vertex(0.002f, 0.597f));
    hairline.push_back(Vertex(-0.098f, 0.618f));
    hairline.push_back(Vertex(-0.178f, 0.695f));
    hairline.push_back(Vertex(-0.252f, 0.593f));
    hairline.push_back(Vertex(-0.2f, 0.535f));
    hairline.push_back(Vertex(-0.277f, 0.495f));
    hairline.push_back(Vertex(-0.292f, 0.564f));
    hairline.push_back(Vertex(-0.334f, 0.5f));
    hairline.push_back(Vertex(-0.375f, 0.413f));
    hairline.push_back(Vertex(-0.374f, 0.317f));
    hairline.push_back(Vertex(-0.379f, 0.203f));
    hairline.push_back(Vertex(-0.38f, 0.14f));
    hairline.push_back(Vertex(-0.404f, 0.177f));
    draw_curve(hairline, 8);
    
    // Right eye
    vector<Vertex> right_eye;
    right_eye.push_back(Vertex(0.195f, 0.132f));
    right_eye.push_back(Vertex(0.215f, 0.121f));
    right_eye.push_back(Vertex(0.176f, 0.141f));
    right_eye.push_back(Vertex(0.155f, 0.149f));
    right_eye.push_back(Vertex(0.134f, 0.154f));
    right_eye.push_back(Vertex(0.113f, 0.153f));
    right_eye.push_back(Vertex(0.0947f, 0.146f));
    right_eye.push_back(Vertex(0.0754f, 0.1343f));
    right_eye.push_back(Vertex(0.062f, 0.1195f));
    right_eye.push_back(Vertex(0.049f, 0.1f));
    right_eye.push_back(Vertex(0.064f, 0.092f));
    right_eye.push_back(Vertex(0.079f, 0.089f));
    right_eye.push_back(Vertex(0.0952f, 0.087f));
    right_eye.push_back(Vertex(0.1093f, 0.0874f));
    right_eye.push_back(Vertex(0.1243f, 0.0897f));
    right_eye.push_back(Vertex(0.141f, 0.0936f));
    right_eye.push_back(Vertex(0.1566f, 0.0992f));
    right_eye.push_back(Vertex(0.1688f, 0.1035f));
    right_eye.push_back(Vertex(0.181f, 0.1073f));
    right_eye.push_back(Vertex(0.1938f, 0.1104f));
    right_eye.push_back(Vertex(0.206f, 0.114f));
    draw_curve(right_eye, 8);
    
    // Right pupil
    vector<Vertex> right_pupil;
    right_pupil.push_back(Vertex(0.161f, 0.148f));
    right_pupil.push_back(Vertex(0.166f, 0.132f));
    right_pupil.push_back(Vertex(0.166f, 0.116f));
    right_pupil.push_back(Vertex(0.156f, 0.099f));
    right_pupil.push_back(Vertex(0.1363f, 0.0925f));
    right_pupil.push_back(Vertex(0.116f, 0.089f));
    right_pupil.push_back(Vertex(0.098f, 0.092f));
    right_pupil.push_back(Vertex(0.087f, 0.1066f));
    right_pupil.push_back(Vertex(0.084f, 0.124f));
    right_pupil.push_back(Vertex(0.091f, 0.138f));
    right_pupil.push_back(Vertex(0.102f, 0.1494f));
    right_pupil.push_back(Vertex(0.12f, 0.158f));
    right_pupil.push_back(Vertex(0.134f, 0.159f));
    draw_curve(right_pupil, 8);
    
    // Right eyebrow
    vector<Vertex> right_eyebrow;
    right_eyebrow.push_back(Vertex(0.312f, 0.152f));
    right_eyebrow.push_back(Vertex(0.287f, 0.176f));
    right_eyebrow.push_back(Vertex(0.266f, 0.199f));
    right_eyebrow.push_back(Vertex(0.242f, 0.223f));
    right_eyebrow.push_back(Vertex(0.218f, 0.243f));
    right_eyebrow.push_back(Vertex(0.20f, 0.247f));
    right_eyebrow.push_back(Vertex(0.12f, 0.244f));
    right_eyebrow.push_back(Vertex(0.08f, 0.242f));
    right_eyebrow.push_back(Vertex(0.053f, 0.23f));
    right_eyebrow.push_back(Vertex(0.018f, 0.28f));
    draw_curve(right_eyebrow, 8);
    
    // Left eye
    vector<Vertex> left_eye;
    left_eye.push_back(Vertex(-0.164f, 0.098f));
    left_eye.push_back(Vertex(-0.151f, 0.0875f));
    left_eye.push_back(Vertex(-0.179f, 0.1086f));
    left_eye.push_back(Vertex(-0.193f, 0.122f));
    left_eye.push_back(Vertex(-0.21f, 0.132f));
    left_eye.push_back(Vertex(-0.227f, 0.139f));
    left_eye.push_back(Vertex(-0.246f, 0.142f));
    left_eye.push_back(Vertex(-0.263f, 0.141f));
    left_eye.push_back(Vertex(-0.279f, 0.136f));
    left_eye.push_back(Vertex(-0.292f, 0.124f));
    left_eye.push_back(Vertex(-0.298f, 0.111f));
    left_eye.push_back(Vertex(-0.2905f, 0.0993f));
    left_eye.push_back(Vertex(-0.2785f, 0.0924f));
    left_eye.push_back(Vertex(-0.266f, 0.0856f));
    left_eye.push_back(Vertex(-0.2504f, 0.0804f));
    left_eye.push_back(Vertex(-0.234f, 0.079f));
    left_eye.push_back(Vertex(-0.2194f, 0.0804f));
    left_eye.push_back(Vertex(-0.2056f, 0.0815f));
    left_eye.push_back(Vertex(-0.1913f, 0.083f));
    left_eye.push_back(Vertex(-0.1775f, 0.082f));
    left_eye.push_back(Vertex(-0.163f, 0.0827f));
    draw_curve(left_eye, 8);
    
    // Left pupil
    vector<Vertex> left_pupil;
    left_pupil.push_back(Vertex(-0.2125f, 0.128f));
    left_pupil.push_back(Vertex(-0.208f, 0.113f));
    left_pupil.push_back(Vertex(-0.2017f, 0.0963f));
    left_pupil.push_back(Vertex(-0.2205f, 0.083f));
    left_pupil.push_back(Vertex(-0.2364f, 0.082f));
    left_pupil.push_back(Vertex(-0.2517f, 0.083f));
    left_pupil.push_back(Vertex(-0.266f, 0.088f));
    left_pupil.push_back(Vertex(-0.276f, 0.097f));
    left_pupil.push_back(Vertex(-0.279f, 0.111f));
    left_pupil.push_back(Vertex(-0.275f, 0.125f));
    left_pupil.push_back(Vertex(-0.266f, 0.1367f));
    left_pupil.push_back(Vertex(-0.2517f, 0.142f));
    left_pupil.push_back(Vertex(-0.237f, 0.14f));
    left_pupil.push_back(Vertex(-0.224f, 0.136f));
    draw_curve(left_pupil, 8);
    
    // Left eyebrow
    vector<Vertex> left_eyebrow;
    left_eyebrow.push_back(Vertex(-0.142f, 0.247f));
    left_eyebrow.push_back(Vertex(-0.155f, 0.215f));
    left_eyebrow.push_back(Vertex(-0.17f, 0.2066f));
    left_eyebrow.push_back(Vertex(-0.1856f, 0.201f));
    left_eyebrow.push_back(Vertex(-0.199f, 0.1996f));
    left_eyebrow.push_back(Vertex(-0.218f, 0.204f));
    left_eyebrow.push_back(Vertex(-0.2355f, 0.211f));
    left_eyebrow.push_back(Vertex(-0.259f, 0.219f));
    left_eyebrow.push_back(Vertex(-0.283f, 0.228f));
    left_eyebrow.push_back(Vertex(-0.3014f, 0.235f));
    left_eyebrow.push_back(Vertex(-0.3186f, 0.243f));
    left_eyebrow.push_back(Vertex(-0.337f, 0.2346f));
    left_eyebrow.push_back(Vertex(-0.35f, 0.217f));
    left_eyebrow.push_back(Vertex(-0.359, 0.199f));
    draw_curve(left_eyebrow, 8);
    
    // Grimace
    vector<Vertex> grimace;
    grimace.push_back(Vertex(0.157f, -0.21f));
    grimace.push_back(Vertex(0.11f, -0.234f));
    grimace.push_back(Vertex(0.062f, -0.243f));
    grimace.push_back(Vertex(0.02f, -0.251f));
    grimace.push_back(Vertex(-0.019f, -0.258f));
    grimace.push_back(Vertex(-0.054f, -0.264f));
    grimace.push_back(Vertex(-0.087f, -0.256f));
    grimace.push_back(Vertex(-0.122f, -0.248f));
    grimace.push_back(Vertex(-0.159f, -0.248f));
    grimace.push_back(Vertex(-0.19f, -0.237f));
    grimace.push_back(Vertex(-0.224f, -0.224f));
    draw_curve(grimace, 8);
    
    // Nose
    vector<Vertex> nose;
    nose.push_back(Vertex(-0.07f, 0.056f));
    nose.push_back(Vertex(-0.18f, -0.076f));
    nose.push_back(Vertex(-0.187f, -0.119f));
    nose.push_back(Vertex(-0.142f, -0.146f));
    nose.push_back(Vertex(-0.066f, -0.147f));
    nose.push_back(Vertex(-0.002f, -0.132f));
    nose.push_back(Vertex(0.053f, -0.119f));
    nose.push_back(Vertex(0.006f, -0.023f));
    nose.push_back(Vertex(-0.07f, 0.056f));
    draw_curve(nose, 8);
    
    // Upper lip
    vector<Vertex> upper_lip;
    upper_lip.push_back(Vertex(0.162f, -0.21f));
    upper_lip.push_back(Vertex(0.103f, -0.212f));
    upper_lip.push_back(Vertex(0.062f, -0.215f));
    upper_lip.push_back(Vertex(0.021f, -0.215f));
    upper_lip.push_back(Vertex(-0.021f, -0.206f));
    upper_lip.push_back(Vertex(-0.058f, -0.222f));
    upper_lip.push_back(Vertex(-0.099f, -0.215f));
    upper_lip.push_back(Vertex(-0.136f, -0.219f));
    upper_lip.push_back(Vertex(-0.173f, -0.223f));
    upper_lip.push_back(Vertex(-0.211f, -0.224f));
    draw_curve(upper_lip, 8);
    
    // Lower lip
    vector<Vertex> lower_lip;
    lower_lip.push_back(Vertex(0.162f, -0.21f));
    lower_lip.push_back(Vertex(0.13f, -0.244f));
    lower_lip.push_back(Vertex(0.091f, -0.275f));
    lower_lip.push_back(Vertex(0.045f, -0.302f));
    lower_lip.push_back(Vertex(0.004f, -0.325f));
    lower_lip.push_back(Vertex(-0.043f, -0.329f));
    lower_lip.push_back(Vertex(-0.094f, -0.327f));
    lower_lip.push_back(Vertex(-0.134f, -0.31f));
    lower_lip.push_back(Vertex(-0.174f, -0.269f));
    lower_lip.push_back(Vertex(-0.211f, -0.224f));
    draw_curve(lower_lip, 8);
    
    // Left smile-line
    vector<Vertex> left_smile_line;
    left_smile_line.push_back(Vertex(-0.199f, -0.104f));
    left_smile_line.push_back(Vertex(-0.233f, -0.152f));
    left_smile_line.push_back(Vertex(-0.226f, -0.299f));
    draw_curve(left_smile_line, 8);
    
    // Right smile-line
    vector<Vertex> right_smile_line;
    right_smile_line.push_back(Vertex(0.164f, -0.306f));
    right_smile_line.push_back(Vertex(0.19f, -0.21f));
    right_smile_line.push_back(Vertex(0.08f, -0.088f));
    draw_curve(right_smile_line, 8);
    
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
