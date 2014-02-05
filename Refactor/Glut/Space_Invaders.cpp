// =======================================
// Space Invaders.cpp
// Copyright (c) Steven Hoover, 2012
// All rights reserved
// ---------------------------------------


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


clock_t prevTime = clock();
float AppSize = 1000, halfsize = AppSize / 2;
float ground = static_cast<float> (halfsize-25);
const float Margin = 15;
const int Anrows = 9, Ancols = 12;
const float moveSpeed = 55;
const float tankSpeed = 210;
const float yDropDistance = 35, yTankLocate = 25;
const int missileAmount = 3;
const float alienAniWait = .5f;

float missilePrep = .8f; // seconds to wait between missile firings
clock_t wait = 0;          // no wait the first time
clock_t await = 0;		//animation wait time
// colors
float blk[] = {0, 0, 0}, wht[] = {1, 1, 1},grn[] = {0,.5f,0};
float alienSize = 22;
float score = 0;
float nCostumesX = 11;
float nCostumesY = 8;
char disScore[200];
char playerScore;
bool wallHit = false;
bool endHit = false;
bool costume[8][11] = { {false,false,true ,false,false,false,false,false,true ,false,false},
						{false,false,false,true ,false,false,false,true ,false,false,false},
						{false,false,false,true ,true ,true ,true ,true ,false,false,false},
						{false,true ,true ,false,true ,true ,true ,false,true ,true ,false},
						{true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true },
						{true ,false,true ,true ,true ,true ,true ,true ,true ,false,true },
						{true ,false,true ,false,false,false,false,false,true ,false,true },
						{true ,false,false,true ,true ,false,true ,true ,false,false,true }};

bool tankarray[6][11] = {{false,false,false,false,false,true ,false,false,false,false,false},
						{false,false,false,true ,true ,true ,true ,true ,false,false,false},
						{false,true ,true ,true ,true ,true ,true ,true ,true ,true ,false},
						{true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true },
						{true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true },
						{true ,true ,true ,true ,true ,true ,true ,true ,true ,true ,true }};
						

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

class cTank{
public:
	float x,y;
	float arrayx, arrayy;

	cTank(){
		x = 200;
		y = ground; // (halfsize-25);
		arrayx = 4;
		arrayy = 5;
	}
	void Draw() {
		for (int col = 0; col < 11; col++)
				for (int row = 0; row < 6; row++)
					if (tankarray[row][col])
						Rectangle(x+col*arrayx, y+row*arrayy, arrayx, arrayy, grn);
	}
};
cTank tank;

class alien {
public:
	bool active, ani;
	float x,y; // location of alien
	float size;
	float xd, yd;
	float costumeW, costumeH;
	alien(){
		active = true;
		ani = false;
		x = 0;
		y = 0;
		size = alienSize;
		xd = moveSpeed;
		yd = 5;
		costumeW = size/nCostumesX, costumeH = size/nCostumesY;
	}
	void Init(float row, float col) {
		x = AppSize - (col*(size+Margin));
		y = row*(size+Margin);
	}
	void Draw(){
		if(active){
			for (int row = 0; row < Anrows; row++)
				for (int col = 0; col < Ancols; col++)
					if(costume[row][col])
						Rectangle(x+col*costumeW, y+row*costumeH, size,size,wht);
		} // end if active

	} //end Draw
	void update(float dt){
		x -= xd *dt;
		if(active){
			if((x < 0 && xd >0) || (x + 50> AppSize && xd <0))
				wallHit = true;
			else if(y >= halfsize){
				endHit = true;  
		      }
		}
	}
	void redraw(){
		if(ani){
				ani = false;
				//to false
				costume[6][10] = false;
				costume[5][10] = false;
				costume[7][7] = false;
				costume[7][6] = false;
				costume[7][4] = false;
				costume[7][3] = false;
				costume[6][0] = false;
				costume[5][0] = false;

				//to true
				costume[2][10] = true ;
				costume[3][10] = true ;
				costume[7][9] = true ;
				costume[7][8] = true ;
				costume[7][2] = true ;
				costume[7][1] = true ;
			}
			else{
				ani = true;
			//to true
				costume[6][10] = true;
				costume[5][10] = true;
				costume[7][7] = true;
				costume[7][6] = true;
				costume[7][4] = true;
				costume[7][3] = true;
				costume[6][0] = true;
				costume[5][0] = true;

				//to false
				costume[2][10] = false ;
				costume[3][10] = false ;
				costume[7][9] = false ;
				costume[7][8] = false ;
				costume[7][2] = false ;
				costume[7][1] = false ;
		}//end else

		if (active) {
			//Rectangle(x, y, size, size, blk); // black alien background
			// draw white alien markings (costume)
			for (int col = 0; col < nCostumesX; col++)
				for (int row = 0; row < nCostumesY; row++)
					if (costume[row][col])
						Rectangle(x+col*costumeW, y+row*costumeH, costumeW, costumeH, wht);
		}//end of if active
	}//end of redraw
}; //end of Class
alien invaders[Anrows][Ancols];


