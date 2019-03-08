//
//  main.cpp
//  opengl_test_main_osx
//
//  Created by Merci on 1/27/18.
//  Copyright © 2018 Merci. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void display() {
    // Do nothing for now
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Test Window Mac OSX");
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
