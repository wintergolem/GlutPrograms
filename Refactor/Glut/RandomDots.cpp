// =======================================
// Kaleidoscope.cpp
// Copyright (c) Jules Bloomenthal, 2012
// All rights reserved
// ---------------------------------------

#include "SimpleFramework.h"
#include "gl/freeglut.h"

const int APP_SIZE    = 600;

void DrawDot()
{
    glPointSize(rand()%150);
    glBegin(GL_POINTS);
    glVertex2i(rand()%APP_SIZE, rand()%APP_SIZE);
    glEnd();
}

void Draw(void)
{
	ClearScreen(0,0,0);
    float r = (rand()%128)/128.f;
    float g = (rand()%128)/128.f;
    float b = (rand()%128)/128.f;
    glColor3f(r, g, b);
    DrawDot();
    glFlush();
}

void Idle()
{
        glutPostRedisplay();
}

void main()
{
    // init app window
    Make2dWindow(100, 100, APP_SIZE, APP_SIZE, "Kaleidoscope");

    // set callbacks
    glutDisplayFunc(Draw);                           // respond to redraw requests
    glutIdleFunc(Idle);                              // animate ball

    // begin event handling, display, and updates
    glutMainLoop();
}
