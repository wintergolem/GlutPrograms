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

float wht[], blk[], red[], yel[];
const int NDISKPOINTS = 12;
float disk[NDISKPOINTS][2];
bool diskSet = false;
//bool randInitialized;

//class Circle {
void Circle::Init(int xyRange) {
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
bool Circle::Collide(Circle &c) {
    float dx = x-c.x, dy = y-c.y;
    float rsum = radius+c.radius;
    return dx*dx+dy*dy < rsum*rsum;
}
void Circle::Draw() {
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
void Circle::Update(float dt) {
        x += dt*dx;
        y += dt*dy;
        if (x < 0) {x = 0; if (dx < 0) dx = -dx;}
        if (y < 0) {y = 0; if (dy < 0) dy = -dy;};
        if (x >= range) {x = range-1.f; if (dx > 0) dx = -dx;}
        if (y >= range) {y = range-1.f; if (dy > 0) dy = -dy;}
    }
//vector<Circle> circles(3000);


void ColWidth(float *color, float width)
{
    // set GL color, point diameter and line width in pixels
    glColor3fv(color);
    glPointSize(width);
    glLineWidth(width);
}

void Line(float x1, float y1, float x2, float y2, bool round)
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

void Line(int x1, int y1, int x2, int y2, bool round)
{
    Line(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2), round);
}


//class Widget {
Widget::Widget(int x, int y, int w, int h, string name) :
x(x), y(y), w(w), h(h), name(name) 
{
}

bool Widget::Hit(int xx, int yy) {
        return xx >= x-h/2 && xx <= x+w+h/2 && yy >= y-h/2 && yy <= y+h/2;
    }

//class Button : public Widget {
Button::Button(int x, int y, int w, int h, string name) :  Widget(x, y, w, h, name) {
}
void Button::Draw() {
    ColWidth(yel, (float) h);
    Line(x, y, x+w, y);
    ColWidth(blk, (float) h-3);
    Line(x, y, x+w, y);
    ColWidth(yel, .2f);
    glRasterPos2i(x, y-7);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*) name.c_str());
}


//class Slider : public Widget {
Slider::Slider(int x, int y, int w, string name, float min, float max, float init) :
    Widget(x, y, w, 14, name), min(min), max(max) {
    loc = x+(float)((init-min)/(max-min))*w;
}
void Slider::Draw() {
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
float Slider::GetValue() {
    float v = min+((loc-x)/w)*(max-min);
    return v < min? min : v > max? max : v;
}
bool Slider::Hit(int x, int y) {
        // called upon mouse-down or mouse-drag
        bool hit = Widget::Hit(x, y);
	    if (hit)
            loc = static_cast<float>(x);
	    return hit;
    }

