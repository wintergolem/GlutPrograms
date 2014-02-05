////// =======================================
////// PongOne.cpp
////// Copyright (c) Steven Hoover, 2012
////// All rights reserved
////// ---------------------------------------
////
////#include <stdio.h>
////#include <stdlib.h>
////#include "SimpleFramework.h"
////#include "gl/freeglut.h"
////#include <time.h>
////
////const int APP_SIZE    = 600, halfSize = APP_SIZE/2;
////int playerOneScore =-1, playerTwoScore =0;
////
////double xBall =APP_SIZE / 2 , yBall = APP_SIZE /2; // pos
////double dxBall = 150, dyBall = 25; // vel in pix/sec
////double yPaddleOne = 250, yPaddleTwo = 250;
////double xPaddleSize = 10, yPaddleSize = 50;
////double xPaddleOne = 50, xPaddleTwo = APP_SIZE - 50;
////
////char strscore[5000];
////char winMess[3000];
////
////clock_t prevTime = clock();
////
////bool oneWin = false, twoWin = false;
////
////void option()
////{
////	char c;
////	do {
////		printf("Press T for Two Players");
////		printf("Press B for Bad Background");
////		printf("Press S for Start Menu");
////		scanf("%c", &c);
////		if (c == 'T') {
////		}
////		if (c == 'B') {
////		}
////	} while (c != 'S');
////}
////
////void DrawDot()
////{
////    glPointSize(10);
////    glBegin(GL_POINTS);
////    glVertex2d(xBall,yBall);
////    glEnd();
////}
////
////void DrawPaddleOne()
////{
////	glBegin(GL_QUADS);
////    glVertex2d(xPaddleOne, yPaddleOne);
////    glVertex2d(xPaddleOne, yPaddleOne + yPaddleSize);
////    glVertex2d(xPaddleOne + xPaddleSize, yPaddleOne + yPaddleSize);
////    glVertex2d(xPaddleOne + xPaddleSize, yPaddleOne);
////    glEnd();
////}
////
////void DrawPaddleTwo()
////{
////	glBegin(GL_QUADS);
////    glVertex2d(xPaddleTwo, yPaddleTwo);
////    glVertex2d(xPaddleTwo, yPaddleTwo + yPaddleSize);
////    glVertex2d(xPaddleTwo + xPaddleSize, yPaddleTwo + yPaddleSize);
////    glVertex2d(xPaddleTwo + xPaddleSize, yPaddleTwo);
////    glEnd();
////}
////
////void Draw(void)
////{
////	ClearScreen(0,0,0);
////	//color variables
////    float r = (rand()%128)/128.f;
////    float g = (rand()%128)/128.f;
////    float b = (rand()%128)/128.f;
////    glColor3f(r, g, b);
////    DrawDot();
////	DrawPaddleOne();
////	DrawPaddleTwo();
////	glColor3f(255, 255, 255);
////	sprintf(strscore, "Player 1 score: %i | Player 2 score: %i", playerOneScore, playerTwoScore);
////	Text(2000, 5000, strscore);
////
////	//check win
////	if(oneWin)
////	{
////		Text(2000,5500,"Player One Won");
////		playerOneScore = 0;
////		playerTwoScore = 0;
////	}
////	else if(twoWin)
////	{
////		Text(2000,5500,"Player Two Won");
////		playerOneScore = 0;
////		playerTwoScore = 0;
////	}
////
////    glFlush();
////}
////
////void Key (unsigned char c, int x, int y)
////{
////	switch (c) {
////	case 'w':
////		if(yPaddleOne + yPaddleSize < APP_SIZE)
////		{
////			yPaddleOne += 10;
////			// raise left paddle
////		}
////		break;
////	case 'x':	
////		if(yPaddleOne > 0) 
////		{
////			yPaddleOne -= 10;
////			//Lower Left Paddle
////		}
////		break;
/////*	case 'o': 
////		if (yPaddleTwo + yPaddleSize < APP_SIZE)
////		{
////			  yPaddleTwo += 10;
////			// raise left paddle
////		}
////		break;
////	case 'm': 
////		if (yPaddleTwo > 0)
////		{
////			yPaddleTwo -= 10;
////			//Lower Left Paddle
////		}
////		break;
////	*/
////	}
////}
////
////void reset()
////{
////	xBall = halfSize;
////	yBall = halfSize;
////}
////
////const double ballSpeed = .2;
////
////int startMenu()
////{
////	// ask user for command
////	// return 0: <none>, 1: Quit, 2: High Scores, 3: Start Game
////	printf("Welcome to Pong \n");
////	printf("ENTER     Start New Game \n");
////	printf("H         High Scores \n");
////	printf("P         Options\n");
////	printf("Q         Quit\n");
////	printf("Command: ");
////	char c;
////	scanf("%c", &c);
////	printf("user typed %c\n", c);
////	switch(c) {
////		case 'q':
////			return 1;
////			break;
////		case 'h':
////			return 2;
////			break;
////		case 'p':
////			return 3;
////		default:
////			return 4;
////	}
////}
////
////void UpdateBall(double dt) 
////{
////	// moveball
////	xBall += dxBall*dt;
////	yBall += dyBall *dt;
////
////	//change direction
////	//use ball x
////	if((xBall > APP_SIZE && dxBall >0))
////	{
////		reset();
////		playerOneScore++;
////
////		if(playerOneScore >= 7)
////		{
////			oneWin = true;
////		}
////	}
////	if(xBall < 0 && dxBall <0)
////	{
////		reset();
////		playerTwoScore++;
////
////		if(playerTwoScore >= 7)
////		{
////			twoWin = true;
////		}
////	}
////
////	//Move ball y
////	if(yBall > APP_SIZE && dyBall >0)
////	{
////		dyBall = -dyBall;
////	}
////	if(yBall < 0 && dyBall <0)
////	{
////		dyBall =-dyBall;
////	}
////
////	//computerize paddle
////	yPaddleTwo = yBall -25;
////
////	// does ball hit paddle 1?
////	bool hit1 = (xBall <= (xPaddleOne +15)) && (yBall >= yPaddleOne && yBall <= yPaddleOne+50);
////	
////	if (hit1)
////	{
////		dxBall = -dxBall;
////		dyBall= dyBall;
////	}
////		// does ball hit paddle 2?
////	bool hit2 = (xBall >= xPaddleTwo) && (yBall >= yPaddleTwo && yBall <= yPaddleTwo+50);
////	
////	if (hit2)
////	{
////		dxBall = -dxBall;
////		dyBall= dyBall;
////	}
////
////}
////
////void TestPaddleMove()
////{
////	if (KeyDown('W')) {
////		// move left paddle up
////	}
////	if (KeyDown('S')) {
////	}
////}
////
////void Idle()
////{	
////    clock_t time = clock();
////	double dt = (double) (time-prevTime)/CLOCKS_PER_SEC;
////	prevTime = time;
////
////	UpdateBall(dt);
////	TestPaddleMove();
////    glutPostRedisplay();
////}
////
////
////void allmystuff()
////{
////	Make2dWindow(100, 100, APP_SIZE, APP_SIZE, "Pong");
////	
////		// set callbacks
////		glutDisplayFunc(Draw);                           // respond to redraw requests
////		glutIdleFunc(Idle);                              // animate ball
////		glutKeyboardFunc(Key);							 //Keystroke
////		// begin event handling, display, and updates
////		glutMainLoop();
////}
////
////void highscores()
////{
////	printf("Me: 5,121,881,821,514,449,848,115\nYou: 0\n");
////	printf("Prepare yourself");
////	getchar();
////	getchar();
////	allmystuff();
////}
////
////void main()
////{
////    // init app window
////	int userCommand = startMenu();
////	if (userCommand == 1)
////		return;
////    if (userCommand == 2)
////		highscores();
////	if (userCommand == 3)
////		option();
////	else {
////		allmystuff();
////	}
////}