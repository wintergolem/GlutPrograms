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
int AppSize = 800, halfsize = AppSize /2, sqrsize = AppSize/50;
float globalspeed = 50;
float red[] = {1, 0, 0}, blk[] = {0, 0, 0}, blu[] = {0, 0, 1}, gry[] = {.5f, .5f, .5f}, wht[] = {1,1,1};
float *colors[] = {wht, red, blu, gry};
clock_t prevTime = clock();


void Rectangle(float x, float y, float w, float h, float *col = NULL)
{
	if (col)
		glColor3fv(col);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

class ground {
public:
	float x,y;
	float size;
	bool solid;
	float color;
	ground(){
		size = static_cast<float>(AppSize)/50;
	}
	void init(int row, int col){
		x = (col*size);
		y = (row*size);
	}
	void Draw(){
		if(solid)
			Rectangle(x,y,size,size,gry);
	}
} world[50][50];
class person{
public:
	float x,y,size,speed;
	person(){
		x = static_cast<float>(halfsize);
		y = static_cast<float>(halfsize);
		size = static_cast<float>(sqrsize);
		speed = globalspeed * 2;
	}
	void Draw(){
		Rectangle(x,y,size,size,red);
	}
	bool testWall(){
		for (int row = 0; row < 50; row++)
			for (int col = 0; col < 50; col++) {
				
		}
	}
	~person(){}
} MC;
void TestKeys(float dt){
	if(KeyDown(37) && MC.x > 0){				//toward left
		if(MC.testWall())	
		MC.x -= MC.speed *dt;
	}
	if(KeyDown(39) && MC.x < (AppSize -15)){	//toward right
		MC.x += MC.speed *dt;
	}
	if(KeyDown(38) && MC.y < AppSize - 15){			//toward up
		MC.y += MC.speed *dt;
	}
	if(KeyDown(40) && MC.y > 0){			//toward down
		MC.y -= MC.speed *dt;
	}
	//space bar to fire
	if(KeyDown(32)) {
// 		missileshoot();
	}

//close window
	if(KeyDown('Q') || KeyDown('q'))
	{
		glutDestroyWindow(1);
	}
	//reset game
	if(KeyDown('r') || KeyDown('R'))
	{
//		resetGame();
	}
	if(KeyDown('v') || KeyDown('V'))
	{
//		allvisible();
	}
}

void drawworld(){
	for(int row=0;row<50;row++)
		for(int col=0;col<50;col++)
			world[row][col].Draw();
}
void Draw(){
	ClearScreen(0,0,0);
	glColor3f(255, 0, 205);
	drawworld();
	MC.Draw();
	glFlush();
}
void Idle(){
	clock_t time = clock();
	float dt = dt = static_cast<float>(time-prevTime)/CLOCKS_PER_SEC;
	prevTime = time;

	TestKeys(dt);
    glutPostRedisplay();
}
void main(){
	Make2dWindow(100, 100, AppSize, AppSize, "Shooter");
	//randomly assign solids
	for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++) {
				bool solid = rand()%8 == 0;
				if(solid) {
					world[i][j].solid = true;
				}
	//assign
	for (int row = 0; row < 50; row++)
		for (int col = 0; col < 50; col++)
			world[row][col].init(row, col);
		}

	glutDisplayFunc(Draw); 
	glutIdleFunc(Idle);                              
	// begin event handling, display, and updates
	glutMainLoop();
}