//--INCLUDE FILES--
#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "CollisionLibraryHeader.h"

using namespace std;

//Globals
int AppsizeInt = 600;
enum state {CC,AABB,RP,SP,AABBP};
state AppState;
int selected = -1;

//template <typename T>
//T *Vector;

//square class
class square{
public:
	float x,y,size;
	unsigned int id;

	void init(int x, int y);
};
vector<square> squares;
void square::init(int x,int y){
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
	size = 25;
	id = squares.size();
}
//circle class
class circle{
public:
	float x,y;
	unsigned int id;

	void init(int x,int y);
};
vector<circle> circles;

union Shape {
	vector<circle> *c;
	vector<square> *s;
};

void circle::init(int x, int y){
		this->x = static_cast<float>(x);
		this->y = static_cast<float>(y);
		id = circles.size();
	}

//draw funcs
void destory(){
	circles.clear();
}
void drawCC(){
	for(vector<circle>::iterator it= circles.begin();it != circles.end(); it++){
		glColor3f(1,0,0);
		Disk(static_cast<float>(it->x),static_cast<float>(it->y),20 );
	}
	
}
void drawAABB(){
	for(vector<square>::iterator it= squares.begin();it != squares.end(); it++){
		glColor3f(1,0,0);
		Rectangle(static_cast<float>(it->x),static_cast<float>(it->y),static_cast<float>(it->x + it->size),static_cast<float>(it->y + it->size) );
	}
}
void drawRP(){

}
void drawSP(){

}
void drawAABBP(){

}
void draw(){
	ClearScreen(0,0,0);
	if(AppState == CC){
		drawCC();
	}
	else if(AppState == AABB){
		drawAABB();
	}
	else if(AppState == RP){
		drawRP();
	}
	else if(AppState == SP){
		drawSP();
	}
	else if(AppState == AABBP){
		drawAABBP();
	}
	else{
		glRasterPos2i(static_cast<int>(  (AppsizeInt/12) * 5), static_cast<int>(  (AppsizeInt / 12) *8)  );
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*) ("Select State"));
	}
	glFlush();
}
//idle funcs
void idle(){
	glutPostRedisplay();
}

bool colli
Shape s;
int nPoints = 0;
float selectablePoints[2][2];
int selectedPoint = -1;

void TestKeys(unsigned char key, int x, int y){
	if(key == 49){
		destory();
		AppState = CC;
	}
	else if(key == 50){
		destory();
		AppState = AABB;
	}
	else if(key == 51){
		destory();
		AppState = RP;
	}
}
void allocObj(int x,int y){
	if(AppState == CC){
		circle c;
		c.init(x,y);
		circles.push_back(c);
	}
	else if(AppState == AABB){
		square c;
		c.init(x,y);
		squares.push_back(c);
	}
}
void typedefing(){
	if(AppState == CC){
		s.c = &circles;
		Vector = &circles;
	}
	else if(AppState == AABB) {
		s.s = &squares;
		Vector = &square;
	}
}
void MouseButton(int button, int state, int x, int y){
	selected = -1;
	if(button == GLUT_LEFT_BUTTON && state == 0){
		if(Vector->size() < 2)	
			allocObj(x,y);
		else{
			for(vector<circle>::iterator it= circles.begin();it != circles.end(); it++)
				if (abs(it->x)-x < 25 && abs(it->y-y) < 25)
					selected = it->id;
		}
	}
}

void MouseDrag(int x , int y){
	if(selected != -1){
		if(AppState == CC){
			circles.at(selected).x = static_cast<float>(x);
			circles.at(selected).y = static_cast<float>(y);
		}
		if(AppState == AABB){
			squares.at(selected).x = static_cast<float>(x);
			squares.at(selected).y = static_cast<float>(y);
		}
	}
	// collision test
	glutPostRedisplay();
}

void main(){
	AppState = CC;

	//window
	int ac = 0;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(AppsizeInt, AppsizeInt);
    glutInit(&ac, NULL);
    glutCreateWindow("Spanvaders");

    // set default viewport and projection
    glViewport(0, 0, AppsizeInt, AppsizeInt);                            // map GL space into entire app window
    glMatrixMode(GL_PROJECTION);                       // camera view mode
    glLoadIdentity();                                  // no rotations
    glOrtho(0, AppsizeInt, AppsizeInt, 0, 0, -1);                        // GL space in pixels, no perspective
	//End making of window

	// set callbacks
	glutDisplayFunc(draw);                           // respond to redraw requests
	glutIdleFunc(idle);  
	glutKeyboardFunc(TestKeys);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseDrag);
	// begin event handling, display, and updates
	glutMainLoop();
}