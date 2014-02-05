// =======================================
// TinyTanksMain.cpp
// Copyright (c) Steven Hoover, 2012
// All rights reserved
// ---------------------------------------
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <assert.h>


using namespace std;

//Globals
int Appsize = 600;
float Appsizef= 600;
int numOfc = 1000;
int state = 0;

const int GRIDSIZE = 10;
const int CELLSIZE = Appsize/GRIDSIZE;

vector<int> grid [GRIDSIZE][GRIDSIZE];

clock_t prevTime;
clock_t now = clock();



class circle {
public:
	double x,y;
	double radius;
	double dx,dy;
	bool collided;

	circle(){
		
		x = rand() % 600 ;
		y = rand() % 600 ;
		radius = rand() %10 +5;

		dx = ((rand() % 5) -2 )*10 ;
		dy = ((rand() % 5) -2 )*10 ;
	}

	void draw(){

		if(collided)
			glColor3f(0,0,1);
		else
			glColor3f(1,1,1);
		glPointSize(static_cast<GLfloat>((radius)*2));
		glBegin(GL_POINTS);
		glVertex2d(x,y);
		glEnd();
	}
	void Update(double dt){
		
		x += dx *dt;
		y += dy * dt;

		//stay on screen
		if (x < 0 || x >= Appsize) {
			dx = -dx;
			if (x < 0)
				x = 0;
			if (x >= Appsize)
				x = Appsize-1;
		}
		if (y < 0 || y >= Appsize) {
			dy = -dy;
			if (y < 0)
				y = 0;
			if (y >= Appsize)
				y = Appsize-1;
		}
		/*if(x <0 || x >= Appsize || y <0 || y>= Appsize){
			dx = -dx;
			dy = -dy;
		}*/
	}
} circles[1000];

bool collisionMath(int i, int j){
		circle one = circles[i];
		circle two = circles[j];
		double delta_x = (one.x - two.x);
		double delta_y = (one.y - two.y);
		double distance_between = delta_x * delta_x + delta_y * delta_y;
		double collision_distance = one.radius*two.radius + two.radius*one.radius;
		return (distance_between <= collision_distance);
}

void bruteForce(circle &c, int i){
	
	for(int j=0;j<numOfc;j++){
			if(j!=i){
					//circles[i].collided = false;
					//circles[j].collided = false;
				
				if(collisionMath(i,j)){
					circles[i].collided = true;
					circles[j].collided = true;
				}
				
		}
	}

}
//void Collision(){
//	for(int i=0;i<numOfc;i++){
//		for(int j=0;j<numOfc;j++){
//			if(i != j)
//
//				if(grid[i][j].size >1){
//					for(vector<int>::iterator it = grid[i][j].begin();it !=grid[i][j].end(); it++){
//						circle a = circles[it];
//					}
//				}
//			
//		}
//}

void Draw(){
	ClearScreen(0,0,0);
	for(int i = 0; i< numOfc; i++){
		circles[i].draw();
	}
	glFlush();
}

void TestKey(){
	if(KeyDown(32)){
		if(state ==0)
			state =1;
		else
			state =0;
	}
}

void Idle(){
	clock_t time = clock();
	double dt = (double) (time-prevTime)/CLOCKS_PER_SEC;
	prevTime = time;

	TestKey();

	for(int i =0; i<numOfc; i++){
		circles[i].Update(dt);
	}

	// reset state of circle collisions
	for (int i = 0; i < numOfc; i++)
		circles[i].collided = false;

	if(state ==0) //brute force
		for(int i =0;i<numOfc;i++)
			bruteForce(circles[i], i);

	if(state == 1){//fixed grid
		// reset grid
		for (int i = 0; i < GRIDSIZE; i++)
			for (int j = 0; j < GRIDSIZE; j++)
				grid[i][j].resize(0);
		// assign circle indices to those grid elements intersected by circle
		for(int i =0;i<numOfc; i++) {
			circle &c = circles[i];
			// determine in which cell the circle center is
			int a = static_cast<int>(c.x/CELLSIZE);
			int b = static_cast<int>(c.y/CELLSIZE);
			assert(a >= 0 && b >= 0 && a < GRIDSIZE && b < GRIDSIZE);
			grid[a][b].push_back(i);
			// now, determine if any adjacent cells covered by circle
			// <code here!>
		}
		// test collision for multiple circles in a grid element
		for (int i = 0; i < GRIDSIZE; i++)
			for (int j = 0; j < GRIDSIZE; j++) {
				vector<int> &ids = grid[i][j];
				int ncircs = ids.size();
				if (ncircs > 1) {
					// brute force test for actual collision of all circles in this grid cell
					for (int i1 = 0; i1 < ncircs; i1++) {
						int c1 = ids[i1];
						for (int i2 = 0; i2 < ncircs; i2++) {
							int c2 = ids[i2];
							if (c1 != c2 && collisionMath(c1, c2)) {
								circles[c1].collided = circles[c2].collided = true;
							}
						}
					}
				}
			}
	}

	glutPostRedisplay();
}

void main(){
	Make2dWindow(100, 100, Appsize, Appsize, "Spacial Collision");
	//for (int i = 0; i < GRIDSIZE; i++)
	//	for (int j = 0; j < GRIDSIZE; j++)
	//		printf("grid[%i][%i] size = %i\n", i, j, grid[i][j].size());
	srand( clock() );
	// set callbacks
	glutDisplayFunc(Draw);                           // respond to redraw requests
	glutIdleFunc(Idle);                              
	// begin event handling, display, and updates
	glutMainLoop();
}