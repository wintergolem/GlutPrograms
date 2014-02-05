/*  =======================================================
    CollisionBruteVsGrid.cpp - test collision detection
    Copyright (c) Jules Bloomenthal, 2013
    All rights reserved

    Exercise: convert this file into three separate source
    (.cpp) files and any needed header (.h) files:
        1. CollisionApp.cpp
            app globals
                appSize ... wht
            application routines
                Idle
                Draw
                MouseButton
                MouseDrag
                main
        2. CollisionMisc.cpp
				support routines
				    ColWidth
                    Line
				    Random
                class Circle
				class Widget
				class Slider
				class Button
        3. CollisionMethods.cpp
                Collide
                SetCircleCollisionsViaBruteForce
				class FixedGrid
 =======================================================
*/

#include <time.h>
#include <string>
#include <vector>
#include <gl/freeglut.h>
#include "CollisionApp.h"

using std::string;
using std::vector;


//***** Globals

// app
int appSize = 650;
bool showCircles = false;
bool showGrid = true;
enum Mode {Brute, Grid} mode = Grid;
extern bool randInitialized = false;

// timing
clock_t was = clock(), updateStatistics = was+CLOCKS_PER_SEC;
float fps = 0;

// collision stats
int   displayCollisionCount, nTestCollisions = 0;
float displayCellsWithPairs, nCellsWithPairs = 0;
float displayCirclesPerCell, nCirclesPerCell = 0;

// display
extern float wht[] = {1,1,1}, blk[] = {0,0,0}, red[] = {1,0,0}, yel[] = {1,1,0};


//****** Randomness

//bool randInitialized = false;

float Random(int min, int max) {
    if (!randInitialized)
        srand((unsigned) time(NULL));
    randInitialized = true;
    // random number between min and max
    float mn = static_cast<float>(min), mx = static_cast<float>(max);
    return mn+(mx-mn)*static_cast<float>(rand()%1000)/1000.f;
}
FixedGrid grid(10, appSize);
vector<Circle> circles(3000);
Slider gridResSlider(15, appSize-140, 100, "res", 2, 100, 10);
Slider nCirclesSlider(15, appSize-165, 100, "ncircs", 100, 10000, (float) circles.size());
Button modeButton(20, appSize-55, 50, 20, "toggle");
Button blankScreenButton(20, appSize-80, 50, 20, " blank");
Button showGridButton(20, appSize-105, 50, 20, " grid");


int sliderGridRes = grid.res; // temporary value, upgrade only in Idle
int sliderNCircles = circles.size();

void Idle()
{
    clock_t now = clock();
    float dt = static_cast<float>(now-was)/CLOCKS_PER_SEC;
    was = now;
	if (now > updateStatistics) {
		fps = 1.f/dt;
        displayCollisionCount = nTestCollisions;
        displayCirclesPerCell = nCirclesPerCell;
        displayCellsWithPairs = nCellsWithPairs;
		updateStatistics = now+CLOCKS_PER_SEC/3;
	}
    // test if new number of circles
    if (circles.size() != sliderNCircles) {
        int oldSize = circles.size();
        circles.resize(sliderNCircles);
        for (size_t i = oldSize; i < circles.size(); i++)
            circles[i].Init(appSize);
    }
    // update and reset circles
    for (size_t i = 0; i < circles.size(); i++) {
        Circle &c = circles[i];
        c.Update(dt);
        c.collided = false;
    }
    if (mode == Brute)
        nTestCollisions = SetCircleCollisionsViaBruteForce(circles);
    else {
        // test if new grid size
        if (grid.res != sliderGridRes)
            grid.Init(sliderGridRes, appSize);
        nTestCollisions = grid.SetCircleCollisionsViaSpatialPartition(circles);
        // get statistics
        nCirclesPerCell = nCellsWithPairs = 0;
        for (int i = 0; i < grid.nCells; i++)
            if (grid.cells[i].size() > 1) {
                nCirclesPerCell += grid.cells[i].size();
                nCellsWithPairs++;
            }
        nCirclesPerCell = nCirclesPerCell/nCellsWithPairs;
    }
    glutPostRedisplay();
}


//****** Display

void Draw()
{
    // init
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // grid
    glColor3f(0, 1, 0);
    glLineWidth(.9f);
    for (int i = 0; showGrid && i < grid.res; i++) {
        float v = grid.cellSize*i;
        Line(0.f, v, (float) appSize, v);
        Line(v, 0.f, v, (float) appSize);
    }
    // circles
    glLineWidth(1.5f);
    for (size_t i = 0; !showCircles && i < circles.size(); i++)
        circles[i].Draw();
    // statistics
	if (fps > 0) {
        char buf[1000];
		sprintf(buf, "%s %5.2f fps (%i tests)", mode == Grid? "grid:  " : "brute:", fps, displayCollisionCount);
        glColor3d(1, 1, 0);
		glRasterPos2i(20, appSize-20);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *) buf);
        if (mode == Grid) {
            sprintf(buf, "%4.2f/cell, %i cells", displayCirclesPerCell, (int) displayCellsWithPairs);
            glRasterPos2i(25, appSize-35);
            glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char *) buf);
        }
	}
    // buttons, sliders, finish
    modeButton.Draw();
    blankScreenButton.Draw();
    showGridButton.Draw();
    gridResSlider.Draw();
    nCirclesSlider.Draw();
    glFlush();
}


//****** Mouse Handlers

void MouseButton(int butn, int state, int x, int y)
{
    y = appSize-y;
    if (blankScreenButton.Hit(x, y))
        showCircles = state == GLUT_DOWN;
    if (modeButton.Hit(x, y) && state == GLUT_DOWN)
        mode = mode == Grid? Brute : Grid;
    if (showGridButton.Hit(x, y) && state == GLUT_DOWN)
        showGrid = !showGrid;
    if (gridResSlider.Hit(x, y))
        sliderGridRes = static_cast<int>(gridResSlider.GetValue());
    if (nCirclesSlider.Hit(x, y))
        sliderNCircles = static_cast<int>(nCirclesSlider.GetValue());
    glutPostRedisplay();
}

void MouseDrag(int x, int y)
{
    y = appSize-y;
    if (gridResSlider.Hit(x, y))
        sliderGridRes = static_cast<int>(gridResSlider.GetValue());
    if (nCirclesSlider.Hit(x, y))
        sliderNCircles = static_cast<int>(nCirclesSlider.GetValue());
    glutPostRedisplay();
}


//****** Application

int main(int ac, char **av)
{
    // create window
    glutInitWindowSize(appSize, appSize);
    glutInitWindowPosition(100, 100);
    glutInit(&ac, av);
    glutCreateWindow(av[0]);
    // init circles
    for (size_t i = 0; i < circles.size(); i++)
        circles[i].Init(appSize);
    // set viewport, projection and view matrices
    glViewport(0, 0, appSize, appSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, appSize+1, 0, appSize+1, 0, -1);
    // callbacks
    glutDisplayFunc(Draw);
    glutIdleFunc(Idle);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseDrag);
    // begin
    glutMainLoop();
    return 0;
}
