//--INCLUDE FILES--
#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

//--GLOBALS
int AppSize = 600;
int selected = -1;
float vec[3];
float wht[] = {1,1,1}, blk[] = {0,0,0}, red[] = {1,0,0}, yel[] = {1,1,0};
//float points[4][2] = {{50,50},{50,350},{550,350},{550,50} };

class Point{
public:
	float pos[2];
	float tang[2];
	bool set;

	Point(float x, float y){
		pos[0] = static_cast<float>(x);
		pos[1] = static_cast<float>(y);
		set = false;
	}
	void draw(){
		glColor3f(1,0,0);
		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex2f(pos[0],pos[1]);
		glEnd();
	}
};

vector<Point> points;

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
}cslider (50,AppSize -50, 155, "C Slider", 0, 1, .5);

void drawdots(){
	for(vector<Point>::iterator it = points.begin();it != points.end(); it++){
		it->draw();
	}
}
void calCurve(float p1[],float p2[], float v1[], float v2[], float *result, float t){
	float a[2],b[2],c[2],d[2];
	float t2 = t*t, t3 =t*t*t;
	for( int i=0; i<2; i++){
		a[i] = -2 * p2[i] + 2 * p1[i] + v1[i] + v2[i];
		b[i] = -3 * p1[i] + 3 * p2[i] - 2 *v1[i] - v2[i];
		c[i] = v1[i];
		d[i] = p1[i];
		
		result[i] = a[i] * t3 + b[i] * t2 + c[i] * t + d[i];
	}
}
void drawcurve(){
	for(vector<Point>::iterator it = points.begin(); it != points.end(); it++)
		if(it->set == false) return;
	for(vector<Point>::iterator it2 = points.begin(); it2 != points.end()-1; it2++){
		vector<Point>::iterator it3 = it2+1;

		float result[2];
		glColor3f(0,0,1);
		glBegin(GL_LINE_STRIP);
		for(float t =0;t<1;t += 0.01f){
			calCurve(it2->pos,it3->pos,it2->tang,it3->tang,result,t);
			glVertex2fv(result);
		}
		glEnd();
	}
}

void drawlines(){
	if(points.size() >1){
			/*glEnable(GL_LINE_STIPPLE);
			glLineStipple(1,0x00FF);*/
			glBegin(GL_LINE_STRIP);
			glColor3f(1,0,0);
			glVertex2f(points[0].pos[0], points[0].pos[1] );
			for(vector<Point>::iterator it = points.begin()+1;it != points.end(); it++)
				glVertex2f(it->pos[0],it->pos[1]);
			glEnd();
			//glDisable(GL_LINE_STIPPLE);
		}
}
//hermite curve
void allocdot(int a, int b){
	float x = static_cast<float>(a);
	float y = static_cast<float>(b);
	points.push_back( Point(x,y) );
}
void alloctangent(){
	float c = cslider.GetValue();

	/*
	vector<Point>::iterator it
	vector<Point>::iterator itn = it +1;
	vector<Point>::iterator itp = it -1;
	*/
	if(points.size() == 0 || points.size() == 1)
		return;
	if(points.size() == 2){//two points
		vector<Point>::iterator it = points.begin();
		//set first
		it->tang[0] = c*( (it+1)->pos[0] - it->pos[0] );
		it->tang[1] = c*( (it+1)->pos[1] - it->pos[1] );
		it->set = true;
		//set last
		it = points.end()-1 ;
		it->tang[0] = c*( it->pos[0] - (it-1)->pos[0] );
		it->tang[1] = c*( (it)->pos[1] - (it-1)->pos[1] );
		it->set = true;
	}
	if(points.size() >= 3){//three plus
		vector<Point>::iterator it = points.begin();
		//set first
		it->tang[0] = c*( (it+1)->pos[0] - it->pos[0] );
		it->tang[1] = c*( (it+1)->pos[1] - it->pos[1] );
		it->set = true;
		//set middle
		for(vector<Point>::iterator it = points.begin() +1; it != points.end() -1; it++){
			it->tang[0] = c*( (it+1)->pos[0] - (it-1)->pos[0] );
			it->tang[1] = c*( (it+1)->pos[1] - (it-1)->pos[1] );
			it->set = true;
		}
		//set last
		it =  points.end() -1;
		it->tang[0] = c*( (it)->pos[0] - (it-1)->pos[0] );
		it->tang[1] = c*( (it)->pos[1] - (it-1)->pos[1] );
		it->set = true;
	}
}
void draw(){
	ClearScreen(0,0,0);
	cslider.Draw();
	drawdots();
	//drawlines();
	if(points.size() > 2)
		drawcurve();
	glFlush();
}

void idle(){
	alloctangent();
	
	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y){
	selected = -1;
	if(cslider.Hit(x,y) ) return;

	if(button == GLUT_LEFT_BUTTON && state == 0){
			allocdot(x,y);
		}
	if(button == GLUT_RIGHT_BUTTON && state ==0){
		
		for(unsigned int i =0;i<points.size() ;i++)
				if (abs(points[i].pos[0]-x) < 10 && abs(points[i].pos[1]-y) < 10)
					selected = i;
	}
	
}

void MouseDrag(int x, int y){
	//slider
	cslider.Hit(x,y);
	if(selected != -1) {
		points[selected].pos[0] = static_cast<float>(x);
		points[selected].pos[1] = static_cast<float>(y);
	}
	glutPostRedisplay();
}

void main(){
	int ac = 0;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(AppSize, AppSize);
    glutInit(&ac, NULL);
    glutCreateWindow("Spanvaders");

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