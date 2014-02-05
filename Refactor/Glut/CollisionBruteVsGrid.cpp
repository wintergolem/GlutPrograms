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

using std::string;
using std::vector;


//***** Globals

// app
int appSize = 650;
bool showCircles = false;
bool showGrid = true;
enum Mode {Brute, Grid} mode = Grid;

// timing
clock_t was = clock(), updateStatistics = was+CLOCKS_PER_SEC;
float fps = 0;

// collision stats
int   displayCollisionCount, nTestCollisions = 0;
float displayCellsWithPairs, nCellsWithPairs = 0;
float displayCirclesPerCell, nCirclesPerCell = 0;

// display
float wht[] = {1,1,1}, blk[] = {0,0,0}, red[] = {1,0,0}, yel[] = {1,1,0};


//****** Randomness

bool randInitialized = false;

float Random(int min, int max) {
    if (!randInitialized)
        srand((unsigned) time(NULL));
    randInitialized = true;
    // random number between min and max
    float mn = static_cast<float>(min), mx = static_cast<float>(max);
    return mn+(mx-mn)*static_cast<float>(rand()%1000)/1000.f;
}


//****** Moving Circle

bool diskSet = false;
const int NDISKPOINTS = 12;
float disk[NDISKPOINTS][2];

class Circle {
public:
    float range, x, y, dx, dy, radius;
    bool collided;
    void Init(int xyRange) {
        range = static_cast<float>(xyRange);
        int margin = xyRange/4;
        x = Random(margin, xyRange-margin);
        y = Random(margin, xyRange-margin);
        float mag2 = Random(5, 50), rad2 = 3.1415926f*Random(0, 360)/180.f;
        dx = mag2*cos(rad2);
        dy = mag2*sin(rad2);
        radius = Random(0, 7);
        collided = false;
    }
    bool Collide(Circle &c) {
        float dx = x-c.x, dy = y-c.y;
        float rsum = radius+c.radius;
        return dx*dx+dy*dy < rsum*rsum;
    }
    void Draw() {
        if (!diskSet)
            for (int i = 0; i < NDISKPOINTS; i++) {
                float angle = 2*3.14159f*i/NDISKPOINTS, *c = disk[i];
                c[0] = cos(angle);
                c[1] = sin(angle);
            }
        diskSet = true;
        glColor3fv(collided? red : wht);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < NDISKPOINTS; i++) {
            float *c = disk[i];
            glVertex2f(x+radius*c[0], y+radius*c[1]);
        }
        glEnd();
    }
    void Update(float dt) {
        x += dt*dx;
        y += dt*dy;
        if (x < 0) {x = 0; if (dx < 0) dx = -dx;}
        if (y < 0) {y = 0; if (dy < 0) dy = -dy;};
        if (x >= range) {x = range-1.f; if (dx > 0) dx = -dx;}
        if (y >= range) {y = range-1.f; if (dy > 0) dy = -dy;}
    }
};
vector<Circle> circles(3000);


//****** Collision Detection

int SetCircleCollisionsViaBruteForce(vector<Circle> &circles)
{
    int nTests = 0;
    // brute force: test all circles against all other circles
    for (size_t k = 0; k < circles.size(); k++) {
        Circle &c = circles[k];
        for (size_t kk = k+1; kk < circles.size(); kk++) {
            Circle &c2 = circles[kk];
            if (!c.collided || !c2.collided) {
                nTests++;
                if (c.Collide(c2))
                    c.collided = c2.collided = true;
            }
        }
    }
    return nTests;
}


//****** Grid

