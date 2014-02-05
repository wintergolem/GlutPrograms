// =======================================
// PongTwo.cpp
// Copyright (c) Steven Hoover, 2012
// All rights reserved
// ---------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>

const int AppSize = 800;
bool rotatekey = false;
char * matrixs;
class Triangle{
public:
	float vertex1[2];
	float vertex2[2];
	float vertex3[2];
	float cx,cy;

	Triangle(){
		vertex1[0] = 200, vertex1[1] = 200;
		vertex2[0] = 400, vertex2[1] = 200;
		vertex3[0] = 300, vertex3[1] = 600;

		cx = 300;
		cy = 400;
			}

	void Draw(){
		glColor3f(0,1,0);
		glBegin(GL_TRIANGLES);
		glVertex2f(vertex1[0], vertex1[1]);
		glVertex2f(vertex2[0], vertex2[1]);
		glVertex2f(vertex3[0], vertex3[1]);
		glEnd();
	}


}triangle;

void Key(){
	if(KeyDown('r') || KeyDown('R')){
		rotatekey = true;
	}

	//close window
	if(KeyDown('Q') || KeyDown('q'))
	{
		glutDestroyWindow(1);
	}
}

void showmatrix(){
	double m[4][4];
	glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble*) m);
	for(int y =0; y<4 ;y++){
		for(int x = 0; x<4 ; y++){
			sprintf(matrixs, "%d   ", m[x][y]);
			Text((x*25 +50),(y*25 + 50),matrixs);
		}
	}
}

void Draw(){
	ClearScreen(0,0,0);
	
	//rotate triangle on r key
	if(rotatekey){
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(triangle.cx,triangle.cy, 0);
		glRotatef(35,0,0,1);
		glTranslatef(-triangle.cx,-triangle.cy,0);
	}
		triangle.Draw();
	if(rotatekey){
		//showmatrix();
		
		//rotatekey = false;
		//glPopMatrix();
	}
		
	glFlush();
}

void Idle(){
	Key();

	 glutPostRedisplay();
}

void main(){
	Make2dWindow(100, 100, AppSize, AppSize, "Pong");
	
		// set callbacks
		glutDisplayFunc(Draw);                           // respond to redraw requests
		glutIdleFunc(Idle);                              // animate ball
		//glutKeyboardFunc(Key);							 //Keystroke
		// begin event handling, display, and updates
		glutMainLoop();
}
