#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>

#define RES 800 

int random(double min, double max){
	int a = rand()%RES;

	return a;
}

void Draw (){

}
void Idle(){
	double x = random(0,RES);
	double y = random(0,RES);
	glColor3f(1, 1, 0);
	Disk(x,y,0.1f);
	glFlush();
	glutPostRedisplay();
}


void main (){
	Make2dWindow(100,100,800,800,"");
	//glutDisplayFunc(Draw);                           // respond to redraw requests
	glutIdleFunc(Idle);                              
	// begin event handling, display, and updates
	glutMainLoop();

}