class FixedGrid {
public:
    int res, nCells;
    float cellSize;
    vector<vector<int>> cells;
    FixedGrid(int res, int pixelSize) {
        Init(res, pixelSize);
    }
    void Init(int res, int pixelSize) {
        this->res = res;
        cells.resize(nCells = res*res);
        cellSize = static_cast<float>(pixelSize)/res;
    }
    int Index(float v) {
        int i = static_cast<int>(v/cellSize);
        return i < 0? 0 : i >= res? res-1 : i;
    }
    int SetCircleCollisionsViaSpatialPartition(vector<Circle> &circles) {
        int nTests = 0;
        // clear cells
        for (int i = 0; i < nCells; i++)
            cells[i].resize(0);
        // set cells for each circle
        for (size_t k = 0; k < circles.size(); k++) {
            Circle &c = circles[k];
            for (int i = Index(c.x-c.radius); i <= Index(c.x+c.radius); i++)
                for (int j = Index(c.y-c.radius); j <= Index(c.y+c.radius); j++)
                    cells[i+j*res].push_back(k);
        }
        // check cells
        for (int i = 0; i < nCells; i++) {
            vector<int> &ids = cells[i];
            int nids = ids.size();
            // test all circle pairings
            for (int k = 0; k < nids; k++) {
                Circle &c = circles[ids[k]];
                for (int kk = k+1; kk < nids; kk++) {
                    Circle &c2 = circles[ids[kk]];
                    if (!c.collided || !c2.collided) {
                        nTests++;
                        if (c.Collide(c2))
                            c.collided = c2.collided = true;
                    }
                }
            }
        }
        return nTests;
    }
} grid(10, appSize);


//****** Graphics

void ColWidth(float *color, float width)
{
    // set GL color, point diameter and line width in pixels
    glColor3fv(color);
    glPointSize(width);
    glLineWidth(width);
}

void Line(float x1, float y1, float x2, float y2, bool round = true)
{
    // draw line from (x1,y1) to (x2,y2), optionally round endpoints
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    if (round) {
        glBegin(GL_POINTS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}

void Line(int x1, int y1, int x2, int y2, bool round = true)
{
    Line(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2), round);
}


//****** UI

class Widget {
public:
    // a horizontal button between x and x+w, centered at y and h thick
    int x, y, w, h;
    string name;
    Widget(int x, int y, int w, int h, string name) :
    x(x), y(y), w(w), h(h), name(name) {
    }
    bool Hit(int xx, int yy) {
        return xx >= x-h/2 && xx <= x+w+h/2 && yy >= y-h/2 && yy <= y+h/2;
    }
};

class Button : public Widget {
public:
    Button(int x, int y, int w, int h, string name) :
        Widget(x, y, w, h, name) {
    }
    void Draw() {
        ColWidth(yel, (float) h);
        Line(x, y, x+w, y);
        ColWidth(blk, (float) h-3);
        Line(x, y, x+w, y);
        ColWidth(yel, .2f);
        glRasterPos2i(x, y-7);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*) name.c_str());
    }
} modeButton(20, appSize-55, 50, 20, "toggle"),
  blankScreenButton(20, appSize-80, 50, 20, " blank"),
  showGridButton(20, appSize-105, 50, 20, " grid");

class Slider : public Widget {
public:
    float loc, min, max; // slider position, min, max
    Slider(int x, int y, int w, string name, float min, float max, float init) :
        Widget(x, y, w, 14, name), min(min), max(max) {
        loc = x+(float)((init-min)/(max-min))*w;
    }
    void Draw() {
	    glLineWidth(2.f);
        glColor3fv(yel);
	    Line(x, y, x+w, y);
        glLineWidth(2.5f);
	    Line(loc, y-7.f, loc, y+7.f);
	    char buf[100];
	    sprintf(buf, "%s: %i", name.c_str(), static_cast<int>(GetValue()));
	    glRasterPos2i(x, y-15);
	    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*) buf);
    }
    float GetValue() {
        float v = min+((loc-x)/w)*(max-min);
        return v < min? min : v > max? max : v;
    }
    bool Hit(int x, int y) {
        // called upon mouse-down or mouse-drag
        bool hit = Widget::Hit(x, y);
	    if (hit)
            loc = static_cast<float>(x);
	    return hit;
    }
} gridResSlider(15, appSize-140, 100, "res", 2, 100, 10),
  nCirclesSlider(15, appSize-165, 100, "ncircs", 100, 10000, (float) circles.size());


//****** Animation

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
