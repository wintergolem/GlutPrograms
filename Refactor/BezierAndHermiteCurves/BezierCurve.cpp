//--INCLUDE FILES--
#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>

using namespace std;

//--GLOBALS
int AppSize = 600;
int selected = -1;
float vec[3];
float points[4][2] = {{50,50},{50,350},{550,350},{550,50} };


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
class Slider : public Widget {
public:
    float loc, min, max; // slider position, min, max
    Slider(int x, int y, int w, string name, float min, float max, float init) :
        Widget(x, y, w, 14, name), min(min), max(max) {
        loc = x+(float)((init-min)/(max-min))*w;
    }
    void Draw() {
	    glLineWidth(2.f);
		glPointSize(2.f);
        glColor3fv(yel);
	    Line(x, y, x+w, y);
        glLineWidth(2.5f);
	    Line(loc, y-7.f, loc, y+7.f);
	    char buf[100];
	    sprintf(buf, "%s: %f", name.c_str(), GetValue());
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
}tslider (50,AppSize -50, 155, "T Slider", 0, 1, .5);

void Lerp(float *result, float *p0, float *p1, float t) {
	for (int i = 0; i < 2; i++)
		result[i] = p0[i]+t*(p1[i]-p0[i]);
}

void drawPolygon(){
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x00FF);
	glColor3f(.5f,.5f,.5f);
	for(int i =0;i<3;i++){
		glBegin(GL_LINES);
		glVertex2f(points[i][0],points[i][1]);
		glVertex2f(points[i+1][0],points[i+1][1]);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
}

void drawdot(float x, float y){
	glColor3f(1,1,1);
	glPointSize(5);
    glBegin(GL_POINTS);
	glVertex2d(x,y);
    glEnd();
}
void drawdots(float a0[2],float a1[2],float a2[2],float b0[2],float b1[2],float c0[2]){
	glColor3f(1,1,1);
	glPointSize(5);
    glBegin(GL_POINTS);
	glVertex2d(a0[0],a0[1]);
	glVertex2d(a1[0],a1[1]);
	glVertex2d(a2[0],a2[1]);
	glVertex2d(b0[0],b0[1]);
	glVertex2d(b1[0],b1[1]);
	glVertex2d(c0[0],c0[1]);
    glEnd();
}
void drawBezierConstruct(float t){
	//setting values
	float a0[2],a1[2],a2[2];
	float b0[2],b1[2];
	float c0[2];
	Lerp(a0, points[0], points[1], t);
	Lerp(a1, points[1], points[2], t);
	Lerp(a2, points[2], points[3], t);
	Lerp(b0, a0, a1, t);
	Lerp(b1, a1, a2, t);
	Lerp(c0, b0, b1, t);
	//draw a
	glColor3f(1,0,0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x00FF);
	glBegin(GL_LINES);
	glVertex2f(a0[0],a0[1]);
	glVertex2f(a1[0],a1[1]);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glColor3f(1,0,0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x00FF);
	glBegin(GL_LINES);
	glVertex2f(a1[0],a1[1]);
	glVertex2f(a2[0],a2[1]);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	//draw b
	glColor3f(0,1,0);
	//glLineStipple(1,0x00FF);
	//glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(b0[0],b0[1]);
	glVertex2f(b1[0],b1[1]);
	glEnd();
	//glDisable(GL_LINE_STIPPLE);

	//draw c
	glColor3f(0,0,1);
	glPointSize(15);
    glBegin(GL_POINTS);
	glVertex2d(c0[0],c0[1]);
    glEnd();

	drawdots(a0,a1,a2,b0,b1,c0);
}
void makeBezierCurve(float *result,float *a, float *b,float c[], float d[], float t){
	float dt = t*t;
	float dat = t*t*t;

	for(int i =0; i<2; i++){
		result[i] = ((-dat + 3*dt - 3*t +1)*a[i]+ ( 3*dat -6*dt + 3*t) * b[i]+ (-3*dat + 3*dt)*c[i]+dat*d[i]);
	}
}
void drawBezierCurve(){
	float result[2];
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t < 1; t += 0.01f) {
//	for(int i=0;i<100;i++){
		makeBezierCurve(result,points[0],points[1],points[2],points[3], t);
		//glVertex2f(result[0],result[1]);
		glVertex2fv(result);
	}
	glEnd();
}
void draw(){
	ClearScreen(0,0,0);
	drawPolygon();
	for(int i=0;i<4;i++){
		drawdot(points[i][0],points[i][1]);
	}
	drawBezierConstruct(tslider.GetValue() );
	drawBezierCurve();
	tslider.Draw();
	glFlush();
}

void idle(){
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y){
	//points on polygon
	selected = -1;
	if(state == GLUT_DOWN)
		for(int i =0;i<4;i++)
				if (abs(points[i][0]-x) < 5 && abs(points[i][1]-y) < 5)
					selected = i;

	//slider
	tslider.Hit(x,y);
}

void MouseDrag(int x, int y){
	//slider
	tslider.Hit(x,y);
	if(selected != -1) {
		points[selected][0] = static_cast<float>(x);
		points[selected][1] = static_cast<float>(y);
	}
	glutPostRedisplay();
}

void main(){
	int ac = 0;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(AppSize, AppSize);
    glutInit(&ac, NULL);
    glutCreateWindow("Space Invaders");

    // set default viewport and projection
    glViewport(0, 0, AppSize, AppSize);                            // map GL space into entire app window
    glMatrixMode(GL_PROJECTION);                       // camera view mode
    glLoadIdentity();                                  // no rotations
    glOrtho(0, AppSize, AppSize, 0, 0, -1);                        // GL space in pixels, no perspective
	//End making of window

	// set callbacks
	glutDisplayFunc(draw);                           // respond to redraw requests
	glutIdleFunc(idle);  
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseDrag);
	// begin event handling, display, and updates
	glutMainLoop();
}