alien *AlienHit(float y, float x)
{
	// test all aliens for hit
	// return first hit alien, or NULL if none
	for (int row = Anrows-1; row >= 0; row--)
		for (int col = 0; col < Ancols; col++) {
			alien *a = &(invaders[row][col]);
			if (a->active && (y < a->y && y > a->y - alienSize) && (x > a->x && x < (a->x +alienSize)))
				return a;
		}
	return NULL;
}
class base{
public:
	float baseSize, pixSize;
	float x,y,a;
	bool active;
	base(float place){
		x = 80 +(place*180);
		y = ground - alienSize;
		baseSize = alienSize;
		pixSize = baseSize/2;
		a = baseSize/3;
		active = true;
	}
	~base(){}
	void checkAlienHit(){
		for (int row = Anrows-1; row >= 0; row--)
			for (int col = 0; col < Ancols; col++) {
				alien *a = &(invaders[row][col]);
				if (a->active && (y < a->y && y > a->y - alienSize) && (x > a->x && x < (a->x +alienSize)))
				active = false;
		}
	}
	void Draw(){
		if(active){
			//big green square
			glColor3fv(grn);
			glBegin(GL_QUADS);
			glVertex2f(x, y);				//starting point
			glVertex2f(x, y - (2*a));		//left wall
			glVertex2f(x +4*a, y-2*a);		//top wall
			glVertex2f(x +4*a, y);			//right wall
			glVertex2f(x + 3*a, y);			//bottom wall
			glEnd();
			//small black rect to make bridge look
			glColor3fv(blk);
			glBegin(GL_QUADS);
			glVertex2f(x +a, y);
			glVertex2f(x +a, y - a);
			glVertex2f(x + 3*a, y - a);
			glVertex2f(x + 3*a, y);
			glEnd();
		}
	}
};base Base1(0),Base2(1),Base3(2),Base4(3),Base5(4);
class cmissile{
public:
	float x,y, speed;
	bool shot;
	cmissile(){
		x = 0;
		y = ground;
		shot = false;
		speed = 0;
	}
	void Reset() {
		shot = false;
		y = ground;
	}
	void init(float ax) {
		x = ax;
	}
	void update(float dt){
		if(shot){
			speed = (moveSpeed * dt);
 			y -= speed*10;
			if(y <0)
				Reset();
			alien *a = AlienHit(y,x);        
			if (a) {
				a->active = false;
				score += 10;
				Reset();
			}
		}// end of <if(shot)>
	}// end of <void shoot()>
	void Draw(){
		if(shot){
			Rectangle(x +20,y,1,5,wht);
		}
	}
}; cmissile missile[missileAmount];

void testWallHit(){
	if(wallHit){
		for (int row = 0; row < Anrows; row++)
			for (int col = 0; col < Ancols; col++){
				invaders[row][col].xd = -invaders[row][col].xd;
				invaders[row][col].y += yDropDistance;
			}
			
			wallHit = false;
		}
}
void testEndHit(){
	if(endHit){
		for (int row = 0; row < Anrows; row++)
			for (int col = 0; col < Ancols; col++){
				invaders[row][col].xd = moveSpeed;
				invaders[row][col].y = row*(invaders[1][1].size+Margin);;
			}
			
			endHit = false;
		}
}


void missileshoot(){
	// missiles can be fired at most once every missilePrep seconds
	clock_t now = clock();
	if (now > wait) {
		for (int i =0 ;i< missileAmount;i++)
			if(missile[i].shot != true){
				missile[i].shot = true;
				missile[i].init(tank.x);
				break;
			}
		wait = now+static_cast<float>(missilePrep*CLOCKS_PER_SEC);
	}
}
void resetGame(){
		for (int row = 0; row < Anrows; row++)
			for (int col = 0; col < Ancols; col++){
				invaders[row][col].xd = moveSpeed;
				invaders[row][col].y = row*(invaders[1][1].size+Margin);;
	}
}
void allvisible(){
	for (int row = 0; row < Anrows; row++)
			for (int col = 0; col < Ancols; col++)
				invaders[row][col].active = true;
}
void Draw() {
	clock_t time = clock();
	float dt = (float) (time-prevTime)/CLOCKS_PER_SEC;
	prevTime = time;
	clock_t now = clock();
	await = now+static_cast<float>(alienAniWait*CLOCKS_PER_SEC); 

	float x = AppSize, y = AppSize;
	float i = 1, h = 1;
	ClearScreen(0,0,0);

	for (int row = 0; row < Anrows; row++)
		for (int col = 0; col < Ancols; col++){
			invaders[row][col].Draw();
			if(time > await)
				invaders[row][col].redraw();
		}
	Base1.Draw();
	Base2.Draw();
	Base3.Draw();
	Base4.Draw();
	Base5.Draw();
	tank.Draw();
	for (int i = 0; i<missileAmount;i++)
		missile[i].Draw();
	
	//draw score
	glMatrixMode(GL_PROJECTION);                       // camera view mode
	glPushMatrix();
    glLoadIdentity();                                  // no rotations
	glOrtho(0, AppSize, 0, halfsize, 0, -1); 
	sprintf(disScore, "Player score: %i", score);
	Text(halfsize, halfsize, disScore);
	glMatrixMode(GL_PROJECTION);                       // camera view mode
	glPopMatrix();

    glFlush();
}
void TestKeys(float dt)
{ //move tank by left arrow | right arrow
	if(KeyDown(37)){
		if((tank.x > 0))
			tank.x -= tankSpeed *dt;
	}
	if(KeyDown(39)){
		if(tank.x +44 < AppSize)
		tank.x += tankSpeed *dt;
	}

	//space bar to fire
	if(KeyDown(32)) {
 		missileshoot();
	}

//close window
	if(KeyDown('Q') || KeyDown('q'))
	{
		glutDestroyWindow(1);
	}
	//reset game
	if(KeyDown('r') || KeyDown('R'))
	{
		resetGame();
	}
	if(KeyDown('v') || KeyDown('V'))
	{
		allvisible();
	}
}//end of Key

void Idle (){
	clock_t time = clock();
	float dt = (float) (time-prevTime)/CLOCKS_PER_SEC;
	prevTime = time;

	TestKeys(dt);
	for (int i = 0; i < missileAmount; i++)
		if (missile[i].shot)
			missile[i].update(dt);
//		missile[i].shoot(dt);
	for (int row = 0; row < Anrows; row++)
		for (int col = 0; col < Ancols; col++)
			invaders[row][col].update(dt);
	testWallHit();
	testEndHit();
    glutPostRedisplay();
}
void main(){
	// initialize GLUT
    int ac = 0;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(AppSize, halfsize);
    glutInit(&ac, NULL);
    glutCreateWindow("Space Invaders");

    // set default viewport and projection
    glViewport(0, 0, AppSize, AppSize);                            // map GL space into entire app window
    glMatrixMode(GL_PROJECTION);                       // camera view mode
    glLoadIdentity();                                  // no rotations
    glOrtho(0, AppSize, halfsize, 0, 0, -1);                        // GL space in pixels, no perspective
	//End making of window

	// initialize invaders
	for (int row = 0; row < Anrows; row++)
		for (int col = 0; col < Ancols; col++)
			invaders[row][col].Init(row, col);
		
	// set callbacks
	glutDisplayFunc(Draw);                           // respond to redraw requests
	glutIdleFunc(Idle);                              
	// begin event handling, display, and updates
	glutMainLoop();
}